#include <iostream>
#include "TGAImage.h"
#include <myWindowsConsle.h>

class windowsconsole: public WindowsConsole{

      bool UserInit(){
            return 0;
      }
      bool UserTick(float dt){
            return 0;
      }
      bool UserClean(){
            return 0;
      }


};

int main(int argv, char *argc[]){
      int a;
      int width  = 200;
      int height = 200;
      TGAimage test(width, height, TGAimage::Greyscale);
      TGAcolor color(255);
      for (int i = 25; i < 50; i++)
      {
            for (int j = 10; j < 20; j++){
                  test.draw(i,j,color);
            }
      }
      test.TGAwrite("./test.tga", 0);

      windowsconsole win;
      win.CreateScreenWithBuffer(width,height,1,1);
      win.TGAToBuffer(test);
      //win.Fill(0,0, 0xDB, 0x000F);
      //win.print();
      std::cin.ignore();

      return 0;
}