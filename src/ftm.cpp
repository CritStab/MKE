#include "ftm.h"

FTM_MemMapPtr Ftm::ftm_ptr[3]={FTM0, FTM1, FTM2};
IRQn Ftm::n_interrupt [3]= {FTM0_IRQn, FTM1_IRQn, FTM2_IRQn};


Ftm::Ftm (nFtm n_, division div, sourceClock s)
{
	num_ftm = static_cast <uint8_t> (n_);
	SIM->SCGC |= 1 << (SIM_SCGC_FTM0_SHIFT+num_ftm);
	//FTM_MODE_REG(ftm_ptr[num_ftm]) |= FTM_MODE_WPDIS_MASK;

	s_clock = s;
	FTM_SC_REG(ftm_ptr[num_ftm]) &= ~(FTM_SC_CLKS_MASK|FTM_SC_PS(0x07));
	FTM_SC_REG(ftm_ptr[num_ftm]) |= FTM_SC_PS(div);
}

Ftm::Ftm (nFtm n_, division div, uint16_t p, sourceClock s)
{
	num_ftm = static_cast <uint8_t> (n_);
	//maped ftm2 ch2 at D0, ch3 at D1
		SIM->PINSEL1 &= ~(SIM_PINSEL1_FTM2PS2_MASK | SIM_PINSEL1_FTM2PS3_MASK);
		SIM->PINSEL1 |= SIM_PINSEL1_FTM2PS3(0x01) | SIM_PINSEL1_FTM2PS2(0x01);
	SIM->SCGC |= 1 << (SIM_SCGC_FTM0_SHIFT+num_ftm);
	//FTM_MODE_REG(ftm_ptr[num_ftm]) |= FTM_MODE_WPDIS_MASK;

	s_clock = s;
	FTM_SC_REG(ftm_ptr[num_ftm]) &= ~(FTM_SC_CLKS_MASK|FTM_SC_PS(0x07));
	FTM_SC_REG(ftm_ptr[num_ftm]) |= FTM_SC_PS(div);
	FTM_MOD_REG(ftm_ptr[num_ftm]) = p;
}

void Ftm::setChannel (channel & ch)
{
	num_ch = static_cast <uint8_t> (ch);
}

void  Ftm::setDivision (division div)
{
	FTM_SC_REG(ftm_ptr[num_ftm]) &= ~FTM_SC_PS_MASK;
	FTM_SC_REG(ftm_ptr[num_ftm]) |= FTM_SC_PS(div);
}

void Ftm::setPeriod (const uint16_t &val)
{
	FTM_MOD_REG(ftm_ptr[num_ftm]) = val;
}

void Ftm::setChannelValue (const uint16_t &val)
{
	FTM_CnV_REG(ftm_ptr[num_ftm], num_ch) = val;
}

void Ftm::setInitValue (uint16_t val)
{
	FTM_CNTIN_REG(ftm_ptr[num_ftm]) = val;

}

void Ftm::setFilter (uint8_t val)
{
	FTM_FILTER_REG (ftm_ptr[num_ftm]) &=~ (0x0F << (num_ch*4));
	FTM_FILTER_REG (ftm_ptr[num_ftm]) |= val << (num_ch*4);
}

void Ftm::interruptEnable ()
{
	FTM_SC_REG(ftm_ptr[num_ftm]) |= FTM_SC_TOIE_MASK;
	NVIC_EnableIRQ (n_interrupt[num_ftm]);
}

void Ftm::interruptDisable ()
{
	FTM_SC_REG(ftm_ptr[num_ftm]) &= ~ FTM_SC_TOIE_MASK;
	NVIC_DisableIRQ (n_interrupt[num_ftm]);
}

void Ftm::start ()
{
	FTM_SC_REG(ftm_ptr[num_ftm]) |= FTM_SC_CLKS(s_clock);
}

void Ftm::stop ()
{
	FTM_SC_REG(ftm_ptr[num_ftm]) &= ~FTM_SC_CLKS_MASK;
}

void Ftm::clearTof ()
{
	FTM_SC_REG(ftm_ptr[num_ftm]) &= ~ FTM_SC_TOF_MASK;
}

