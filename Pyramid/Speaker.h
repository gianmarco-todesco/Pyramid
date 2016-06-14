#ifndef Speaker_H
#define Speaker_H

#include <Arduino.h>

class Speaker 
{
  public:
    Speaker();
    void begin(void);

    void play(unsigned int file, unsigned int vol);
    
  private:
    void confAudio();
    void sendCommand(int8_t command, int16_t dat);
};

#endif

