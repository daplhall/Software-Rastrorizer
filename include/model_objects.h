#ifndef H_MODELOBJECT_H
#define H_MODELOBJECT_H

#include <string>
#include <vector>

// Data container for the vertecies and such.
class model{
   private:
      std::vector<float> m_vx;   // vector positons in x
      std::vector<float> m_vy;   // -||-               y
      std::vector<float> m_vz;   // -||-               z
      std::vector<float> m_vtx;  // texture values X
      std::vector<float> m_vty;  // texture values Y
      std::vector<float> m_vtz;  // texture values Z
      std::vector<float> m_vnx;  // normal values  X
      std::vector<float> m_vny;  // normal values  Y
      std::vector<float> m_vnz;  // normal values  Z
      std::vector<int>   m_fv;   // face vertex  indexes needs to be mapped like a matrix.
      std::vector<int>   m_fvt; // face texture indexes
      std::vector<int>   m_fvn; // face normals indexes
      int m_ndim = 3;
   public:
   //make private
      int m_nfaces, m_ntextures, m_nnormals, m_nvertexes;
      int nfaces(){return m_fv.size()/m_ndim;}
      int nvertexes(){return m_vx.size();}

      model(std::string filename);
      // rename to vertt
      float  const  & vx (int const idvertex) const;
      float  const  & vy (int const idvertex) const;
      float  const  & vz (int const idvertex) const;
      float  const  & vtx(int const idvertex) const;
      float  const  & vty(int const idvertex) const;
      float  const  & vtz(int const idvertex) const;
      float  const  & vnx(int const idvertex) const;
      float  const  & vny(int const idvertex) const;
      float  const  & vnz(int const idvertex) const;
      int  const  & fv (int const idvertex, int const idvalue) const;
      int  const  & fvt(int const idvertex, int const idvalue) const;
      int  const  & fvn(int const idvertex, int const idvalue) const; 
};
#endif