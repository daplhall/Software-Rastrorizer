#ifndef H_RAZTORIZER_H
#define H_RAZTORIZER_H
#include "TGAImage.h"
#include "myLinAlg.h"

void line          (int x0, int y0, int x1, int y1, TGAimage &image, TGAcolor &color);
void line          (vec2i v0, vec2i v1, TGAimage &image, TGAcolor &color);
void filledtriangle(int vx0, int vy0, int vx1, int vy1, int vx2, int vy2, TGAimage &image, TGAcolor &color);
void filledtriangle(vec2i v0, vec2i v1, vec2i v2,                 TGAimage &image, TGAcolor &color);
void filledtriangle(vec3f v0, vec3f v1, vec3f v2,                 TGAimage &image, TGAcolor &color);
void filledtriangle(vec3f v0, vec3f v1, vec3f v2, float *zbuffer, TGAimage &image, TGAcolor &color);
void raztorize2D   (vec2i v0, vec2i v1, TGAimage &image, TGAcolor &color, int *ybuffer);
float ZpolateBary  (vec3f v0, vec3f v1, vec3f v2, vec3f p);
#endif //H_RAZTORIZER_H