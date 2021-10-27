#include "myLinAlg.h"


float dot(vec3f v0, vec3f v1){
   return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z;
}
float Cross2D(vec3f &va, vec3f &vb){
   return va.x*vb.y - va.y*vb.x;
}