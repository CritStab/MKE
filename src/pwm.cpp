#include "pwm.h"

Pwm::PtrPwm Pwm::funcMode [3] = {&Pwm::setEdgePwm};


Pwm::Pwm (nFtm n_, channel ch, mode m, pulseMode m_)
:Ftm (n_)
{
	setChannel(ch);
	(this->*(Pwm::funcMode[m]))(m_);
	pin.settingPinPort(PwmDef::PwmPort);
	pin.settingPin(PwmDef::PwmPin, Gpio::Alt4);
}

void Pwm::setEdgePwm (pulseMode m)
{
	FTM_SC_REG(ftm_ptr[num_ftm]) &= ~FTM_SC_CPWMS_MASK;
	FTM_CnSC_REG(ftm_ptr[num_ftm], num_ch) |= FTM_CnSC_MSB_MASK;
	FTM_CnSC_REG(ftm_ptr[num_ftm], num_ch) &= ~(FTM_CnSC_ELSA_MASK|FTM_CnSC_ELSB_MASK);
	FTM_CnSC_REG(ftm_ptr[num_ftm], num_ch) |= m << FTM_CnSC_ELSA_SHIFT;
}

void Pwm::setCenterPwm ()
{

}

void Pwm::setCombinePwm ()
{

}
