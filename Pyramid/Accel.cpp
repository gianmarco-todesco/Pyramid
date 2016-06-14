#include "Accel.h"
#include <Wire.h>
#include <Arduino.h>

// REQUIRED IF MPU6050 ACCEL/GYRO IS USED
const int MPU_addr = 0x68; // I2C address of the MPU-6050


// REQUIRED ONLY IF ADXL345 ACCEL IS USED
#ifdef ADXL345
#include <ADXL345.h>
ADXL345 adxl; //variable adxl is an instance of the ADXL345 library
#endif

Accel::Accel()
{
  a0=a1=a2=0.0;
  oldStatus=0;
  curStatus=0;
  curStatusStartTime = 0;
  
}

void Accel::begin(void)
{
  setupAccelMPU();  // MPU 6050 accelerometer&gyro if present
#ifdef ADXL345
  adxl.powerOn();   // adxl345 accelerometer if present
#endif
}

void Accel::setupAccelMPU(void)
{
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
}

int Accel::getPlacement(void)
{
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers

  int16_t GyX, GyY, GyZ;
  int16_t iv0 = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  int16_t iv1 = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  int16_t iv2 = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  int Tmp = Wire.read() << 8 | Wire.read(); //  IGNORED 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)

  const float t = 0.1;
  a0 = (1-t)*a0 + (float)iv0*t;
  a1 = (1-t)*a1 + (float)iv1*t;
  a2 = (1-t)*a2 + (float)iv2*t;

  int status;
  if(a0>8200) status = 0;
  else if(a1<-3000) status = 1;
  else if(a1>3000) status = 2;
  else if(a2<-4000) status = 3;
  else if(a2>4000) status = 4;
  else status = 5;

  if(status != 5)
  {
    if(status == oldStatus) {curStatus=oldStatus; curStatusStartTime=0; }
    else 
    {
      if(status != curStatus) { curStatus = status; curStatusStartTime = millis(); }
      else if(millis()-curStatusStartTime>100) 
      {
        oldStatus=curStatus=status;
        // Serial.print("status= "); Serial.println(status); 
      }
    }
  }
  return oldStatus;
}

