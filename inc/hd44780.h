#include "MK02F12810.h"               // Device header
#include "gpio.h"
#include "delay.h"

#ifndef HD44780_H
#define HD44780_H


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

class Hd44780
{
//variables
public:
	enum Direction {Left = 0, Right= 0x04};
	enum Shifter {Cursor=0, Window=0x08};
	
protected:
private:
	enum commPins {RS=4, E=5, RW=6, D7=3};
	const char shift_data = 0;
	uint8_t position;
	//uint8_t custom_chars [][8];
	Gpio pin;
	unsigned int x_start, x_end, y_start, y_end;

//functions
public:
	Hd44780();
	void init ();
	void tetra (uint8_t t);
	void command (uint8_t com);
	void init_command (uint8_t com);
	void data (char data) ;
	void send_string (const char *str) ;
	void send_string (uint8_t n, const char *str) ;
	void clear ();
	void set_position (uint8_t col, uint8_t row);
	void newChar (const char *ch, uint8_t addr);
	void RS_assert ();
	void RS_disassert ();
	void E_assert ();
	void E_disassert ();
	void RW_assert ();
	void RW_disassert ();
	void check_busy ();
	void Shift (Shifter s, Direction d, uint8_t val);
	void Shift_simple (Shifter s, Direction d, uint8_t val);
	void set_shift_position (uint8_t pos);
	uint8_t get_Shift_position ();
protected:
private:

};






#endif
