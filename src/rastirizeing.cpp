#include <algorithm>
#include <vector>
#include <math.h>
#include "rastorising.h"
#include <iostream>

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


void filledtriangle(vec3f v0, vec3f v1, vec3f v2, float *zbuffer, TGAimage &image, TGAcolor &color){
   // maybe add heigh widht and 0 as some more limits
   int bbox[4] = {std::min( v2.x, std::min(v0.x, v1.x)),  // 0: xmin
                  std::min( v2.y, std::min(v0.y, v1.y)),  // 1: ymin
                  std::max( v2.x, std::max(v0.x, v1.x)),  // 2: xman
                  std::max( v2.y, std::max(v0.y, v1.y))}; // 3: ymax
   vec3f v10 = v1 - v0; 
   vec3f v20 = v2 - v0;
   float A = Cross2D(v20, v10);
   vec3f p;
   for ( p.y = bbox[1]; p.y < bbox[3]; p.y++){
      for ( p.x = bbox[0]; p.x < bbox[2]; p.x++){
         vec3f q = p - v0;
         float u = Cross2D(v20, q)/A; // v1 area
         float v = Cross2D(q, v10)/A; // v2 area
         float z = (1-u-v)*v0.z + u*v1.z + v*v2.z;
         int zid = p.x+p.y*image.getWidth();
         if (u >= 0 && v >= 0 && 1 >= u + v  && z > zbuffer[zid]){
            zbuffer[zid] = z;
            image.draw(p.x, p.y, color); 
         }
      }
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


