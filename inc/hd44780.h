#include "MKE04Z1284.h"                // Device header
#include "port.h"
#include "pin.h"
#include "delay.h"

#ifndef HD44780_H
#define HD44780_H

#define bit8

namespace hd44780Def
{
  //Port
  const Gpio::Port dataPort = Gpio::Port::D;
  const uint8_t dataPins = 0xFF;
  //D7
  const Gpio::Port d7port = Gpio::Port::D;
  const uint8_t d7pin = 7;
  //E
  const Gpio::Port eport = Gpio::Port::E;
  const uint8_t epin = 1;
  //RS
  const Gpio::Port rsport = Gpio::Port::C;
  const uint8_t rspin = 5;
  //RW
  const Gpio::Port rwport = Gpio::Port::E;
  const uint8_t rwpin = 0;
}

//COMMANDS

const uint8_t clear_dram = 0x01;
const uint8_t clear_counter = 0x02;
//shift command & option
const uint8_t shift = 0x10;


//settings display command

const uint8_t display_settings = 0x08;

const uint8_t turn_off_display = 0x08;
const uint8_t turn_on_display = 0x0C;


const uint8_t turn_off_cursor = 0x0C;
const uint8_t turn_on_blink = 0x0D;
const uint8_t turn_on_cursor = 0x0E;
const uint8_t blink_cursor = 0x0F;

const uint8_t set_dram_addr = 0x80;
const uint8_t second_col = 0x40;

const uint8_t set_cgram_addr = 0x40;

const uint8_t shiftWindowsLeft = 0x18;

//Bus defenition
const uint8_t busPin = 0x0F;

class Hd44780
{
//variables
public:
	enum Direction{Left = 0, Right= 0x04};
	enum Shifter  {Cursor=0, Window=0x08};
	
protected:
private:
	const char shift_data = 0;
	uint8_t position;
	//uint8_t custom_chars [][8];
	Pin rs, e, rw, d7;
	Port data_;
	unsigned int x_start, x_end, y_start, y_end;

//functions
public:
	Hd44780();
	void init ();
	void tetra (uint8_t t);
	void command (uint8_t com);
	void initCommand (uint8_t com);
	void data (char data) ;
	void sendString (const char *str) ;
	void sendString (uint8_t n, const char *str) ;
	void clear ();
	void setPosition (uint8_t col, uint8_t row);
	void newChar (const char *ch, uint8_t addr);
	void checkBusy ();
	void Shift (Shifter s, Direction d, uint8_t val);
	void Shift_simple (Shifter s, Direction d, uint8_t val);
	void setShiftPosition (uint8_t pos);
	uint8_t & getShiftPosition ();
protected:
private:

};






#endif
