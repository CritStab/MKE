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
  enum class lpoclkDivision : uint8_t {div1, div2, div4, div8, div16, div32, div64, div128};
  enum class busclkDivision : uint8_t {div2=0, div4, div8, div16, div32=0, div64, div128, div256, div512, div1024, div2048, div4096};
  enum clkFilter {off, busclkLow, busclkHigh, lpoclk};
  enum class sourceFilter {PTA=0, PTB=2, PTC=4, PTD=6, PTE=8, PTF=10, PTG=12, PTH=14, RESET=16, KBI0_=18, KBI1_=20, NMI_=22, PTI_=32, IRQ_=34, FTM0_=36, FTM_=38, PWT1_=40, I2C0_=42, I2C1_=44};

private:
  static GPIO_Type* GpioBase [3];
  static PORT_Type* PortBase [3];
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
  void setPin (uint8_t pin );
  void clearPin (uint8_t pin);
  void toglePin (uint8_t pin);
  void setPinState (uint8_t pin , bool state);
  bool pinState (uint8_t pin);

  //function for port
  void settingPort (uint8_t val, mode m = Output);
  void setValPort (uint8_t value);
  void clearValPort (uint8_t value);

  void setFltDiv2 (busclkDivision &);
  void setFltDiv1 (busclkDivision &);
  void setFltDiv3 (lpoclkDivision &);
  void setFilter (sourceFilter &, clkFilter &);
};

#endif



