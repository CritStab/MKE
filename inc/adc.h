#include "MKE04Z1284.h"                // Device header
#include "gpio.h"


#ifndef ADC_H
#define ADC_H

class Adc
{
public:
	enum class channel: uint8_t {SE0, SE1, SE2, SE3, SE4, SE5, SE6, SE7, SE8, SE9, SE10, SE11, SE12, SE13, SE14, SE15};
	enum class resolution: uint8_t {bit_8, bit_10, bit_12};
	enum class trigger : uint8_t {rtcOvf, ftm0, ftm2Init, ftm2Match, pit0, pit1, cmp0, cmp1};
	enum class divider : uint8_t {div1, div2, div4, div8};
	enum class clockSource : uint8_t {busClock, busClock2, altclk, adack};

private:
	//static const uint8_t pinDef [10];
	static const Gpio::Port portDef [10];
	Gpio pin_;
	uint8_t n_channel;

public:
	//constructor with soft trigger
	Adc(channel ch_, resolution r_, divider d = divider::div4);

	//constructor with soft trigger
	Adc(channel ch_, resolution r_, trigger t, divider d = divider::div4);
	uint16_t getData ();
	void interrupt (bool);

};

#endif
