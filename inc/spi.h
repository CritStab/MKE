#include "MKE04Z1284.h"                  // Device header
#include "gpio.h"


/*
 Spi0
 B: CS-5, SCK-2, MOSI-3, MISO-4 // 0
 E: CS-3, SCK-0, MOSI-1, MISO-2 // 1

 Spi1
 D: CS-3, SCK-0, MOSI-1, MISO-2 // 0
 G: CS-7, SCK-4, MOSI-5, MISO-6 // 1
*/

#ifndef SPI_H
#define SPI_H

class Spi
{
//variables
public:

	enum nSpi :uint8_t {spi0, spi1};
	enum Division :uint8_t {div2 , div4 , div8 , div16 , div32 , div64 , div128 , div256, div512};
	enum Role {slave , master};
	enum Cpol : uint8_t {neg, pos};
	enum Cpha : uint8_t {first, second};
	enum Mode : bool {hardware, software};
	enum State : bool {off, on};
	enum pinSet : uint8_t {set0, set1};

private:
	using PtrInitFunc = void (Spi::*)();

	Mode m;
	nSpi n;
	static SPI_Type* spiBase [2];
	Gpio cs;
	uint8_t csPin;
	static PtrInitFunc initFunc [2];


//functions
public:

	Spi (nSpi, Mode , pinSet pinS_,Role r=master);

	void initHardware ();
	void initSoftware ();

	void setCs (Gpio::Port &, uint8_t pin_);
	void assertCs ();
	void disassertCs ();

	Mode & getMode (){return m;}

	void setCpol (Cpol c = neg);
	void setCpha (Cpha c = first);
	void setBaudrate (Division d);

	static void set_cpol (Spi &, Cpol c);
	static void set_cpha (Spi &, Cpha c);
	static void set_ctar (Spi &, uint8_t c);
	static void set_baudrate (Spi &, Division d);

	void start ();
	void stop ();

	void put_data (uint8_t data);
	uint8_t get_data ();

	bool flagSprf ();
	bool flagSpmf ();
	bool flagSptef ();
	bool flagModf ();


  //
private:


};



typedef uint16_t(Spi::*PotMemFn)() ;
typedef uint16_t(Spi::*ptr_ex)(uint16_t) ;

typedef void(Spi::*PotMemF)(uint16_t) ;

#endif

