#include "spi.h"

SPI_Type * Spi::spiBase [2] = {SPI0, SPI1};
Spi::PtrInitFunc  Spi::initFunc [2] = {&Spi::initHardware, &Spi::initSoftware};

void Spi::set_cpol (Spi &s, Cpol c)
{
	s.setCpol(c);
}

void Spi::set_cpha (Spi &s, Cpha c)
{
	s.setCpha(c);
}



void Spi::set_baudrate (Spi &s, Division d)
{

}

Spi::Spi (nSpi nspi_, Mode mode_, pinSet pinS_, Role role_)
{
	//Turn on tacting Spi
	n = nspi_;
	SIM->SCGC |= 1 << SIM_SCGC_SPI0_SHIFT+nspi_;
	spiBase [n]->C1 &= ~SPI_C1_MSTR_MASK;
	spiBase [n]->C1 |= role_ << SPI_C1_MSTR_SHIFT;
	if (nspi_)
		{
			SIM->PINSEL &=  ~SIM_PINSEL_SPI0PS_MASK;
			SIM->PINSEL |= pinS_ << SIM_PINSEL_SPI0PS_SHIFT;
		}
	else
	{
		SIM->PINSEL1 &=  ~SIM_PINSEL1_SPI1PS_MASK;
		SIM->PINSEL1 =  pinS_ << SIM_PINSEL1_SPI1PS_SHIFT;
	}

	(this->*(Spi::initFunc[mode_]))();
}

void Spi::initHardware ()
{
	spiBase [n]->C1 |= SPI_C1_SSOE_MASK;
	spiBase [n]->C2 |= SPI_C2_MODFEN_MASK;
}

void Spi::initSoftware ()
{
	spiBase [n]->C1 &= ~ SPI_C1_SSOE_MASK;
	spiBase [n]->C2 &= ~ SPI_C2_MODFEN_MASK;

}

void Spi::setCs (Gpio::Port &port_, uint8_t pin_)
{
	csPin = pin_;
	cs.settingPinPort(port_);
	cs.settingPin(pin_);
	cs.setPin(pin_);
}

void Spi::assertCs ()
{
	cs.clearPin (csPin);
}

void Spi::disassertCs ()
{
	cs.setPin (csPin);
}

void Spi::setCpol (Cpol c)
{
	spiBase [n]->C1 &= ~ SPI_C1_CPOL_MASK;
	spiBase [n]->C1 |= c << SPI_C1_CPOL_SHIFT;
}

void Spi::setCpha (Cpha c)
{
	spiBase [n]->C1 &= ~ SPI_C1_CPHA_MASK;
	spiBase [n]->C1 |= c << SPI_C1_CPHA_SHIFT;
}

void Spi::setBaudrate (Division d)
{
	spiBase [n]->BR = SPI_BR_SPR(d);
}

void Spi::start ()
{
	spiBase [n]->C1 |= SPI_C1_SPE_MASK;
}

void Spi::stop ()
{
	spiBase [n]->C1 &= ~ SPI_C1_SPE_MASK;
}

void Spi::put_data (uint8_t data)
{
	spiBase [n]->D = data;
}

uint8_t Spi::get_data ()
{
	return spiBase [n]->D;
}

bool Spi::flagSprf ()
{
	return spiBase [n]->S&SPI_S_SPRF_MASK;
}

bool Spi::flagSpmf ()
{
	return spiBase [n]->S&SPI_S_SPMF_MASK;
}

bool Spi::flagSptef ()
{
	return spiBase [n]->S&SPI_S_SPTEF_MASK;
}

bool Spi::flagModf ()
{
	return spiBase [n]->S&SPI_S_MODF_MASK;
}

