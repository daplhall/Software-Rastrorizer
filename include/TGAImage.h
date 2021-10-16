// http://www.paulbourke.net/dataformats/tga/
#ifndef H_TGAIMG_H
#define H_TGAIMG_H
// char = 1 byte = 8 bits
#include <cstdint>
#include <vector>
#include <string>
#include "myLinAlg.h"
// This is just a header of the file, note the data
#pragma pack(push,1)// Needed to stop padding for this file, it needs exaclty 18 bytes.
struct TGAHeader{             // Comments here are for a color mapped images
      uint8_t  IDlength{};      // length of the image identification field
      uint8_t  CmapType{};      // what type of color map is the img, RGB etc,
      uint8_t  DataTypeCode{};  // weather or not this is  uncompressed RPH full length etc. Always contain binary 1?
      uint16_t ColorMapOrigin{};// index of first color map  entry
      uint16_t ColorMapLength{};// count of the color map entry
      uint8_t  ColorMapDepth{}; // number of bits in each color map
      uint16_t x_origin{};      // x coordinate of the lower left corner
      uint16_t y_origin{};      // y coordinate of the lower left corner
      uint16_t width{};         // Width of the image  
      uint16_t height{};        // Height of the image
      uint8_t  BitsPrPixel{};   // number of bytes in the stored pixel index
      uint8_t  ImgDescriptor{}; // Image Descriptor Byte.                                    |
                              // Bits 3-0 - number of attribute bits associated with each  
                              //            pixel.                                         
                              // Bit 4    - reserved.  Must be set to 0.                   
                              // Bit 5    - screen origin bit.                             
                              //            0 = Origin in lower left-hand corner.          
                              //            1 = Origin in upper left-hand corner.          
                              //            Must be 0 for Truevision images.               
                              // Bits 7-6 - Data storage interleaving flag.                
                              //            00 = non-interleaved.                          
                              //            01 = two-way (even/odd) interleaving.          
                              //            10 = four way interleaving.                    
                              //            11 = reserved.                                 
                              // This entire byte should be set to 0.  Don't ask me.       
};
#pragma pack(pop)

struct TGAcolor{
      uint8_t BGRA[4] = {0,0,0,0}; // TGA reads rgb as BGR so backwards
      // might need a bytessprpixel here, such that i can ensure you dont try to write a RGB color in aGrey scale system
      TGAcolor() = default;
      TGAcolor(uint8_t const Grey) : BGRA{Grey,0,0,0} {}
      TGAcolor(uint8_t const R, uint8_t const G, uint8_t const B) : BGRA{B,G,R,0} {}; 
      TGAcolor(uint8_t const R, uint8_t const G, uint8_t const B, uint8_t const A) : BGRA{B,G,R,A} {};
      TGAcolor(uint8_t const *v, uint8_t const bpp) {
            for (int i=0; i < bpp; i++){
                  BGRA[i] = v[i];
            }
      }
      uint8_t & operator[](int const i) { return BGRA[i]; }
};

class TGAimage{
      protected:
            std::vector<uint8_t> m_screenbuffer; // uint8_t is just to ensure that the char is 8 bits/1byte long
            short m_width, m_height;// should this me uint8_t as that is the max for the file?
            short m_bpp; // bytes pr pixel
      public:
            enum Colorformat {// bytes pr format
                  WindowsConsole = 1,
                  Greyscale      = 1,
                  RGB            = 3,
                  RGBA           = 4,
            };
            TGAimage();
            TGAimage(short const width, short const height, uint8_t const bpp);
            void draw(int const x, int const y, TGAcolor const color);
            void clear();
            bool TGAwrite(std::string filename, bool fliped);
            // Getters - Setters
            int getWidth()                              const { return m_width;  }
            int getHeight()                             const { return m_height; }
            int getBytesPrPixel()                       const { return m_bpp;    }
            std::vector<uint8_t> getBuffer()            const { return m_screenbuffer; };
            std::vector<uint8_t> const & getBufferRef() const { return m_screenbuffer; };
            
};

#endif