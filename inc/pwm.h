#include "MK02F12810.h"
#include "gpio.h"
#include "ftm.h"

namespace PwmDef
{
//===Defenitions===//
//CS
const Gpio::Port PwmPort = Gpio::C;
const uint8_t PwmPin = 1;

}




#ifndef PWM_H
#define PWM_H

class Pwm;


class Pwm : public Ftm
{
  //variables
public:

	enum mode {EdgePwm, CenterPwm, CombinePwm};
	enum pulseMode {highPulse=2, lowPulse};


private:
	using PtrPwm = void(Pwm::*)(Pwm::pulseMode) ;
	Gpio pin;
	static PtrPwm funcMode [3];


public:
	Pwm (nFtm n_, channel ch, mode, pulseMode);


private:
	void setMode ();
	void setEdgePwm (pulseMode m);
	void setCenterPwm ();
	void setCombinePwm ();

};



#endif
