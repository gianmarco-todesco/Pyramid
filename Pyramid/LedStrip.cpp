#include "LedStrip.h"
#include "Common.h"

#define PIN 5

LedStrip::LedStrip()
{
  strip = Adafruit_NeoPixel(4, PIN, NEO_GRB + NEO_KHZ800);
  mainColor = strip.Color(50,0,0);
  for(int i=0;i<4;i++) ledColors[i] = 0;
  mode = oldTime = spd = 0;
}

void LedStrip::begin(void)
{
  strip.begin();
  switchOff();
}


   
void LedStrip::switchOff()
{
  if(mode==0) return;
  mode = 0;
  Serial.println("switchOff");
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
    strip.setPixelColor(i, strip.Color(0,0,0));   
  }    
}

void LedStrip::switchOn()
{
  if(mode!=0) return;
  mode = 1;
  oldTime = millis();
}

void LedStrip::setSupercar(int r, int g, int b)
{
  mainColor = strip.Color(r,g,b);
  mode = 2;
}

void LedStrip::setMovement(int s)
{
  spd = s;
  mode = 3;
}
    

void LedStrip::show()
{      
  if(mode==1) doFlash();
  else if(mode==2) doSupercar();
  else if(mode==3) doMovement();
  //Serial.println();
  strip.show();
}

void LedStrip::doFlash(void)
{
  int dt = millis() - oldTime;
  float arg = dt * 0.01;
  if(arg>=2*pi) 
  {
    setSupercar(30,5,0);
    // setMovement(0.3);
  }
  else
  {    
    int v = (int)((1-cos(arg))*0.5*255);
    for(uint16_t i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, strip.Color(v,v,v));
  }
}


void LedStrip::doSupercar(void)
{
  float t = 0.001*(float)millis();
  float x = cos(t*10)*0.5+0.5;
  //Serial.print(t); Serial.print(" "); Serial.print(x); Serial.print("  ");
  int r,g,b;
  r = (uint8_t)(mainColor >> 16);
  g = (uint8_t)(mainColor >> 8);
  b = (uint8_t)(mainColor);
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
    float s = (float)i/(float)(strip.numPixels()-1) - x;
    float v = exp(-3.0*s*s);
    //Serial.print(v); Serial.print("  ");      
    strip.setPixelColor(i, strip.Color((int)(r*v),(int)(g*v),(int)(b*v)));
  }    
}

void LedStrip::doMovement(void)
{
  float t = 0.001*spd*(float)millis();
  t = (t-floor(t))*4.0;
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
    float v = max(0,1-abs(i+0.5-t));
    float r = 0.20 * (1-v) + 0.05 * v;
    float g = 0.05 * (1-v) + 0.80 * v;
    float b = 0.20 * (1-v) + 0.80 * v;
    strip.setPixelColor(i, strip.Color((int)(255*r), (int)(255*g), (int)(255*b)));
  } 
}


uint32_t LedStrip::wheel(byte pos)
{
  pos = 255 - pos;
  if(pos < 85) {
    return strip.Color(255 - pos * 3, 0, pos * 3);
  }
  if(pos < 170) {
    pos -= 85;
    return strip.Color(0, pos * 3, 255 - pos * 3);
  }
  pos -= 170;
  return strip.Color(pos * 3, 255 - pos * 3, 0);
}

