#include "adc.h"

//const uint8_t Adc::pinDef [10]= {20, 16, 18, 22, 21, 17, 19, 23, 0, 1};

const Gpio::Port Adc::portDef [10] = {
Gpio::Port::E,
Gpio::Port::E,
Gpio::Port::E,
Gpio::Port::E,
Gpio::Port::E,
Gpio::Port::E,
Gpio::Port::E,
Gpio::Port::E,
Gpio::Port::B,
Gpio::Port::D
};
Adc::Adc(channel ch_, resolution r_, divider d)
:pin_ (portDef [(uint8_t)ch_])
{
	n_channel = static_cast<uint8_t> (ch_);
	//tact ADC0
	SIM->SCGC |= SIM_SCGC_ADC_MASK;

	//Set channel
	ADC->APCTL1 |= 1 << n_channel;
	//set clock source
	ADC->SC3 &= ~ADC_SC3_ADICLK_MASK;
	ADC->SC3 |= ADC_SC3_ADICLK(clockSource::busClock2);

	//Set divider
	ADC->SC3 &= ~ADC_SC3_ADIV_MASK;
	ADC->SC3 |= ADC_SC3_ADIV(d);

	//settings resolution
	ADC->SC3 &= ~ ADC_SC3_MODE_MASK;
	ADC->SC3 |= ADC_SC3_MODE(r_);

	//Long sample
	ADC->SC3 |= ADC_SC3_ADLSMP_MASK;

	//soft trigger
	ADC->SC2 &= ~ ADC_SC2_ADTRG_MASK;

	//one conversion
	ADC->SC1 &= ~ADC_SC1_ADCO_MASK;
}

Adc::Adc(channel ch_, resolution r_, buffer b, divider d)
:pin_ (portDef [(uint8_t)ch_])
{
	n_channel = static_cast<uint8_t> (ch_);
	//tact ADC0
	SIM->SCGC |= SIM_SCGC_ADC_MASK;

	//Set channel
	ADC->APCTL1 |= 1 << n_channel;
	//set clock source
	ADC->SC3 &= ~ADC_SC3_ADICLK_MASK;
	ADC->SC3 |= ADC_SC3_ADICLK(clockSource::busClock2);

	//Set divider
	ADC->SC3 &= ~ADC_SC3_ADIV_MASK;
	ADC->SC3 |= ADC_SC3_ADIV(d);

	//settings resolution
	ADC->SC3 &= ~ ADC_SC3_MODE_MASK;
	ADC->SC3 |= ADC_SC3_MODE(r_);

	//Long sample
	ADC->SC3 |= ADC_SC3_ADLSMP_MASK;

	//soft trigger
	ADC->SC2 &= ~ ADC_SC2_ADTRG_MASK;

	//one conversion
	ADC->SC1 &= ~ADC_SC1_ADCO_MASK;

	ADC->SC4 = ADC_SC4_ASCANE_MASK;
	ADC->SC4 |= ADC_SC4_AFDEP(b);
}

Adc::Adc(channel ch_, resolution r_, trigger t, divider d )
{
	n_channel = static_cast<uint8_t> (ch_);

	//tact ADC0
	SIM->SCGC |= SIM_SCGC_ADC_MASK;

	//Set channel
	ADC->APCTL1 |= 1 << n_channel;

	//set clock source
	ADC->SC3 &= ~ADC_SC3_ADICLK_MASK;
	ADC->SC3 |= ADC_SC3_ADICLK(clockSource::busClock2);

	//Set divider
	ADC->SC3 &= ~ADC_SC3_ADIV_MASK;
	ADC->SC3 |= ADC_SC3_ADIV(d);

	//settings resolution
	ADC->SC3 &= ~ ADC_SC3_MODE_MASK;
	ADC->SC3 |= ADC_SC3_MODE(r_);

	//Long sample
	ADC->SC3 |= ADC_SC3_ADLSMP_MASK;

	//hard trigger
	ADC->SC2 |= ADC_SC2_ADTRG_MASK;
	SIM->SOPT0 &= ~SIM_SOPT0_ADHWT_MASK;
	SIM->SOPT0 |= SIM_SOPT0_ADHWT(t);

	//one conversion
	ADC->SC1 &= ~ADC_SC1_ADCO_MASK;
}

uint16_t Adc::getData ()
{
	//Select channal and start conversation
	ADC->SC1 = ADC_SC1_ADCH(n_channel);
	while (!(ADC->SC1&ADC_SC1_COCO_MASK));
	return ADC->R;
}

void Adc::convert ()
{
	ADC->SC1 = ADC_SC1_ADCH(n_channel);
}

bool Adc::getCoco ()
{
	while (!(ADC->SC1&ADC_SC1_COCO_MASK));
}

void Adc::interrupt (bool state)
{
	if (state) n_channel |= ADC_SC1_AIEN_MASK;
	else n_channel &= ~ ADC_SC1_AIEN_MASK;
}


