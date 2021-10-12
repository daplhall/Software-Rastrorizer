#include <iostream>
#include <math.h>
#include <algorithm>
#include <vector>
#include <chrono>
#include <random>
#include "TGAImage.h"
#include "myWindowsConsle.h"
#include "model_objects.h"
#include "myLinAlg.h"

class windowsconsole: public WindowsConsole{

   bool UserInit(){
      return 0;
   }
   bool UserTick(float dt){
      return 0;
   }
   bool UserClean(){
         return 0;
   }
};

void line(int x0, int y0, int x1, int y1, TGAimage &image, TGAcolor &color) { 
   bool transpose = false;
   int dx = std::abs(x1 - x0); // will always be positive after swap
   int dy = std::abs(y1 - y0);
   if (dx < dy){ // transposed
      transpose = true;
      std::swap(dx, dy);
      std::swap(x0, y0);
      std::swap(x1, y1);
   }   
   if (x0 > x1){
      std::swap(x0,x1);
      std::swap(y0,y1);
   }
   int dySum = 0;
   int y = y0;
   if (transpose){
      for (int x = x0; x <= x1; x++){
         image.draw(y, x, color);
         dySum += dy;
         if (dySum >= dx){
            y     += y1>y0 ? 1 : -1;
            dySum -= dx;
         }
      }
   }
   else{
      for (int x = x0; x <= x1; x++){
         image.draw(x, y, color);
         dySum += dy;
         if (dySum >= dx){
            y     += y1>y0 ? 1 : -1;
            dySum -= dx; // So the criteria holds
         }
      }
   }
}

void line(vec2i v0, vec2i v1, TGAimage &image, TGAcolor &color) { 
   bool transpose = false;
   int dx = std::abs(v1.x - v0.x); // will always be positive after swap
   int dy = std::abs(v1.y - v0.y);
   if (dx < dy){ // transposed
      transpose = true;
      std::swap(dx, dy);
      std::swap(v0.x, v0.y);
      std::swap(v1.x, v1.y);
   }   
   if (v0.x > v1.x) std::swap(v0, v1);
   int dySum = 0;
   int y = v0.y;
   if (transpose){
      for (int x = v0.x; x <= v1.x; x++){
         image.draw(y, x, color);
         dySum += dy;
         if (dySum >= dx){
            y     += v1.y>v0.y ? 1 : -1;
            dySum -= dx;
         }
      }
   }
   else{
      for (int x = v0.x; x <= v1.x; x++){
         image.draw(x, y, color);
         dySum += dy;
         if (dySum >= dx){
            y     += v1.y>v0.y ? 1 : -1;
            dySum -= dx; // So the criteria holds
         }
      }
   }
}

