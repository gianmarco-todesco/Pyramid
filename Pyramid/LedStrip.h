#ifndef LedStrip_H
#define LedStrip_H

#include <Adafruit_NeoPixel.h>

class LedStrip {
  public:
    LedStrip();
    void begin(void);
    void show();

    
    void switchOff();
    void switchOn();

    void setSupercar(int r, int g, int b);
    void setMovement(int spd);

    
    
  private:    
    void doFlash(void);
    void doSupercar(void);
    void doMovement(void);

    uint32_t wheel(byte pos);
  
    Adafruit_NeoPixel strip;
    uint32_t mainColor;
    uint32_t ledColors[4];
    int mode, oldTime, spd;
};

#endif

