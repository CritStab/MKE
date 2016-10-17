#include "shift_registr.h"


Shift::Shift(Spi &s)
{
	mod = &s;
	mod->set_ctar(ShiftDef::CtarNumber);
	setMode();
}

void Shift::setMode ()
{
	//===settings GPIO===//
	//CS
	mod->set_CS(ShiftDef::CsPort, ShiftDef::CsPin, Gpio::Alt2, ShiftDef::CsNumber);

	//SCK
	mod->set_SCK(ShiftDef::SckPort, ShiftDef::SckPin, Gpio::Alt2);

	//MOSI
	mod->set_MOSI(ShiftDef::MosiPort, ShiftDef::MosiPin, Gpio::Alt2);

	//settings SPI
	mod->set_cpha(Spi::first);
	mod->set_cpol(Spi::neg);
	mod->set_baudrate(Spi::div8);
	mod->set_f_size(Spi::bit_8);
}

void Shift::send (uint8_t data)
{
	mod->put_data(data, ShiftDef::CsNumber, ShiftDef::CtarNumber);
	while (!mod->flag_tcf());
	mod->clear_flag_tcf();
}




