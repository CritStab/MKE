#include "qd.h"




Qd::Qd (uint16_t range)
:Ftm (QdDef::N)//, Ftm::Fixed_clk)
{
	high = range;
	setMode ();
}


void Qd::setMode ()
{
	//===Setings pin===//
	//pha
	pha.settingPinPort(QdDef::PhaPort);
	pha.settingPin(QdDef::PhaPin, QdDef::PhaAlt);
	pha.PuPdPin(QdDef::PhaPin, Gpio::On, Gpio::PullUp);
	//phb
	phb.settingPinPort(QdDef::PhbPort);
	phb.settingPin(QdDef::PhbPin, QdDef::PhbAlt);
	phb.PuPdPin(QdDef::PhbPin, Gpio::On, Gpio::PullUp);

	//===Settings timer===//
	FTM_SC_REG(ftm_ptr[num_ftm]) = 0;
	setPeriod(FTM_MOD_MOD_MASK);
	setInitValue(0);
	FTM_MODE_REG (ftm_ptr[num_ftm]) |= FTM_MODE_WPDIS_MASK;
	//FTM_MODE_REG (ftm_ptr[num_ftm]) |= FTM_MODE_FTMEN_MASK;

	FTM_CnSC_REG(ftm_ptr[num_ftm], 0) = 0;
	FTM_CnSC_REG(ftm_ptr[num_ftm], 1) = 0;
	setDivision(Ftm::div8);
	FTM_QDCTRL_REG(ftm_ptr[num_ftm]) |= FTM_QDCTRL_QUADEN_MASK
			//|FTM_QDCTRL_PHAFLTREN_MASK|FTM_QDCTRL_PHBFLTREN_MASK
			;
	//setFilter(2);
	//FTM_FILTER_REG (ftm_ptr[num_ftm]) |= FTM_FILTER_CH0FVAL(2) | FTM_FILTER_CH1FVAL(2) ;
	//
	start ();
}

uint16_t Qd::getValue ()
{
	/*
	value = FTM_CNT_REG (ftm_ptr[num_ftm]);
	if (value>high)
	{
		value = high;
		FTM_CNT_REG (ftm_ptr[num_ftm]) = high;
	}
	return value;*/
	return  FTM1->CNT;

}

void Qd::setRange (uint16_t r)
{

}
