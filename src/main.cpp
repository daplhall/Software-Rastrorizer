#include <iostream>
#include <math.h>
#include <algorithm>
#include <vector>
#include <chrono>
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

void line1(int x0, int y0, int x1, int y1, TGAimage &image, TGAcolor color) {
    bool steep = false;
    if (std::abs(x0-x1)<std::abs(y0-y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0>x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    for (int x=x0; x<=x1; x++) {
        float t = (x-x0)/(float)(x1-x0);
        int y = y0*(1.-t) + y1*t;
        if (steep) {
            image.draw(y, x, color);
        } else {
            image.draw(x, y, color);
        }
    }
}

class vector3d
{
   private://         0  1  2  3  4  5
      int data[6] = {20,20,40,50,20,80};
      int nrows = 3;
      int ncols = 2;
   public:
      int & operator[](int const i) { return  data[i]; }
      int & at(int const i, int const j) {
         return data[j + ncols*i];
         }

};


int main(int argv, char *argc[]){
   const int width  = 800;
   const int height = 800;
   vector3d vec;

   TGAimage image(width, height, TGAimage::RGB);
   TGAcolor red(255,0,0);
   TGAcolor green(0,255,0);
   TGAcolor white(255,255,255);
   
   model african("./models/african_head.obj");
   /*
   int face = african.nfaces() -1;
   for (int j = 0; j < 3;j++){
   std::cout << "faces(index from 1)::" << " 1: " << african.fv(face, j) +1<< " 2: " << african.fv(face, (j+1)%3) +1 << std::endl;
   std::cout << "x: "<< std::endl;
   std::cout << african.vx(african.fv(face, j)) << "\t";
   std::cout << african.vx(african.fv(face, (j+1)%3)) << std::endl ;
   std::cout << "y: "<< std::endl;
   std::cout << african.vy(african.fv(face, j)) << "\t";
   std::cout << african.vy(african.fv(face, (j+1)%3)) << std::endl ;
   }
   */
   
   for (int face = 0; face < 10; face++){
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
         std::cout << "faces: \t" << african.fv(face, j) << " " << african.fv(face, (j+1)%3) << std::endl;
         std::cout << "vertex: \t" << vx0 << " "  << vy0 << " " << vx1<< " " <<vy1 << std::endl;
         std::cout << "pixel: \t" << x0 << " "  << y0 << " " << x1<< " " << y1 << std::endl;
      }
   }
   
   // line(vec.at(0,0), vec.at(0,1), vec.at(1,0), vec.at(1,1), test, green);
   // line(vec.at(1,0), vec.at(1,1), vec.at(2,0), vec.at(2,1), test, green);
   // line(vec.at(2,0), vec.at(2,1), vec.at(0,0), vec.at(0,1), test, white);
   image.TGAwrite("./TGA/test.tga", 1);
/*make
   windowsconsole win;
   win.CreateScreenWithBuffer(width,height,1,1);
   win.TGAToBuffer(test);
*/
   //std::cin.ignore();

   return 0;
}    