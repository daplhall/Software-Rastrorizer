#include <iostream>
#include <math.h>
#include <algorithm>
#include <vector>
#include <chrono>
#include <random>
#include "TGAImage.h"
#include "myWindowsConsle.h"
#include "model_objects.h"

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
                                                            
void line_old(int x0, int y0, int x1, int y1, TGAimage &image, TGAcolor &color) { 
   float slope ;
   if (x0 > x1){// NOTE has an error here for if y0 > y1 then we fail longer down
      std::swap(x0,x1);
      std::swap(y0,y1);
   }
   int dx = x1 - x0; // will always be positive after swap
   int dy = y1 - y0;
   if (dx < std::abs(dy)){ // transpose and draw the line
      std::swap(x0,y0);
      std::swap(x1,y1);
      std::swap(dx,dy);
      slope = dy/(float)dx;
      for (int x=x0; x<=x1; x++) { 
         int y = y0 + (x-x0)*slope; // this is better than y0*(1-t) + y1*t as we get y0 if y  = 0 we also go down 1 calulation operations
         image.draw(y, x, color);
      }
   }  
   else {
      slope = dy/(float)dx;
      for (int x=x0; x<=x1; x++) { // draw the line normally
         int y = y0 + (x-x0)*slope; // this is better than y0*(1-t) + y1*t as we get y0 if y  = 0 we also go down 1 calulation operations
         image.draw(x, y, color); 
      }
   }

}
/*
  TODO Test performance vs Teacher and old
  NOTE refactored so that we transpose before and set a state in "transpose"
       test if the unrefactored is faster or slower, midly curious.
       Also test pulling y0 > y1 ? -1 : 1 out so it becomes a const.
       This might increase performance, or it might also be a micro optimization
       Also try for fun to check the error between our new line draw and the old, to see
       how they differ, and also who is the better one.
       test if it is faster to store dx before transpose and then multiply by 1
       or do the dx calulations in transposed if and then later re calulate them
*/
void line(int x0, int y0, int x1, int y1, TGAimage &image, TGAcolor &color) { 
   bool transpose = false;
   int dx = x1 - x0; // will always be positive after swap
   int dy = y1 - y0;
   if (std::abs(dx) < std::abs(dy)){ // transposed
      transpose = true;
      std::swap(dx, dy);
      std::swap(x0, y0);
      std::swap(x1, y1);
   }   
   if (x0 > x1){
      std::swap(x0,x1);
      std::swap(y0,y1);
      dx *= -1;
      dy *= -1;
   }
   int dySum = 0;
   int dyabs = std::abs(dy);
   int y = y0;
   if (transpose){
      for (int x = x0; x <= x1; x++){
         image.draw(y, x, color);
         dySum += dyabs;
         if (dySum >= dx){
            y     += y1>y0 ? 1 : -1;
            dySum -= dx;
         }
      }
   }
   else{
      for (int x = x0; x <= x1; x++){
         image.draw(x, y, color);
         dySum += dyabs;
         if (dySum >= dx){
            y     += y1>y0 ? 1 : -1;
            dySum -= dx; // So the criteria holds
         }
      }
   }
}

float max(float x, float y){
   if (x > y){
      return x;
   }
   return y;
}
/*
Works by splitting the trangle in 2 - Appraently the "standard algorithm" by 1 source
TODO make it go draw along the x axis isntead of the y, this makes it better for the memory access.
TODO learn other algorithms
*/
void filledtriangle(int vx0, int vy0, int vx1, int vy1, int vx2, int vy2, TGAimage &image, TGAcolor &color){
   
   if (vx0 > vx1){
      std::swap(vx0, vx1);
      std::swap(vy0, vy1);
   }
   if (vx0 > vx2){
      std::swap(vx0, vx2);
      std::swap(vy0, vy2);
   }
   if (vx1 > vx2){
      std::swap(vx1,vx2);
      std::swap(vy1,vy2);
   }
   // vx0 is now always the samllest and vx2 is alwoasy the biggest
   for (int x = vx0; x <= vx1; x++){
      int xdiff = x-vx0;
      float t01 = xdiff/(vx1-vx0 + 1e-15);
      float t02 = xdiff/(vx2-vx0 + 1e-15);
      int y01 = t01*(vy1 - vy0) + vy0;
      int y02 = t02*(vy2 - vy0) + vy0;
      line(x, y01, x, y02, image, color);
   }
   for (int x = vx1 + 1; x <= vx2; x++){
      //std::cout << x << " " << vx1 << " " << vx2 << std::endl;
      float t21 = (x-vx1)/(vx2-vx1 + 1e-15);
      float t02 = (x-vx0)/(vx2-vx0 + 1e-15);
      int y21 = t21*(vy2 - vy1) + vy1;
      int y02 = t02*(vy2 - vy0) + vy0;
      line(x, y21, x, y02, image, color);
   }
}


int main(int argv, char *argc[]){
   const int width  = 450;
   const int height = 300;

   TGAimage image(width, height, TGAimage::WindowsConsole);
   TGAcolor red(255,0,0);
   TGAcolor green(0,255,0);
   TGAcolor white(255,255,255);
   model african("./models/african_head.obj");   
   /*
   for (int face = 0; face < african.nfaces(); face++){
      for (int j = 0; j < 3; j++){
         float vx0 = african.vx(african.fv(face, j));
         float vy0 = african.vy(african.fv(face, j));
         float vx1 = african.vx(african.fv(face, (j+1)%3)); // this can be built into the fv func
         float vy1 = african.vy(african.fv(face, (j+1)%3));
         int x0 = (vx0+1.)*width /2.; //we add 1 because of the negative values and we are centered around 0 withing -1 and 1
         int y0 = (vy0+1.)*height/2.; 
         int x1 = (vx1+1.)*width /2.; 
         int y1 = (vy1+1.)*height/2.; 
         line(x0, y0, x1, y1, image, white); 
      }
   }
   */
   for (int face = 0; face < african.nfaces(); face++){
         float vx0 = african.vx(african.fv(face, 0));
         float vy0 = african.vy(african.fv(face, 0));
         float vx1 = african.vx(african.fv(face, 1)); // this can be built into the fv func
         float vy1 = african.vy(african.fv(face, 1));
         float vx2 = african.vx(african.fv(face, 2));
         float vy2 = african.vy(african.fv(face, 2));
         
         int x0 = (vx0+1.)*width /2.; //we add 1 because of the negative values and we are centered around 0 withing -1 and 1
         int y0 = (vy0+1.)*height/2.; 
         int x1 = (vx1+1.)*width /2.; 
         int y1 = (vy1+1.)*height/2.; 
         int x2 = (vx2+1.)*width /2.; 
         int y2 = (vy2+1.)*height/2.; 
      
         white[0] = rand()%255;
         //white[1] = rand()%255;
         //white[2] = rand()%255;
         filledtriangle(x0 , y0 ,  \
                        x1 , y1, \
                        x2 , y2, image, white);
                        
   }

/*
   filledtriangle(0, 0, \
                  30, 50, \
                  80, 20, image, white);
  */ 
   // line(vec.at(0,0), vec.at(0,1), vec.at(1,0), vec.at(1,1), test, green);
   // line(vec.at(1,0), vec.at(1,1), vec.at(2,0), vec.at(2,1), test, green);
   // line(vec.at(2,0), vec.at(2,1), vec.at(0,0), vec.at(0,1), test, white);
   
   //image.TGAwrite("./TGA/test.tga", 1);
   
   windowsconsole win;
   win.CreateScreenWithBuffer(width,height,1,1);
   win.TGAToBuffer(image);
   std::cin.ignore();
   
   return 0;
}    