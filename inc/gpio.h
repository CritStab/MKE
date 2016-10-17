#include "MK02F12810.h"                 // Device header

#ifndef GPIO_H
#define GPIO_H



class Gpio
{
//variables
public:
  enum Port :uint8_t {A , B , C , D , E};
  enum mux :uint8_t {Analog, GPIO , Alt2 , Alt3 , Alt4 , Alt5 , Alt6 , Alt7};
  enum mode {Input, Output};
  enum out {PushPull , OpenDrain};
  enum PP {PullDown, PullUp};
  enum state {Off, On};

private:
  static PORT_MemMapPtr PortBase [5];
  static GPIO_MemMapPtr GpioBase [5];
//static unsigned int pAdr [3];	
  unsigned char prt;
//functions
public:
  Gpio ();
  Gpio(Port p );
  Gpio(uint8_t p );
  void settingPinPort (Port p);
  void settingPinPort (uint8_t p);

  //function for pins

  void settingPin (uint8_t pin , mux mx = GPIO, mode m = Output);
  void settingPin (uint8_t pin , uint8_t mx = 1, uint8_t m = 1);
  void settingPinDirection (uint8_t pin , mode m);
  void settingPinDirection (uint8_t pin , uint8_t m);
  void PuPdPin (uint8_t pin, state s, PP p);
  void PuPdPin (uint8_t pin, uint8_t s, uint8_t p);
  void setPin (unsigned int pin );
  void clearPin (unsigned char pin);
  void changePinState (unsigned char pin);
  void setPinState (unsigned char pin , unsigned char state);
  bool pinState (unsigned char pin);

  //function for port
  void settingPort (uint32_t val, mux mx = GPIO, mode m = Output);
  void settingPortDirection (uint32_t val, mode m);
  void setValPort (uint32_t value);
  void clearValPort (uint32_t value);

public:
  class Pin
  {
  public:
	  Pin ();
	  void sPin (unsigned int pin );
  };

};

#endif



