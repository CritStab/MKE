#include "tact.h"

uint8_t Tact::cpu_clock;
uint8_t Tact::bus_clock;
uint8_t Tact::pwt_clock;
uint8_t Tact::icsffclk_clock;
uint8_t Tact::icsirclk_clock;

Tact::Tact ()
{

	cpu_clock = 48;
	bus_clock = 24;
	initFEI ();
	SIM->SOPT0 &= ~SIM_SOPT0_NMIE_MASK;
}

void Tact::initFEI ()
{
	/* ICS->C2: BDIV|=1 */
	ICS->C2 |= ICS_C2_BDIV(0x01);         /* Update system prescalers */

	/* SIM->CLKDIV:OUTDIV1=0 (core clock), OUTDIV2=0 (bus clock), OUTDIV3=0 (pwt/ftm clock)*/
	SIM->CLKDIV = SIM_CLKDIV_OUTDIV1(0x00)|SIM_CLKDIV_OUTDIV2_MASK; /* Update system prescalers */
	SIM->CLKDIV |= SIM_CLKDIV_OUTDIV3_MASK;
	/* Switch to FEI Mode */
	/* ICS_C1: CLKS=0,RDIV=0,IREFS=1,IRCLKEN=1,IREFSTEN=0 */
	ICS->C1 = ICS_C1_CLKS(0x00) |
	          ICS_C1_RDIV(0x00) |
	          ICS_C1_IREFS_MASK |
	          ICS_C1_IRCLKEN_MASK;
	/* ICS->C2: BDIV=0,LP=0 */
	  ICS->C2 &= ~( ICS_C2_BDIV_MASK|ICS_C2_LP_MASK);

	  /* OSC->CR: OSCEN=0,??=0,OSCSTEN=0,OSCOS=0,??=0,RANGE=0,HGO=0,OSCINIT=0 */
	  OSC->CR = 0x00U;
	  while((ICS->S & ICS_S_IREFST_MASK) == 0x00U);
	  while((ICS->S & 0x0CU) != 0x00U);
}

void Tact::initFEI (icsDiv & ics, coreDiv & core, busDiv & bus, ftmDiv & ftm)
{
	/* ICS->C2: BDIV|=1 */
	ICS->C2 |= ICS_C2_BDIV(0x01);         /* Update system prescalers */

	/* SIM->CLKDIV:OUTDIV1=0 (core clock), OUTDIV2=0 (bus clock), OUTDIV3=0 (pwt/ftm clock)*/
	SIM->CLKDIV = SIM_CLKDIV_OUTDIV1(core)| (uint8_t)bus << SIM_CLKDIV_OUTDIV2_SHIFT | (uint8_t)ftm << SIM_CLKDIV_OUTDIV3_SHIFT;
	/* Switch to FEI Mode */
	/* ICS_C1: CLKS=0,RDIV=0,IREFS=1,IRCLKEN=1,IREFSTEN=0 */
	ICS->C1 = ICS_C1_CLKS(0x00) |
	          ICS_C1_RDIV(0x00) |
	          ICS_C1_IREFS_MASK |
	          ICS_C1_IRCLKEN_MASK;
	/* ICS->C2: BDIV=0,LP=0 */
	  ICS->C2 &= ~( ICS_C2_BDIV_MASK|ICS_C2_LP_MASK);
	  ICS->C2 |= ICS_C2_BDIV(ics);

	  /* OSC->CR: OSCEN=0,??=0,OSCSTEN=0,OSCOS=0,??=0,RANGE=0,HGO=0,OSCINIT=0 */
	  OSC->CR = 0x00U;
	  while((ICS->S & ICS_S_IREFST_MASK) == 0x00U);
	  while((ICS->S & 0x0CU) != 0x00U);
}



