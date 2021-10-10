#ifndef H_MATHEMATICS_H
#define H_MATHEMATICS_H

template<typename T> struct vec3d {
   union{
      struct {T x,y,z}; // now i can use x,y,z to change the values in data! fucking amazing feature, ie same memory different names
      T data[3];
   }
   T &operator[int const i] {return data[i]}; 

};

template<typename T> struct vec2d {
   union{
      struct {T x,y}; // now i can use x,y,z to change the values in data! fucking amazing feature, ie same memory different names
      T data[2];
   }
   T &operator[int const i] {return data[i]}; 
   
};

typedef vec2d<int>   vec2i;
typedef vec3d<float> vec3f;


#endif //  H_MATHEMATICS_H