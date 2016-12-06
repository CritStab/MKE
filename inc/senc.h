#include "MKE04Z1284.h"
#include "pin.h"




#ifndef SENC_H
#define SENC_H


class Senc 
{
  //variables
public:

private:
  Pin pha, phb;
  uint16_t high;
  uint16_t low;
  uint16_t value;
  uint8_t previusState;

public:
  Senc (Gpio::Port, uint8_t , Gpio::Port, uint8_t, uint16_t range);
  uint16_t getValue ();
  void setValue  (uint16_t);
  void scan ();
  void setHigh (uint16_t);
  void setLow (uint16_t);

};



#endif
