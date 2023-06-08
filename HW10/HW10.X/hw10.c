#include "nu32dip.h"
#include "ws2812b.h"

#define LEDNUM 8

int main(void) {
  NU32DIP_Startup();
  ws2812b_setup();

  wsColor pixels[LEDNUM];
  volatile int hues[8];
  
  for (int i = 0; i < 8; i++)
  {
    hues[i] = i*45;
  }
  
  while(1){
        for (int i = 0; i < 8; i++){
          pixels[i] = HSBtoRGB(hues[i],1,1);
          hues[i] = hues[i] + 1;
          if (hues[i] > 360){
              hues[i] = 0;
          }
        }

        ws2812b_setColor(pixels, LEDNUM);
        int delay = _CP0_GET_COUNT();
        while(_CP0_GET_COUNT()< delay + 24000000/1000){}
        
  }
}