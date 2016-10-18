#include "MK02F12810.h"
#include "gpio.h"
#include "ftm.h"

/*
 * FTM1
 * PHA - PTA12 (Alt7)
 * PHB - PTA13 (Alt7)
 * PHA - PTB0 (Alt6)
 * PHB - PTB1 (Alt6)
 *
 * FTM2
 * PHA - PTB18 (Alt6)
 * PHB - PTB19 (Alt6)
 */

namespace QdDef
{
//===Defenitions===//
const Ftm::nFtm N = Ftm::FTM_1;
//PHA
const Gpio::Port PhaPort = Gpio::B;
const Gpio::mux PhaAlt = Gpio::Alt6;
const uint8_t PhaPin = 0;

//PHB
const Gpio::Port PhbPort = Gpio::B;
const Gpio::mux PhbAlt = Gpio::Alt6;
const uint8_t PhbPin = 1;


}




#ifndef QD_H
#define QD_H


class Qd : public Ftm
{
  //variables
public:

private:
	Gpio pha, phb;
	uint16_t high;
	uint16_t value;

public:
	Qd (uint16_t range);
	uint16_t getValue ();
	void setRange (uint16_t r);

private:
	void setMode ();
};



#endif
