#include <iostream>
#include <vector>
#include <chrono>

#include "myWindowsConsle.h"
#include "model_objects.h"
#include "myLinAlg.h"
#include "rastorising.h"

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
vec3f WorldToScreen(vec3f A, TGAimage const &image){
   return vec3f(int(0.5 * image.getWidth()  * (A.x + 1)) ,\
                int(0.5 * image.getHeight() * (A.y + 1)), \
                A.z);
}

int main(int argv, char *argc[]){
   const int width  = 300;
   const int height = 300;
   int Cformat = (argv > 1) ? TGAimage::RGB : TGAimage::WindowsConsole;
   
   float *zbuffer = new float[width*height];
   for (int i=width*height; i--; zbuffer[i] = -std::numeric_limits<float>::max());
   
   vec3f lightdir(0,0,1);

   TGAimage image(width, height, Cformat);;
   TGAcolor white(255, 255, 255);
   model african("./models/african_head.obj");

   image.TGAread("./models/cbw8.tga");
   // there is a problem when widht and height are off center
   /*
   for (int face = 0; face < african.nfaces(); face++){
      vec3f ImgCord[3];
      vec3f FCord[3];
      for (int j=0; j<3; j++) {
         ImgCord[j] = WorldToScreen(african.vert(african.fvert(face,j)),image);
         FCord[j]   = african.vert(african.fvert(face,j));
      }
      vec3f v10 = FCord[1] - FCord[0];
      vec3f v20 = FCord[2] - FCord[0];
      vec3f normal = cross(v10, v20);
      normal.normalize(1);
      float inten/ty = dot(normal, lightdir);  
      if (intensity > 0) { 
         TGAcolor Crand(rand()/255, rand()/255, rand()/255);
         TGAcolor Light(intensity*255, intensity*255, intensity*255);
         filledtriangle(ImgCord[0], ImgCord[1], ImgCord[2], zbuffer, image, Crand);
      }
   }
   
   image.VerticalFlip()
*/
////////
   if (argv > 1) {image.TGAwrite("./TGA/test.tga", 01, 0);}
   else{
      windowsconsole win;
      win.CreateScreenWithBuffer(image.getWidth(),image.getHeight(),2,2);
      win.TGAToBuffer(image);
      std::cin.ignore();
   }
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

/*
   for (int face = 0; face < african.nfaces(); face++){
      vec3f fcoord  [3];

      TGAcolor Crand(rand()%255, rand()%255, rand()%255);
      for (int j = 0; j < 3; j++){
         vec3f vert  = african.vert(african.fvert(face,j));
         fcoord  [j] = vert;
      }
      vec3f v21 = fcoord[2] - fcoord[1];
      vec3f v10 = fcoord[2] - fcoord[0]; // not all triangles are right orientated? for of x = 1 y =0 and z = 0 for light tehn we get oposite light
      vec3f Xdot = cross(v10, v21);// assume that the obj file is always postivly orientated.
      Xdot.normalize(1);
      float intensity = 1;//dot(light_dir, Xdot);
      if (intensity > 0){ // bigger than zero as the norms i calulate point into the object, so they need to be parallel in the same direction to be light.
         TGAcolor WhiteLight(intensity*255, intensity*255, intensity*255);
         vec2i v0 = WorldToScreen(fcoord[0],image);
         vec2i v1 = WorldToScreen(fcoord[1],image);
         vec2i v2 = WorldToScreen(fcoord[2],image);
         filledtriangle(v0, v1, v2, image, Crand);
      }
   }
   */