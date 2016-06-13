#include "Compass.h"
#include "Common.h"
#include <Wire.h>
#include <Arduino.h>

const int HMC5883_addr = 0x1E; //0011110b, I2C 7bit address of HMC5883
const float c180_pi = 57.2957795131; // 180/pi

Compass::Compass()
{
  oldAzimuth = 0;
}


void Compass::begin()
{
    Wire.beginTransmission(HMC5883_addr); //open communication with HMC5883
    Wire.write((uint8_t)0x02); //select mode register
    Wire.write((uint8_t)0x00); //continuous measurement mode
    Wire.endTransmission();  
}

float Compass::getAzimuth()
{
  Wire.beginTransmission(HMC5883_addr);
  Wire.write((uint8_t)0x03); //select register 3, X MSB register
  Wire.endTransmission();
  Wire.requestFrom(HMC5883_addr, 6);
  if (Wire.available()<6) return 0.0;
  
  int x,y,z; //triple axis data

  x = Wire.read() << 8  | Wire.read(); //X msb ,  lsb
  z = Wire.read() << 8  | Wire.read(); //Z msb ,  lsb
  y = Wire.read() << 8  | Wire.read(); //Y msb ,  lsb

  // (x,y) => azimuth
  float azimuth = atan2((float)y,(float)x)*c180_pi;

  // set azimuth close to oldAzimuth
  if(azimuth>oldAzimuth)
  {
    if(azimuth-oldAzimuth>180.0) azimuth -= 360.0;
  }
  else if(azimuth<oldAzimuth)
  {
    if(oldAzimuth-azimuth>180.0) azimuth += 360.0;
  }

  // filter
  const float t = 0.1;
  azimuth = (1-t)*oldAzimuth + t*azimuth;

  // normalize the angle
  if(azimuth<0)azimuth += 360.0;
  else if(azimuth>=360.0) azimuth -= 360.0;

  oldAzimuth = azimuth;
  
  return azimuth;
}

float Compass::getDelta(float target)
{
  float delta = target - getAzimuth();
  if(delta>180) delta -= 360;
  else if(delta<-180) delta += 360;
  return delta;
}



