#ifndef H_MYWINDOWSCONSLE_H
#define H_MYWINDOWSCONSLE_H
#include <windows.h>
#include <TGAImage.h>

namespace MyWindowsColor{
   // taken from the onelonecoder and is OLC.h
   enum WinConColor
   {
      FG_BLACK        = 0x0000 ,
      FG_DARK_BLUE    = 0x0001,
      FG_DARK_GREEN   = 0x0002,
      FG_DARK_CYAN    = 0x0003,
      FG_DARK_RED     = 0x0004,
      FG_DARK_MAGENTA = 0x0005,
      FG_DARK_YELLOW  = 0x0006,
      FG_GREY         = 0x0007,
      FG_DARK_GREY    = 0x0008,
      FG_BLUE         = 0x0009,
      FG_GREEN        = 0x000A,
      FG_CYAN         = 0x000B,
      FG_RED          = 0x000C,
      FG_MAGENTA      = 0x000D,
      FG_YELLOW       = 0x000E,
      FG_WHITE        = 0x000F,
      BG_BLACK        = 0x0000,
      BG_DARK_BLUE    = 0x0010,
      BG_DARK_GREEN   = 0x0020,
      BG_DARK_CYAN    = 0x0030,
      BG_DARK_RED     = 0x0040,
      BG_DARK_MAGENTA = 0x0050,
      BG_DARK_YELLOW  = 0x0060,
      BG_GREY         = 0x0070,
      BG_DARK_GREY    = 0x0080,
      BG_BLUE         = 0x0090,
      BG_GREEN        = 0x00A0,
      BG_CYAN         = 0x00B0,
      BG_RED          = 0x00C0,
      BG_MAGENTA      = 0x00D0,
      BG_YELLOW       = 0x00E0,
      BG_WHITE        = 0x00F0,
   };
}



class WindowsConsole{
    protected:
      SMALL_RECT m_rectWindow;
      HANDLE     m_hConsole;
      short      m_ScreenHeight;
      short      m_ScreenWidth;
      int        m_ScreenBufferLength;
      CHAR_INFO *m_Screen = NULL;
      bool       m_WindowActive;
    public:
      WindowsConsole   ();
      WindowsConsole   (short width, short height, short fontwidth, short fontheight, bool WithBuffer = true);
      ~WindowsConsole  ();

      int  TGAToBuffer(TGAimage const &image, short const chr = 0xDB);
      int  CreateScreen(short width, short height, short fontwidth, short fontheight);
      int  CreateScreenWithBuffer(short width, short height, short fontwidth, short fontheight);
      void StartThread ();  
      void Start();
      int  print();
      int  Fill(int const xstart, int const ystart,short const chr = 0x2588, short const color  = 0x000F);
      virtual bool UserInit () = 0;
      virtual bool UserTick (float dt) = 0;
      virtual bool UserClean() = 0;
};



#endif