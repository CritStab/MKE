#include "max6675.h"


Max6675::Max6675 (Spi &s)
{
	mod = &s;
	mod->set_ctar(max6675def::CtarNumber);
	setMode();
}

void Max6675::setMode ()
{
	//===settings GPIO===//
	//CS
	mod->set_CS(max6675def::CsPort, max6675def::CsPin, Gpio::Alt2, max6675def::CsNumber);

	//SCK
	mod->set_SCK(max6675def::SckPort, max6675def::SckPin, Gpio::Alt2);

	//MOSI
	mod->set_MISO (max6675def::MisoPort, max6675def::MisoPin, Gpio::Alt2);

	//settings SPI
	mod->set_cpha(Spi::second);
	mod->set_cpol(Spi::neg);
	mod->set_baudrate(Spi::div8);
	mod->set_f_size(Spi::bit_16);
}

bool Max6675::readCelsius()
{
	mod->put_data(0x0F0F, max6675def::CsNumber, max6675def::CtarNumber);
	while (!mod->flag_rfdf());
	byte_code = mod->get_data();
	while (!mod->flag_tcf());
	mod->clear_flag_tcf();
	mod->clear_flag_rfdf();
	if (byte_code & 0x4)
	  {
    // uh oh, no thermocouple attached!
    return false;
    //return -100;
  }
	byte_code *=10;
	byte_code>>=5;
	return true;
}

uint16_t & Max6675::getTemp ()
{
	return byte_code;
}
