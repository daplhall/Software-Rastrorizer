#include <iostream>
#include <math.h>
#include <algorithm>
#include <vector>
#include "TGAImage.h"
#include <myWindowsConsle.h>

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
   std::cout << x0 << " " << y0 <<  " " << x1 << " " << y1 << std::endl;
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
   int a;
   int width  = 200;
   int height = 200;
   vector3d vec;

   TGAimage test(width, height, TGAimage::RGB);
   TGAcolor red(255,0,0);
   TGAcolor green(0,255,0);
   TGAcolor white(255,255,255);
        
   line(vec.at(0,0), vec.at(0,1), vec.at(1,0), vec.at(1,1), test, red);
   line(vec.at(1,0), vec.at(1,1), vec.at(2,0), vec.at(2,1), test, green);
   line(vec.at(2,0), vec.at(2,1), vec.at(0,0), vec.at(0,1), test, white);

   test.TGAwrite("./TGA/test.tga", 1);
/*
   windowsconsole win;
   win.CreateScreenWithBuffer(width,height,1,1);
   win.TGAToBuffer(test);
*/
   std::cin.ignore();

   return 0;
}    