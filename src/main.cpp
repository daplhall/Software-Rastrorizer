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

int main(int argv, char *argc[]){
   const int width  = 450;
   const int height = 300;
   vec3f light_dir(0,0, 1);
   int ybuffer[width*height] = {std::numeric_limits<int>::min()};

   TGAimage image(width, height, TGAimage::RGB);
   TGAcolor red  (255, 0  , 0  );
   TGAcolor green(0  , 255, 0  );
   TGAcolor blue (0  , 0, 255  );
   TGAcolor white(255, 255, 255);
   model african("./models/african_head.obj");   
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
      float intensity =1;// dot(light_dir, Xdot);
      if (intensity > 0){ // bigger than zero as the norms i calulate point into the object, so they need to be parallel in the same direction to be light.
         TGAcolor WhiteLight(intensity*255, intensity*255, intensity*255);
         filledtriangle(Imgcoord[0], Imgcoord[1], Imgcoord[2],  image, WhiteLight);
      }
   }
   image.TGAwrite("./TGA/test.tga", 1);
   windowsconsole win;
   win.CreateScreenWithBuffer(width,height,1,1);
   win.TGAToBuffer(image);
   std::cin.ignore();
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