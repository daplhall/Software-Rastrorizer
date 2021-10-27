#ifndef H_MODELOBJECT_H
#define H_MODELOBJECT_H

#include <string>
#include <vector>
#include <algorithm>
#include "myLinAlg.h"

// Data container for the vertecies and such.
class model{
   private:
   //NOTE test perforamce of vec3f vs 3 different arryas
      //the best solution is a hybrid 
      std::vector<vec3f> m_v;   // vertexes; XYZXYZ.. format should be fine in terms of memory catching for each vertex basicly doesn't care for eachother, although you can test the performan
      std::vector<vec3f> m_vt;  // texture values X
      std::vector<vec3f> m_vn;  // normal values  X
      std::vector<int>   m_fv;   // face vertex  indexes needs to be mapped like a matrix.
      std::vector<int>   m_fvt; // face texture indexes
      std::vector<int>   m_fvn; // face normals indexes
      int m_ndim = 3;
   public:
   //make private
      int m_nfaces, m_ntextures, m_nnormals, m_nvertexes;
      int nfaces(){return m_fv.size()/m_ndim;}
      int nverts(){return m_v.size();}

      model(std::string filename);
      // rename to vertt
      inline vec3f const & vert(int const idvertex) const   {return m_v[idvertex];}
      inline vec3f const & vt  (int const idvertex) const   {return m_vt[idvertex];}
      inline vec3f const & vnorm (int const idvertex) const {return m_vn[idvertex];}
      inline int const  & fvert(int const idpolygon, int const idvalue) const {return m_fv[idvalue + m_ndim *idpolygon];}
      inline int const  & fvt  (int const idpolygon, int const idvalue) const {return m_fvt[idvalue + m_ndim *idpolygon];}
      inline int const  & fvn  (int const idpolygon, int const idvalue) const {return m_fvn[idvalue + m_ndim * idpolygon];}

      // TODO: add a combination of fvert and vert, so i just get the 
      //       wanted triangle coord, ie mask vert(fvert(i,j));
};
#endif //H_MODELOBJECT_H