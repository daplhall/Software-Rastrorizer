#ifndef H_MYLINALG_H
#define H_MYLINALG_H
#include <cmath>
template<typename T = float> struct vec3d {
   union{
      struct {T x, y, z;}; // now i can use x,y,z to change the values in data! fucking amazing feature, ie same memory different names
      T data[3];
   };
   vec3d()                 : x(0) , y(0) , z(0)  {}
   vec3d(T x_, T y_, T z_) : x(x_), y(y_), z(z_) {} 
   T & operator[](int const i) { return data[i]; } 
   inline vec3d<T> operator+(vec3d<T> const &v) const { return vec3d<T>(x + v.x, y + v.y, z + v.z); }
   inline vec3d<T> operator-(vec3d<T> const &v) const { return vec3d<T>(x - v.x, y - v.y, z - v.z); }
   inline vec3d<T> operator*(vec3d<T> const &v) const { return vec3d<T>(x * v.x, y - v.y, z - v.z); }
   inline vec3d<T> operator*(float f)         { return vec3d<T>(f * x , f * y, f * z);      }
   inline T norm(){ return std::sqrt(x*x + y*y + z*z); }
   void normalize(T length) { 
      T m_norm = norm();
      x *= length/m_norm;
      y *= length/m_norm;
      z *= length/m_norm;
   }
};

template<typename T = float> struct vec2d {
   union{
      struct {T x, y;}; // now i can use x,y,z to change the values in data! fucking amazing feature, ie same memory different names
      T data[2];
   };
   vec2d()                 : x(0) , y(0)  {}
   vec2d(T x_, T y_) : x(x_), y(y_) {} 
   T &operator[](int const i) { return data[i]; }
   inline vec2d<T> &operator+(vec2d<T> const &v){ return vec2d<T>(x + v.x, y + v.y); }
   inline vec2d<T> &operator-(vec2d<T> const &v){ return vec2d<T>(x - v.x, y - v.y); }
   inline vec2d<T> &operator*(vec2d<T> const &v){ return vec2d<T>(x * v.x, y - v.y); }
   inline vec2d<T> &operator*(float f)         { return vec2d<T>(f * x , f * y   ); }
   inline T norm(){ return std::sqrt(x*x + y*y); }
   void normalize(T length) { 
      T m_norm = norm();
      x *= length*m_norm;
      y *= length*m_norm;
   }
};

typedef vec2d<int>   vec2i;
typedef vec3d<int>   vec3i;
typedef vec3d<int>   vec3i;
typedef vec3d<float> vec3f;

template<typename T = vec3f> 
T cross(T v0, T v1){
   T product;
   product.x = v0.y*v1.z - v0.z*v1.y;
   product.y = v1.x*v0.z - v0.x*v1.z;
   product.z = v0.x*v1.y - v0.y*v1.x;
   return product;
}
/*
   R - Standard number type
   T - vector format 
*/
template<typename T = vec3f, typename R = float>
R AreaCrossPlane(T v0, T v1){
   return v0.x*v1.y - v0.y*v1.x;
}
float dot (vec3f v0, vec3f v1);

#endif //  H_MYLINALG_H