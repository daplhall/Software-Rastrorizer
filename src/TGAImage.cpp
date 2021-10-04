#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdint>
#include "TGAImage.h"


// TGAimage Class
TGAimage::TGAimage() : m_screenbuffer(), m_width(0), m_height(0), m_bpp(0){}

TGAimage::TGAimage(short const width, short const height, uint8_t const bpp) : m_screenbuffer(width*height*bpp,0), m_width(width), m_height(height),m_bpp(bpp) 
{
}    //header.bitsperpixel = colorbits << 3; // translates the bytes to pits. eg. RGB is 3 bytes, and we then translate it to 24 bytes.
/*
    return : bool
    -- return 1 if an error has occured and 0 if it finished
*/
/*
    Assumes the programmer knows or controls weather we are within the bounds
*/
void TGAimage::draw(int const x, int const y, TGAcolor const color){
    memcpy(&m_screenbuffer[0] + (x + m_width * y)*m_bpp, color.BGRA, m_bpp); // memcpy needs number of bytes to copy.
}


bool TGAimage::TGAwrite(std::string filename, bool fliped){
    std::ofstream fileout;
    TGAHeader header; 
    header.width         = m_width;
    header.height        = m_height;
    header.BitsPrPixel   = m_bpp << 3; // translates the bytes to pits. eg. RGB is 3 bytes, and we then translate it to 24 bytes. eqivilant to mutlipying with 8.
    header.DataTypeCode  = (m_bpp == Greyscale) ? 3 : 2; // TODO add compression ie more '?', values come from table in  http://www.paulbourke.net/dataformats/tga/
    header.ImgDescriptor = fliped ? 0x00 : 0x20; // 0x20 corresponds exactly to a 1 on the 5th bith and 0 elsewhere
    // leave the rest as 0
    // FOOTER
    uint8_t Extension_offset[4] = {0,0,0,0};
    uint8_t Developer_offset[4] = {0,0,0,0};
    uint8_t signature[18]       = {'T','R','U','E','V','I','S','I','O','N','-','X','F','I','L','E','.','\0'};
    /*
        The above information fully (except fileout) is defined for the structure of 
    */
    fileout.open(filename, std::ios::binary);
    if (!fileout.is_open()){
        std::cerr << "Unable to openfile " << filename << "\n";
        fileout.close();
        return true;
    }
    fileout.write(reinterpret_cast<char const *> (&header), sizeof(header)); // reinterping the pointer of header struct as char, as structs and classes are continously allocated like a list
    // We inerpt as char because it is what write needs, and the struct is uint#_t so the same as a char or a array of chars.
    if (!fileout.good()){
        fileout.close();
        std::cerr << "Cant write the TGA header \n";
        return true;
    }
    // Now save data; TODO write data
    fileout.write(reinterpret_cast<char const *> (&m_screenbuffer[0]), m_width*m_height*m_bpp);
    if (!fileout.good()){
        fileout.close();
        std::cerr << "Cant Write image to TGA file\n";
        return true;
    }
    // save footer 
    fileout.write(reinterpret_cast<char const *> (&Extension_offset), sizeof(Extension_offset));
    if (!fileout.good()){
        fileout.close();
        std::cerr << "Cant write TGA Extention offset \n";
        return true;
    }
    fileout.write(reinterpret_cast<char const *> (&Developer_offset), sizeof(Developer_offset));
    if (!fileout.good()){
        fileout.close();
        std::cerr << "Cant write TGA Developer offset\n";
        return true;
    }
    fileout.write(reinterpret_cast<char const *> (&signature), sizeof(signature));
    if (!fileout.good()){
        fileout.close();
        std::cerr << "Cant write TGA Signature\n";
        return true;
    }
    fileout.close();
    return false;
}