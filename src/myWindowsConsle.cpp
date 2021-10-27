#include "myWindowsConsle.h"
#include <TGAImage.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

WindowsConsole::WindowsConsole(){}
WindowsConsole::WindowsConsole(short width, short height, short fontwidth, short fontheight, bool WithBuffer){
    if (!WithBuffer){
        CreateScreen(width, height, fontwidth, fontheight);
    }
    else{
        CreateScreenWithBuffer(width, height, fontwidth, fontheight);
    }
}
WindowsConsole::~WindowsConsole(){
    if (!m_Screen) {
        delete[] m_Screen;
    }
}
/*
    Assumes greyscale
    Windows have predefined colors that i accept and seems to just approximate those
    They functions like a Greyscale, although it is 1 "short" ie 2 bytes rather than byte that TGA uses
    pr color range.
*/
int WindowsConsole::TGAToBuffer(TGAimage const &image, short const chr){
   int TGAwidth                         = image.getWidth();
   int TGAHeight                        = image.getHeight();
   int TGABufferLength                  = TGAwidth*TGAHeight;
   std::vector<uint8_t> const TGABuffer = image.getBufferRef(); // Change to const reference?
   for (int pixel = 0; pixel < TGABufferLength; pixel++){
      m_Screen[pixel].Char.UnicodeChar = chr;
      m_Screen[pixel].Attributes       = static_cast<short>(TGABuffer[pixel]); // NOTE WE FLIP THE IMAGE HERE:
      //m_Screen[pixel].Attributes       = static_cast<short>(TGABuffer[(TGABufferLength - 1) - pixel]); // NOTE WE FLIP THE IMAGE HERE:
   }
   print();
   return 0;
}

int WindowsConsole::Fill(int const xstart, int const ystart,short const chr, short const color){
    for (int pixel = 0; pixel < m_ScreenBufferLength; pixel++){
        m_Screen[pixel].Char.UnicodeChar = chr;
        m_Screen[pixel].Attributes       = color;
    }
    return 0;
}

int WindowsConsole::print(){
    wchar_t title[256];
    swprintf(title, 256, L"Test Window" ); // Frame over second
    SetConsoleTitleW(title);
    WriteConsoleOutput(m_hConsole, m_Screen, { m_ScreenWidth, m_ScreenHeight }, { 0,0 }, &m_rectWindow);
    return 0;
}

void WindowsConsole::Start(){
    m_WindowActive= true;
    std::thread ConsoleThread  = std::thread(&WindowsConsole::StartThread, this);
    // Why actually have a thread of execution here?
    ConsoleThread.join();
}

