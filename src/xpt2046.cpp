#include "xpt2046.h"


Xpt2046::Xpt2046 (Spi &s)
{
	mod = &s;
	mod->set_ctar(Xpt2046Def::CtarNumber);
	setMode();
}

void Xpt2046::setMode ()
{
	//===settings GPIO===//
	//CS
	mod->set_CS(Xpt2046Def::CsPort, Xpt2046Def::CsPin, Gpio::Alt2,Xpt2046Def::CsNumber);

	//SCK
	mod->set_SCK(Xpt2046Def::SckPort, Xpt2046Def::SckPin, Gpio::Alt2);

	//MISO
	mod->set_MISO (Xpt2046Def::MisoPort, Xpt2046Def::MisoPin, Gpio::Alt2);

	//MOSI
	mod->set_MOSI (Xpt2046Def::MosiPort, Xpt2046Def::MosiPin, Gpio::Alt2);

	//settings SPI
	mod->set_cpha(Spi::first);
	mod->set_cpol(Spi::neg);
	mod->set_baudrate(Spi::div8);
	mod->set_f_size(Spi::bit_8);

	//init
	transmitt (0x80);
	transmitt (0x00);
	transmitt (0x00);
	delay_ms(1000);
}

uint16_t Xpt2046::getData (uint8_t adress)
{
	uint16_t data;
	//send request
	mod->put_data(adress, Xpt2046Def::CsNumber, Xpt2046Def::CtarNumber, Spi::on);
	while (!mod->flag_rfdf());
	mod->clear_flag_rfdf();

	//read High byte
	while (!mod->flag_tfuf());
	mod->clear_flag_tfuf();
	mod->put_data(0, Xpt2046Def::CsNumber, Xpt2046Def::CtarNumber, Spi::on);
	while (!mod->flag_rfdf());
	data = mod->get_data() << 8;
	mod->clear_flag_rfdf();

	//read Low byte
	while (!mod->flag_tfuf());
	mod->clear_flag_tfuf();
	mod->put_data(0, Xpt2046Def::CsNumber, Xpt2046Def::CtarNumber);
	while (!mod->flag_rfdf());
	data |= (0x00FF & mod->get_data());
	mod->clear_flag_rfdf();
	while (!mod->flag_tcf());
	mod->clear_flag_tcf();
	return data;
}

void Xpt2046::getXY ()
{
	x = getData(chx);
	delay_us(100);
	y = getData(chy);
}

void Xpt2046::transmitt (uint8_t val)
{
	mod->put_data(val, Xpt2046Def::CsNumber, Xpt2046Def::CtarNumber);
	while (!mod->flag_tcf());
	mod->clear_flag_tcf();
}






