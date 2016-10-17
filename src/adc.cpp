#include "adc.h"

const uint8_t adc::pin_def [10][2] = {
{E, 20},
{E, 16},
{E, 18},
{E, 22},
{E, 21},
{E, 17},
{E, 19},
{E, 23},
{B, 0},
{D, 1}
};

adc::adc(channel ch_, resolution r_)
{
	n_channel = ch_;
	//tact ADC0
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
	//Set busclock
	ADC0->CFG1 &= ~ADC_CFG1_ADICLK_MASK;
	//Set divider - 8
	ADC0->CFG1 |= ADC_CFG1_ADIV(3);
	ADC0->SC3 |= ADC_SC3_AVGE_MASK|ADC_SC3_AVGS(3);
	ADC0->SC2 &= ~ ADC_SC2_ADTRG_MASK;
	ADC0->SC3 |= ADC_SC3_CAL_MASK ;
	while (!(ADC0->SC1[0]&ADC_SC1_COCO_MASK));
	//Settings pins
	/*pin.setPort (pin_def [n_channel][0]);
	pin.setOutPin (pin_def [n_channel][1], 0);*/
	ADC0->CFG1|= ADC_CFG1_ADLSMP_MASK|ADC_CFG1_MODE(r_);
}

uint16_t adc::convert ()
{
	//Select 4 channal and start conversation
	ADC0->SC1[0] = ADC_SC1_ADCH(n_channel);
	while (!(ADC0->SC1[0]&ADC_SC1_COCO_MASK));
	return ADC0->R[0];
}



