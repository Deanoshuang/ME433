#include "nu32dip.h"
#include "ws2812b.h"

#define LEDNUM 8
#define HUE_DIFF 45
#define SATURATION 1
#define BRIGHTNESS 1
#define MAX_HUE 360
#define DELAY_MS 1

void delay(int delay_ms);
void update_hue(int *hues);

int main(void) {
  NU32DIP_Startup();
  ws2812b_setup();

  wsColor pixels[LEDNUM];
  int hues[LEDNUM];
  
  for (int i = 0; i < LEDNUM; i++) {
    hues[i] = i * HUE_DIFF;
  }
  
  while(1) {
    for (int i = 0; i < LEDNUM; i++) {
      pixels[i] = HSBtoRGB(hues[i], SATURATION, BRIGHTNESS);
    }
    
    update_hue(hues);
    ws2812b_setColor(pixels, LEDNUM);
    
    delay(DELAY_MS);
  }
}

void delay(int delay_ms) {
  unsigned int delay_count = _CP0_GET_COUNT() + delay_ms * (24000000 / 1000);
  while(_CP0_GET_COUNT() < delay_count) {}
}

void update_hue(int *hues) {
  for (int i = 0; i < LEDNUM; i++) {
    hues[i] = (hues[i] + 1) % MAX_HUE;
  }
}