#include "Speaker.h"
#include "Common.h"

#include <SoftwareSerial.h>

/////////////////////////////////////////
// PIN DEFINITION
/////////////////////////////////////////
#define AUDIO_RX  4        //can be disconnected to TX of the Serial MP3 Player module
#define AUDIO_TX  12        //connect to RX of the module

/////////////////////////////////////////
// AUDIO DEFINITIONS
/////////////////////////////////////////
SoftwareSerial AudioSerial(AUDIO_RX, AUDIO_TX);

#define CMD_PLAY_W_INDEX 0X03
#define CMD_SET_VOLUME 0X06
#define CMD_SEL_DEV 0X09
#define DEV_TF 0X02
#define CMD_PLAY 0X0D
#define CMD_PAUSE 0X0E
#define CMD_SINGLE_CYCLE 0X19
#define SINGLE_CYCLE_ON 0X00
#define SINGLE_CYCLE_OFF 0X01
#define CMD_PLAY_W_VOL 0X22

Speaker::Speaker()
{
}

void Speaker::begin(void)
{
  //***************************************
  //SETUP AUDIO
  //***************************************
  pinMode(AUDIO_TX, OUTPUT);
  pinMode(AUDIO_RX, INPUT);
  AudioSerial.begin(9600);
  confAudio();
}


void Speaker::confAudio()
{
  delay(500);//Wait chip initialization is complete
  sendCommand(CMD_SEL_DEV, DEV_TF);//select the TF card
  delay(200);//wait for 200ms
}

void Speaker::play(unsigned int file, unsigned int vol)
{
   sendCommand(CMD_PLAY_W_VOL, (vol << 8) + file);  
}

void Speaker::sendCommand(int8_t command, int16_t dat)
{
  // delay(20);
  TmpBuffer[0] = 0x7e; //starting byte
  TmpBuffer[1] = 0xff; //version
  TmpBuffer[2] = 0x06; //the number of bytes of the command without starting byte and ending byte
  TmpBuffer[3] = command; //
  TmpBuffer[4] = 0x00;//0x00 = no feedback, 0x01 = feedback
  TmpBuffer[5] = (int8_t)(dat >> 8);//datah
  TmpBuffer[6] = (int8_t)(dat); //datal
  TmpBuffer[7] = 0xef; //ending byte
  for (uint8_t i = 0; i < 8; i++) //
  {
    AudioSerial.write(TmpBuffer[i]) ;
  }
}

