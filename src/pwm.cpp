#include "pwm.h"

Pwm::PtrPwm Pwm::funcMode [3] = {&Pwm::setEdgePwm};


Pwm::Pwm (Ftm &t, Ftm::channel ch, mode mode_, pulseMode m_)
{
	pwmChannel = static_cast <uint8_t> (ch);
	timer = &t;
	ptrTimer = timer->getPtrTimer();
	(this->*(Pwm::funcMode[(uint8_t)mode_]))(m_);
	timer->start();
}

void Pwm::setEdgePwm (pulseMode m)
{
	ptrTimer->SC &= ~FTM_SC_CPWMS_MASK;
	ptrTimer->CONTROLS[pwmChannel].CnSC |= FTM_CnSC_MSB_MASK;
	ptrTimer->CONTROLS[pwmChannel].CnSC &= ~(FTM_CnSC_ELSA_MASK|FTM_CnSC_ELSB_MASK);
	ptrTimer->CONTROLS[pwmChannel].CnSC |= (uint8_t)m << FTM_CnSC_ELSA_SHIFT;
}

void Pwm::setCenterPwm ()
{

}

void Pwm::setCombinePwm ()
{

}

void Pwm::setValue (uint16_t val)
{
	ptrTimer->CONTROLS[pwmChannel].CnV = val;
}

