#include "delay.h"
/*
extern "C"
{
  void SysTick_Handler ();
}

uint16_t delay;

void SysTick_Handler (void)
{
	if (delay > 0)delay--;
}

void delay_ms (uint16_t del)
{
	SysTick->LOAD = Tact::get_frq_cpu()*1000*del;
	SysTick->VAL = 0xFF;
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk|SysTick_CTRL_CLKSOURCE_Msk;
	while (!(SysTick->CTRL&SysTick_CTRL_COUNTFLAG_Msk));
	SysTick->CTRL &= ~ SysTick_CTRL_COUNTFLAG_Msk;
}*/

void delay_ms (uint16_t del)
{
	Pit ms (Pit::ch0, del, Pit::ms);
	ms.start();
	while (!(ms.flag_TIF()));
	ms.stop();
	ms.clear_flag();
}
void delay_us (uint16_t del)
{
	Pit us (Pit::ch0, del, Pit::us);
	us.start();
	while (!(us.flag_TIF()));
	us.stop();
	us.clear_flag();
}
/*
void delay_us (uint16_t del)
{
	SysTick->LOAD = Tact::get_frq_cpu()*del;
	SysTick->VAL = 0xFF;
	SysTick->CTRL = SysTick_CTRL_ENABLE_Msk|SysTick_CTRL_CLKSOURCE_Msk;
	while (!(SysTick->CTRL&SysTick_CTRL_COUNTFLAG_Msk));
	SysTick->CTRL &= ~ SysTick_CTRL_COUNTFLAG_Msk;
}*/
