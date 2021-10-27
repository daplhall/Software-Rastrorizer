#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdint>
#include "TGAImage.h"
#include "myLinAlg.h"

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

bool TGAimage::HorizontalFlip(){
    TGAcolor cleft;
    TGAcolor cright;
    for (int y = 0; y < m_height; y++){
        for (int x = 0; x < m_width/2; x++){
            int x_ = m_width - x;
            cleft  = GetPixelColor(x,y);
            cright = GetPixelColor(x_, y);
            draw(x,  y, cright);
            draw(x_, y, cleft);
        }
    }
    return 0;
}

bool TGAimage::VerticalFlip(){
    int halfheight  = m_height >> 1;
    int bppwidth    = m_bpp * m_width;
    uint8_t *buffer = new uint8_t[bppwidth];
    for ( int y = 0; y < halfheight; y++){
        int TopId = y*bppwidth;
        int BotId = (m_height - 1 - y)*bppwidth;
        memcpy(buffer                , &m_screenbuffer[TopId], bppwidth);
        memcpy(&m_screenbuffer[TopId], &m_screenbuffer[BotId], bppwidth);
        memcpy(&m_screenbuffer[BotId], buffer                , bppwidth);
    }
    delete[] buffer;
    return 0;
}

bool TGAimage::UnravleRle(){


}

bool TGAimage::TGAwrite(std::string filename, bool verticalflip, bool horizontalflip){
    std::ofstream fileout;
    TGAHeader header; 
    header.width          = m_width;
    header.height         = m_height;
    header.BitsPrPixel    = m_bpp << 3; // translates the bytes to pits. eg. RGB is 3 bytes, and we then translate it to 24 bytes. eqivilant to mutlipying with 8.
    header.DataTypeCode   = (m_bpp == Greyscale) ? 3 : 2; // TODO add compression ie more '?', values come from table in  http://www.paulbourke.net/dataformats/tga/
    header.ImgDescriptor  = verticalflip   ? 0x00 : 0x20; // 0x20 corresponds exactly to a 1 on the 5th bith and 0 elsewhere
    header.ImgDescriptor += horizontalflip ? 0x00 : 0x10; // 0x10 corresponds exactly to a 1 on the 4th bith and 0 elsewhere
    // leave the rest as 0
    // FOOTERj
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

bool TGAimage::TGAread(std::string filename){
    TGAHeader header;
    std::ifstream reader;
    reader.open(filename, std::ios::binary);
    if (!reader.is_open()){
        reader.close();
        std::cerr << "Error: -> TGA reader cant open file" << std::endl;
    }
    reader.read((char *)&header, sizeof(header));
    if (!reader.good()){
        reader.close();
        std::cerr << "Error: -> TGA reader cant read file" << std::endl;
        return 1;
    }
    m_width  = header.width;
    m_height = header.height;
    m_bpp    = header.BitsPrPixel >> 3;
    if (0 >= m_width  || 0 >= m_height || ( m_bpp != Greyscale && m_bpp != RGB && m_bpp != RGBA)){
        reader.close();
        std::cerr << "Error: -> TGA reader cant read file due to unsupported colorscale or no data" << std::endl;
        return 1;
    }
    m_screenbuffer.reserve(m_width*m_height*m_bpp);
    if (header.DataTypeCode == 3 || header.DataTypeCode == 2){
        reader.read((char*) &m_screenbuffer[0], m_width*m_height*m_bpp);
        if (!reader.good()){
            reader.close();
            std::cerr << "Error: -> TGA reader problem with loading data" << std::endl;
            return 1;
        }    
    }
    else{
        reader.close();
        std::cerr << "File compression format is unsupported" << std::endl;
        return 1;
    }
    if (header.ImgDescriptor & 0x20){ // 0x20 is hex for 1 on the 5th but
        VerticalFlip();
    }
    if (header.ImgDescriptor & 0x10){ // 0x10 is hex for 1 on the 4th bit
        HorizontalFlip();
    }
    // i may need flips, as the data might be flipped.
    reader.close();
    return 0;
}
