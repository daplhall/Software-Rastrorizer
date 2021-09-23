#ifndef H_MYWINDOWSCONSLE_H
#define H_MYWINDOWSCONSLE_H
#include <windows.h>
#include <TGAImage.h>
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