#include "spi.h"

Spi::Ctar_set Spi::C1;
Spi::Ctar_set Spi::C0;

//Spi::Ctar_set* Spi::set_ctar [2] = {&Spi::C0, &Spi::C1};
Spi::ctarPtr Spi::s_ctar [2] = {&Spi::C0, &Spi::C1};

void Spi::set_cpol (Spi &s, Cpol c)
{
	s.set_cpol(c);
}

void Spi::set_cpha (Spi &s, Cpha c)
{
	s.set_cpha(c);
}

void Spi::set_ctar (uint8_t n)
{
	ctar_N = n;
}

void Spi::update_ctar ()
{

}

void Spi::set_ctar (Spi &s, uint8_t c)
{
	s.set_ctar(c);
}

void Spi::set_baudrate (Spi &s, Division d)
{
	s.set_baudrate(d);
}

void Spi::set_f_size (Spi &s, Fsize f)
{
	s.set_f_size(f);
}



Spi::Spi( Role r)
{
  //Turn on tacting Spi1
  SIM->SCGC6 |= SIM_SCGC6_SPI0_MASK;

  //Settings role and turn off tx and rx fifo
  SPI0->MCR &= ~ (SPI_MCR_MSTR_MASK|SPI_MCR_MDIS_MASK);
  SPI0->MCR |= SPI_MCR_MSTR(r)|SPI_MCR_DIS_TXF(1) |SPI_MCR_DIS_RXF(1);//|SPI_MCR_PCSIS(1<<0);
  //сделать настройку
  SPI0_CTAR(0) |= SPI_CTAR_PCSSCK(1)|SPI_CTAR_PASC(1);
  //Start
  SPI0->SR |= SPI_SR_EOQF_MASK;
  SPI0->MCR &= ~(SPI_MCR_HALT_MASK|SPI_MCR_FRZ_MASK);
}

void Spi::set_cpol (Cpol c)
{
	s_ctar [ctar_N]->cpol = c;
	SPI0_CTAR(ctar_N) &= ~ SPI_CTAR_CPOL_MASK;
	SPI0_CTAR(ctar_N) |= SPI_CTAR_SLAVE_CPOL(c);
}

void Spi::set_cpha (Cpha c)
{
	s_ctar [ctar_N]->cpha = c;
	SPI0_CTAR(ctar_N) &= ~ SPI_CTAR_CPHA_MASK;
	SPI0_CTAR(ctar_N) |= SPI_CTAR_SLAVE_CPHA(c);
}

void Spi::set_f_size (Fsize f)
{
	s_ctar [ctar_N]->f_size = f;
	SPI0_CTAR(ctar_N) &= ~ SPI_CTAR_FMSZ(0x0F);
	SPI0_CTAR(ctar_N) |= SPI_CTAR_FMSZ(f);
}

void Spi::set_baudrate (Division d)
{
	s_ctar [ctar_N]->br = d;
	SPI0_CTAR(ctar_N) &= ~ SPI_CTAR_BR(0x0F);
	SPI0_CTAR(ctar_N) |= SPI_CTAR_BR (d);
}


void Spi::set_CS (Gpio::Port p, const uint8_t & pin, Gpio::mux m, Spi::CS_number n)
{
	Cs.settingPinPort(p);
	Cs.settingPin(pin, m);
	SPI0->MCR |= SPI_MCR_PCSIS(1<<n);
}

void Spi::set_SCK (Gpio::Port p, const uint8_t & pin, Gpio::mux m)
{
	Sck.settingPinPort(p);
	Sck.settingPin(pin, m);

}

void Spi::set_MOSI (Gpio::Port p, const uint8_t & pin, Gpio::mux m)
{
	Miso.settingPinPort(p);
	Miso.settingPin(pin, m);
}

void Spi::set_MISO (Gpio::Port p, const uint8_t & pin, Gpio::mux m)
{
	Mosi.settingPinPort(p);
	Mosi.settingPin(pin, m);
}

void Spi::transmit (uint16_t data)
{

}


uint8_t Spi::receive ()
{

}

uint8_t Spi::exchange (uint8_t data)
{

}

void Spi::put_data (uint16_t data, uint8_t cs, uint8_t ctar, State cont)
{

	SPI0->PUSHR = SPI_PUSHR_PCS(1<<cs)|SPI_PUSHR_TXDATA(data)|SPI_PUSHR_CTAS(ctar)|SPI_PUSHR_CONT(cont);
}

uint16_t Spi::get_data ()
{
	return SPI0->POPR;
}

bool Spi::flag_tcf ()
{
	return SPI0->SR&SPI_SR_TCF_MASK;
}

bool Spi::flag_tfff ()
{
	return SPI0->SR&SPI_SR_TFFF_MASK;
}

bool Spi::flag_tfuf ()
{
	return SPI0->SR&SPI_SR_TFUF_MASK;
}

bool Spi::flag_txctr ()
{
	return SPI0->SR&SPI_SR_TXCTR_MASK;
}

bool Spi::flag_rfof ()
{
	return SPI0->SR&SPI_SR_RFOF_MASK;
}

bool Spi::flag_rfdf ()
{
	return SPI0->SR&SPI_SR_RFDF_MASK;
}

void Spi::clear_flag_tcf()
{
	SPI0->SR |= SPI_SR_TCF_MASK;
}

void Spi::clear_flag_tfuf()
{
	SPI0->SR |= SPI_SR_TFUF_MASK;
}

void Spi::clear_flag_rfof()
{
	SPI0->SR |= SPI_SR_RFOF_MASK;
}

void Spi::clear_flag_rfdf()
{
	SPI0->SR |= SPI_SR_RFDF_MASK;
}
