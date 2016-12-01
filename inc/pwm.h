#include "MKE04Z1284.h"
#include "ftm.h"


//===Defenitions===//

#ifndef PWM_H
#define PWM_H

class Pwm
{
  //variables
public:

	enum class mode {EdgePwm, CenterPwm, CombinePwm};
	enum class pulseMode {highPulse=2, lowPulse};

private:
	using PtrPwm = void(Pwm::*)(Pwm::pulseMode) ;
	static PtrPwm funcMode [3];
	Ftm * timer;
	uint8_t pwmChannel;
	FTM_MemMapPtr ptrTimer;

public:
	Pwm (Ftm &, Ftm::channel ch, mode, pulseMode);
	void setValue (uint16_t);

private:
	void setMode ();
	void setEdgePwm (pulseMode m);
	void setCenterPwm ();
	void setCombinePwm ();
};

#endif