void WindowsConsole::StartThread(){
    //User Initilize
    if (!UserInit()) {m_WindowActive = false;}

    auto timestamp_prev = std::chrono::system_clock::now();
    auto timestamp_curr = std::chrono::system_clock::now();
    wchar_t title[256];
    // add keys later.
    while(m_WindowActive){
        timestamp_curr = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsedtime = timestamp_curr - timestamp_prev;
        timestamp_prev = timestamp_curr;
        float dt = elapsedtime.count();
        if (!UserTick(dt))
        {
            m_WindowActive = false;
            break;
        }
        swprintf(title, 256, L"wCanva Graphics - FPS: %3.2f -- dt: %.3f s", 1.0f/dt, dt ); // Frame over second
        SetConsoleTitleW(title);
        WriteConsoleOutput(m_hConsole, m_Screen, { m_ScreenWidth, m_ScreenHeight }, { 0,0 }, &m_rectWindow);
        /*
        WriteConsoleOutput is the limit on fps it seems, with out it we reach  ~20K frames
        Else we hower around 160 ish frames. This is then the benchmark.
        It seems that the more colors and such there is the less frames.
        Fill with all white boxes are 1000 frames
        */
    }

    if (!UserClean()) {}

}
// TODO ADD FONT SIZE
int WindowsConsole::CreateScreen(short width, short height, short fontwidth, short fontheight){
    m_WindowActive       = true;
    m_ScreenWidth        = width;
    m_ScreenHeight       = height;
    m_ScreenBufferLength = width*height;
    COORD coord          = {width, height};
    m_rectWindow         = {0, 0, 1, 1};
    m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleWindowInfo(m_hConsole, true, &m_rectWindow);
    if (!SetConsoleScreenBufferSize(m_hConsole, coord)){
        std::cerr << "Error -> WindowsConsole - create screen : Buffer size error\n";
        return 1;
    }
    if (!SetConsoleActiveScreenBuffer(m_hConsole)){
        std::cerr << "Error -> WindowsConsole - create screen : Error Active Buffer\n";
        return 1;
    }
 // Font Controll
    CONSOLE_FONT_INFOEX cfi;    
    cfi.cbSize = sizeof(cfi);
    cfi.nFont  = 0;
    cfi.dwFontSize.X = fontwidth;
    cfi.dwFontSize.Y = fontheight;
    cfi.FontFamily   = FF_DONTCARE;
    cfi.FontWeight   = FW_NORMAL;
    //wcscpy(cfi.FaceName, L"Raster Fonts");
    wcscpy(cfi.FaceName, L"Consolas");
    if (!SetCurrentConsoleFontEx(m_hConsole, false, &cfi)){
        std::cerr << "Error -> WindowsConsole - Setting Font size";
        return 1;
    }
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(m_hConsole, &csbi)){
        std::cerr << "Error -> Windows Console - Cant get buffer info";
        return 1;
    }
    if (m_ScreenHeight > csbi.dwMaximumWindowSize.Y){
        std::cerr << "Error -> Windows Console - Screen/Font height to large!";
        return 1;
    }
    if (m_ScreenWidth > csbi.dwMaximumWindowSize.X){
        std::cerr << "Error -> Windows Console - Screen/Font width to large!";
        return 1;
    }
////
    m_rectWindow = {0, 0, m_ScreenWidth - 1, m_ScreenHeight - 1};
    SetConsoleWindowInfo(m_hConsole, true, &m_rectWindow);
    return 0;
}

int WindowsConsole::CreateScreenWithBuffer(short width, short height, short fontwidth, short fontheight){
    m_WindowActive       = true;
    m_ScreenWidth        = width;
    m_ScreenHeight       = height;
    m_ScreenBufferLength = width*height;
    COORD coord          = {width, height};
    m_rectWindow         = {0, 0, 1, 1};
    if (m_Screen) {
        delete[] m_Screen;
    }
    m_Screen   = new CHAR_INFO[m_ScreenBufferLength];
    m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleWindowInfo(m_hConsole, true, &m_rectWindow);
    if (!SetConsoleScreenBufferSize(m_hConsole, coord)){
        std::cerr << "Error -> WindowsConsole - create screen : Buffer size error\n";
        return 1;
    }
    if (!SetConsoleActiveScreenBuffer(m_hConsole)){
        std::cerr << "Error -> WindowsConsole - create screen : Error Active Buffer\n";
        return 1;
    }
// Font Controll
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont  = 0;
    cfi.dwFontSize.X = fontwidth;
    cfi.dwFontSize.Y = fontheight;
    cfi.FontFamily   = FF_DONTCARE;
    cfi.FontWeight   = FW_NORMAL;
    //wcscpy(cfi.FaceName, L"Raster Fonts");
    wcscpy(cfi.FaceName, L"Consolas");

    if (!SetCurrentConsoleFontEx(m_hConsole, false, &cfi)){
        std::cerr << "Error -> WindowsConsole - Setting Font size";
        return 1;
    }
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(m_hConsole, &csbi)){
        std::cerr << "Error -> Windows Console - Cant get buffer info";
        return 1;
    }
    if (m_ScreenHeight > csbi.dwMaximumWindowSize.Y){
        std::cerr << "Error -> Windows Console - Screen/Font height to large!";
        return 1;
    }
    if (m_ScreenWidth > csbi.dwMaximumWindowSize.X){
        std::cerr << "Error -> Windows Console - Screen/Font width to large!";
        return 1;
    }
////
    m_rectWindow = {0, 0, m_ScreenWidth - 1, m_ScreenHeight - 1};
    SetConsoleWindowInfo(m_hConsole, true, &m_rectWindow);
    return 0;
}