#include "MKE04Z1284.h"                // Device header

#ifndef GPIO_H
#define GPIO_H



class Gpio
{
//variables
public:
  enum class Port :uint8_t {A, B, C, D, E, F, G, H, I};
  enum class mode :uint8_t {Input, Output};
  enum class out :uint8_t {PushPull , OpenDrain};
  enum class PP :uint8_t {PullDown, PullUp};
  enum class state : bool {Off, On};


private:
  static GPIO_Type* GpioBase [3];
  static PORT_Type* PortBase [3];
  static uint8_t arrPort [9];
  static uint8_t arrPinShift [9];
  uint8_t prt;

//functions
public:
  Gpio ();
  Gpio(Port p );
  void settingPinPort (Port p);

  //function for pins

  void settingPin (uint8_t pin , mode m = mode::Output);

  void PuPdPin (uint8_t pin, state s, PP p);
  void setPin (uint8_t pin );
  void clearPin (uint8_t pin);
  void toglePin (uint8_t pin);
  void setPinState (uint8_t pin , bool state);
  bool pinState (uint8_t pin);

  //function for port
  void settingPort (uint8_t val, mode m = mode::Output);
  void setValPort (uint8_t value);
  void clearValPort (uint8_t value);
};

#endif



