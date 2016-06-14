#ifndef Accel_H
#define Accel_H

class Accel 
{
  public:
    Accel();
    void begin(void);

    int getPlacement(void);
    
  private:
    void setupAccelMPU(void);
    float a0,a1,a2;
    int oldStatus;
    int curStatus;
    int curStatusStartTime;
};


#endif


