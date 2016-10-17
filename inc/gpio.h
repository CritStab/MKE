#include "MKE04Z1284.h"                // Device header

#ifndef GPIO_H
#define GPIO_H



class Gpio
{
//variables
public:
  enum Port :uint8_t {A, B, C, D, E, F, G, H, I};
  enum mode {Input, Output};
  enum out {PushPull , OpenDrain};
  enum PP {PullDown, PullUp};
  enum state {Off, On};

private:
  static GPIO_Type* GpioBase [3];
  static uint8_t arrPort [9];
  static uint8_t arrPinShift [9];
  Port prt;
//functions
public:
  Gpio ();
  Gpio(Port p );
  void settingPinPort (Port p);

  //function for pins

  void settingPin (uint8_t pin , mode m = Output);

  void PuPdPin (uint8_t pin, state s, PP p);
  void PuPdPin (uint8_t pin, uint8_t s, uint8_t p);
  void setPin (uint8_t pin );
  void clearPin (uint8_t pin);
  void changePinState (uint8_t pin);
  void setPinState (uint8_t pin , bool state);
  bool pinState (uint8_t pin);

  //function for port
  void settingPort (uint32_t val, mode m = Output);
  void setValPort (uint32_t value);
  void clearValPort (uint32_t value);
};

#endif



