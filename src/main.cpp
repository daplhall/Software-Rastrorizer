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
vec2i WorldToScreen(vec3f A, TGAimage const &image){
   return vec2i(0.5 * image.getWidth() * (A.x + 1) , 0.5 * image.getWidth() * (A.y + 1));
}

int main(int argv, char *argc[]){
   const int width  = 800;
   const int height = 800;
   vec3f light_dir(0,0, 1);
   float *zBuffer = new float[width*height];
   for (int i=width*height; i--; zBuffer[i] = -std::numeric_limits<float>::max());
   int Cformat = (argv > 1) ? TGAimage::RGB : TGAimage::WindowsConsole;
   TGAimage image(width, height, Cformat);
   TGAcolor red  (255, 0  , 0  );
   TGAcolor green(0  , 255, 0  );
   TGAcolor blue (0  , 0, 255  );
   TGAcolor white(255, 255, 255);
   model african("./models/african_head.obj");   
// african.nfaces()
   for (int face = 0; face < african.nfaces(); face++){
      vec3f Imgcoord[3];
      vec3f fcoord  [3];
      TGAcolor Crand(rand()%255, rand()%255, rand()%255);
      for (int j = 0; j < 3; j++){
         vec3f vert  = african.vert(african.fvert(face,j));
         Imgcoord[j] = vec3f(int(0.5*width *(vert.x + 1.)), int(0.5*height*(vert.y + 1.)), vert.z);
         fcoord  [j] = vert;
      }
      vec3f v10 = fcoord[2] - fcoord[0]; // not all triangles are right orientated? for of x = 1 y =0 and z = 0 for light tehn we get oposite light
      vec3f v21 = fcoord[2] - fcoord[1];
      vec3f Xdot = cross(v10, v21);// assume that the obj file is always postivly orientated.
      Xdot.normalize(1);
      float intensity = 1;//dot(light_dir, Xdot);
      if (intensity > 0){ // bigger than zero as the norms i calulate point into the object, so they need to be parallel in the same direction to be light.
         TGAcolor WhiteLight(intensity*255, intensity*255, intensity*255);
         filledtriangle(Imgcoord[0], Imgcoord[1], Imgcoord[2],zBuffer, image, Crand);
      }
   }
   if (argv > 1) {image.TGAwrite("./TGA/test.tga", 1);}
   else{
      windowsconsole win;
      win.CreateScreenWithBuffer(width,height,1,1);
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