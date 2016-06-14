#include "LedStrip.h"
#include "Common.h"
#include "Speaker.h"
#include "Compass.h"
#include "Accel.h"

char TmpBuffer[200];

//-----------------------------------------------------
class GestureParser
{
  public:
    GestureParser()
    {
      j = m = 0;
      last = 0;
      cmd = 0;
    }

    void setPlacement(int p)
    {
      if(p==last) return;
      last = p;
      s[j][0] = p;
      s[j][1] = millis();
      if(m<4)m++;
      Serial.println(p);
      if(m==4)
      {
        Serial.print("   ");
        Serial.print(s[j][0]);
        Serial.print(", ");
        Serial.print(s[(j+3)%4][0]);
        Serial.print(", ");
        Serial.print(s[(j+2)%4][0]);
        Serial.print(", ");
        Serial.println(s[(j+1)%4][0]);
        if(s[j][0]==0 && s[(j+3)%4][0]==1 && s[(j+2)%4][0]==0 && s[(j+1)%4][0]==2)
        {
          int dt = millis()-s[(j+1)%4][1];
          Serial.print("  dt=");
          Serial.println(dt);
          if(dt<4000)
          {
            Serial.println("YUPPPPP");
            cmd = 1;
          }
        }
      }
      j++;
      if(j>=4)j=0;
    }

    int getCommand() 
    {
      int x = cmd; cmd = 0;
      return x;
    }
    
  private:
    int s[4][2];
    int j,m;
    int last;
    int cmd;
};

//-----------------------------------------------------


LedStrip strip = LedStrip();
Speaker speaker = Speaker();
Compass compass = Compass();
Accel accel = Accel();
GestureParser gp = GestureParser();
int state = 0;
unsigned long oldTime;

void setup() {
  Serial.begin(9600);
  strip.begin();
  speaker.begin();
  compass.begin();
  accel.begin();
  oldTime = millis();
  Serial.println("Pyramid started");
  speaker.play(3,0xf);
      
}

void loop() {
  strip.show();
  if(state == 0)
  {
    gp.setPlacement(accel.getPlacement());
    if(gp.getCommand()==1) 
    { 
      Serial.println("activated");
      speaker.play(1,0xF);
      state = 1; 
      oldTime = millis(); 
      strip.switchOn(); 
    }
  }
  else if(state == 1)
  {
    if(millis()-oldTime>5000)
    {
      state = 2;
    }
  }
  else if(state == 2)
  {
    if(millis()-oldTime>30000)
    {
      Serial.println("Fine");
      state = 0;
      strip.switchOff();
    }
    else
    {
      float delta = compass.getDelta(0);
      if(delta>10) strip.setMovement(-1.0f);
      else if(delta<-10) strip.setMovement(1.0f);
      else strip.setSupercar(10,255,70);    
    }
  }
  delay(20);  
}
