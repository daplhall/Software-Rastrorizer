#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>
#include "model_objects.h"
/*
   NOTE >> seems to ignore white spaces?
*/
// TODO bench vs teacher
model::model(std::string filename){
   int idstart = 0;
   std::ifstream in;
   std::string   line;
   in.open (filename, std::ifstream::in);
   if (in.fail()) return;
   while (!in.eof()){ // end of the file (e -end , o -of , f-file
      char trash;
      std::getline(in, line);
      std::istringstream iss(line.c_str());
      if (!line.compare(0, 2, "v ")){
         iss >> trash; // discard v
         float v;
         iss >> v; m_vx.push_back(v);
         iss >> v; m_vy.push_back(v);
         iss >> v; m_vz.push_back(v);
         // read the vertexes, just do a for loop
      }
      else if(!line.compare(0, 3, "vn  ")){
         // read vn here
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
         // read the format of v/vt/vn, and stores the numbers in f,t and n, stores '/' trash
         // TODO write to model
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
float const & model::vx (int const idvertex) const {
   return m_vx[idvertex];
}
float const  & model::vy (int const idvertex) const {
   return m_vy[idvertex];
}
float const & model::vz (int const idvertex) const {
   return m_vz[idvertex];
}
float const & model::vtx (int const idvertex) const {
   return m_vtx[idvertex];
}
float const & model::vty (int const idvertex) const {
   return m_vty[idvertex];
}
float const & model::vtz (int const idvertex) const {
   return m_vtz[idvertex];
}
float const & model::vnx (int const idvertex) const {
   return m_vnx[idvertex];
}
float const & model::vny (int const idvertex) const {
   return m_vny[idvertex];
}
float const  & model::vnz (int const idvertex) const {
   return m_vnz[idvertex];
}

int const  & model::fv (int const idpolygon, int const idvalue) const{
   return m_fv[idvalue + m_ndim *idpolygon];
}
int const  & model::fvt(int const idpolygon, int const idvalue) const{
   return m_fvt[idvalue + m_ndim *idpolygon];
}
int const  & model::fvn(int const idpolygon, int const idvalue) const{
   return m_fvn[idvalue + m_ndim * idpolygon];
}