/*
Works by splitting the trangle in 2 - Appraently the "standard algorithm" by 1 source
TODO make it go draw along the x axis isntead of the y, this makes it better for the memory access.
TODO learn other algorithms
*/
void filledtriangle(int vx0, int vy0, int vx1, int vy1, int vx2, int vy2, TGAimage &image, TGAcolor &color){
   if (vy0 > vy1){
      std::swap(vx0, vx1);
      std::swap(vy0, vy1);
   }
   if (vy0 > vy2){
      std::swap(vx0, vx2);
      std::swap(vy0, vy2);
   }
   if (vy1 > vy2){
      std::swap(vx1,vx2);
      std::swap(vy1,vy2);
   }

   for (int y = vy0; y <= vy1; y++){
      float t01 = (y - vy0)/(vy1 - vy0 + 1e-15);
      float t02 = (y - vy0)/(vy2 - vy0 + 1e-15);
      int   x01 = t01*(vx1 - vx0) + vx0;
      int   x02 = t02*(vx2 - vx0) + vx0;
      //* Test why line is faster that image.draw and if ti is faster at O3
      if (x01 > x02) {std::swap(x01,x02);}
      for (int x = x01; x <= x02; x++){
         if 
         image.draw(x, y, color);
      }
      //line(x01, y, x02, y ,image, color);
   }
   for (int y = vy1 + 1; y <= vy2; y++){
      float t21 = (y - vy1)/(vy2 - vy1 + 1e-15);
      float t02 = (y - vy0)/(vy2 - vy0 + 1e-15);
      int   x21 = t21*(vx2 - vx1) + vx1;
      int   x02 = t02*(vx2 - vx0) + vx0;
      if (x21 > x02) {std::swap(x21,x02);}
      for (int x = x21; x <= x02; x++){
         image.draw(x, y, color);
      }
      //line(x21, y, x02, y ,image, color);
   }
}
void filledtriangle(vec2i v0, vec2i v1, vec2i v2, TGAimage &image, TGAcolor &color){
   if (v0.y > v1.y) std::swap(v0, v1);
   if (v0.y > v2.y) std::swap(v0, v2);
   if (v1.y > v2.y) std::swap(v1, v2);
   // Upper triangle
   for (int y = v0.y; y <= v1.y; y++){
      float t01 = (y - v0.y)/(v1.y - v0.y + 1e-15);
      float t02 = (y - v0.y)/(v2.y - v0.y + 1e-15);
      int   x01 = t01*(v1.x - v0.x) + v0.x;
      int   x02 = t02*(v2.x - v0.x) + v0.x;
      //* Test why line is faster that image.draw and if ti is faster at O3
      if (x01 > x02) std::swap(x01,x02);
      for (int x = x01; x <= x02; x++){
         image.draw(x, y, color);
      }
      //line(x01, y, x02, y ,image, color);
   }
   // Bottom Trianlge
   for (int y = v1.y + 1; y <= v2.y; y++){
      float t21 = (y - v1.y)/(v2.y - v1.y + 1e-15);
      float t02 = (y - v0.y)/(v2.y - v0.y + 1e-15);
      int   x21 = t21*(v2.x - v1.x) + v1.x;
      int   x02 = t02*(v2.x - v0.x) + v0.x;
      if (x21 > x02) {std::swap(x21,x02);}
      for (int x = x21; x <= x02; x++){
         image.draw(x, y, color);
      }
      //line(x21, y, x02, y ,image, color);
   }
}
/*
   Y buffer rastorizer for a 2D to 1D
   https://github.com/ssloy/tinyrenderer/wiki/Lesson-3:-Hidden-faces-removal-(z-buffer)
   it draws in layers, the first line is always drawn and then the second is only drawn if it is over the first and so on.
*/
void raztorize2D(vec2i v0, vec2i v1, TGAimage &image, TGAcolor &color, int *ybuffer){
   if (v0.x > v1.x) std::swap(v0, v1);
   for (int x = v0.x; x < v1.x; x++){
      float t = (x-v0.x)/(float)(v1.x - v0.x);
      int y = v0.y + t*(v1.y - v0.y);
      if (y > ybuffer[x]){
         ybuffer[x] = y;
         image.draw(x, 0, color);
      }  
   }  
}



int main(int argv, char *argc[]){
   const int width  = 800;//450;
   const int height = 1;//300;
   vec3f light_dir(0,0, 1);
   int ybuffer[width*height] = {std::numeric_limits<int>::min()};

   TGAimage image(width, height, TGAimage::RGB);
   TGAcolor red  (255, 0  , 0  );
   TGAcolor green(0  , 255, 0  );
   TGAcolor blue (0  , 0, 255  );
   TGAcolor white(255, 255, 255);
   //model african("./models/african_head.obj");   
   raztorize2D(vec2i(20, 34),   vec2i(744, 400), image, red,   ybuffer);
   raztorize2D(vec2i(120, 434), vec2i(444, 400), image, green, ybuffer);
   raztorize2D(vec2i(330, 463), vec2i(594, 200), image, blue,  ybuffer);

   image.TGAwrite("./TGA/test.tga", 1);
   /*
   windowsconsole win;
   win.CreateScreenWithBuffer(width,height,1,1);
   win.TGAToBuffer(image);
   std::cin.ignore();
   */
   return 0;
} 
/*
   for (int face = 0; face < african.nfaces(); face++){
      vec2i Imgcoord[3];
      vec3f fcoord  [3];
      TGAcolor Crand(rand()%255, rand()%255, rand()%255);
      for (int j = 0; j < 3; j++){
         vec3f vert  = african.vert(african.fvert(face,j));
         Imgcoord[j] = vec2i(0.5*width *(vert.x + 1.), 0.5*height*(vert.y + 1.));
         fcoord  [j] = vert;
      }
      vec3f v10 = fcoord[2] - fcoord[0]; // not all triangles are right orientated? for of x = 1 y =0 and z = 0 for light tehn we get oposite light
      vec3f v21 = fcoord[2] - fcoord[1];
      vec3f Xdot = cross(v10, v21);// assume that the obj file is always postivly orientated.
      Xdot.normalize(1);
      float intensity = dot(light_dir, Xdot);
      if (intensity > 0){ // bigger than zero as the norms i calulate point into the object, so they need to be parallel in the same direction to be light.
         TGAcolor WhiteLight(intensity*255, intensity*255, intensity*255);
         filledtriangle(Imgcoord[0], Imgcoord[1], Imgcoord[2],  image, WhiteLight);
      }
   }
*/