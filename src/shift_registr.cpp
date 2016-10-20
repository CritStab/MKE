#include "shift_registr.h"

Shift::ptrMode Shift::modeFunc [2] = {&Shift::setModeHw, &Shift::setModeSw};
Shift::ptrSend Shift::sendFunc [2] = {&Shift::sendHw, &Shift::sendSw};

Shift::Shift(Spi &s)
{
	mod = &s;
	mode_ = mod->getMode();
	(this->*(Shift::modeFunc[mode_]))();
	mod->start();
}

void Shift::setModeHw ()
{
	//settings SPI
	mod->setCpha(Spi::Cpha::first);
	mod->setCpol(Spi::Cpol::neg);
	mod->setBaudrate(Spi::Division::div8);
}

void Shift::setModeSw ()
{
	CS.settingPinPort(ShiftDef::CsPort);
	CS.settingPin(ShiftDef::CsPin);
	CS.setPin(ShiftDef::CsPin);
	//settings SPI
	mod->setCpha(Spi::Cpha::first);
	mod->setCpol(Spi::Cpol::neg);
	mod->setBaudrate(Spi::Division::div8);
}

void Shift::send (uint8_t data)
{
	(this->*(Shift::sendFunc[mode_]))(data);
}

void Shift::sendHw (uint8_t data)
{
	while (!mod->flagSptef());
	mod->put_data(data);
}

void Shift::sendSw (uint8_t data)
{
	CS.clearPin(ShiftDef::CsPin);
	while (!mod->flagSptef());
	mod->put_data(data);
	CS.setPin(ShiftDef::CsPin);
}






