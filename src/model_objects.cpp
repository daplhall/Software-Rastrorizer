#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>
#include "model_objects.h"
#include "myLinAlg.h"
/*
   NOTE >> seems to ignore white spaces?
*/
// TODO bench vs teacher
model::model(std::string filename){
   int idstart = 0;
   std::ifstream in;
   std::string   line;
   vec3f         vec_buffer;// might be bad idea if an element used is not 3D
   in.open (filename, std::ifstream::in);
   if (in.fail()) return;
   while (!in.eof()){ // end of the file (e -end , o -of , f-file
      char trash;
      std::getline(in, line);
      std::istringstream iss(line.c_str());
      if (!line.compare(0, 2, "v ")){
         iss >> trash; // discard v
         for (int i = 0; i < 3; i++) iss >> vec_buffer[i];
         m_v.push_back(vec_buffer);
         // read the vertexes, just do a for loop
      }
      else if(!line.compare(0, 3, "vn  ")){
         iss >> trash;
         for (int i = 0; i < 3; i++) iss >> vec_buffer[i];
         m_vt.push_back(vec_buffer);

      }
      else if(!line.compare(0, 3, "vt  ")){
         //read vt ie texture
      }
      else if(!line.compare(0, 2, "s ")){
         iss >> trash;
         iss >> idstart; //checks if we index from 0 or 1
      }
      else if(!line.compare(0, 2, "f "))
      {
         iss >> trash;
         int f,t,n;
         int triangular = 0;
         while (iss >> f >> trash >> t >> trash >> n){ // why does this work? what is the logic?
         // reads the format of v/vt/vn, and stores the numbers in f,t and n, stores '/' trash
            triangular++;
            m_fv .push_back(f-=idstart);  
            m_fvt.push_back(t-=idstart);  
            m_fvn.push_back(n-=idstart);  
         }
         if (triangular != 3){
            std::cerr << "Error -> The obj files build from triangles, ie. not triangulated" << std::endl;
            in.close();
            return;
         }
      }      
   }
}
// assert?
vec3f const & model::vert(int const idvertex) const {
   return m_v[idvertex];
}
vec3f const & model::vt  (int const idvertex) const {
   return m_vt[idvertex];
}
vec3f const & model::vnorm (int const idvertex) const {
   return m_vn[idvertex];
}
int const  & model::fvert(int const idpolygon, int const idvalue) const{
   return m_fv[idvalue + m_ndim *idpolygon];
}
int const  & model::fvt  (int const idpolygon, int const idvalue) const{
   return m_fvt[idvalue + m_ndim *idpolygon];
}
int const  & model::fvn  (int const idpolygon, int const idvalue) const{
   return m_fvn[idvalue + m_ndim * idpolygon];
}
