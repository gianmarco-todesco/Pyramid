#ifndef Compass_H
#define Compass_H

class Compass {
  public:
    Compass();
    void begin(void);

    float getAzimuth(void);
    float getDelta(float target);
  private:
    float oldAzimuth;
};

#endif

