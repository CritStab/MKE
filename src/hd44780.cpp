#include "hd44780.h"


Hd44780::Hd44780()
:data_ (hd44780Def::dataPort, hd44780Def::dataPins),
 d7 (hd44780Def::d7port, hd44780Def::d7pin, Gpio::out::PushPull),
 rs (hd44780Def::rsport, hd44780Def::rspin, Gpio::out::PushPull),
 e (hd44780Def::eport, hd44780Def::epin, Gpio::out::PushPull),
 rw (hd44780Def::rwport, hd44780Def::rwpin, Gpio::out::PushPull)
{
	init();
	position=0;
}

void Hd44780::init ()
{
	//8bit
	delay_ms (16);
	tetra (0x30);
	delay_ms (5);
	tetra (0x30);
	delay_us (110);
	tetra (0x30);
	delay_ms (1);
	tetra (0x38);
	delay_ms (1);
	tetra (0x0C);
	delay_ms (1);
	tetra (0x06);
	delay_ms (1);
	clear();
}

void Hd44780::tetra (uint8_t t)
{
	e.set();
	data_.clear(0xFF);
	data_.set(t);
	delay_us (2);
	e.clear();
}

void Hd44780::command (uint8_t b)
{
	checkBusy ();
	rs.clear();
	tetra (b);
}

void Hd44780::initCommand (uint8_t com)
{
	rw.clear();
	rs.clear();
	tetra (com);
	delay_us(50);
}

void Hd44780::data (char b)
{
	checkBusy();
	rs.set();
	tetra (b);
}

void Hd44780::sendString (const char *str)
{
	while (*str) data (*str++);
}

void Hd44780::sendString (uint8_t n, const char *str)
{
	for (uint8_t i=0;i<n;++i) data (*str++);
}

void Hd44780::clear ()
{
	command (clear_dram);
	delay_us (1500);
}

void Hd44780::setPosition (uint8_t col, uint8_t row)
{
	uint8_t addr = second_col*col + row;
	command (addr|set_dram_addr);
}



void Hd44780::newChar (const char *ch, uint8_t addr)
{
	command ((set_cgram_addr+addr*8));
	for (uint8_t i=0;i<8;++i, ch++) data (*ch);
	//send_byte (set_dram_addr, command);
}

void Hd44780::checkBusy ()
{
	d7.setIn(Gpio::PP::PullUp);
	rw.set();
	rs.clear();
	uint8_t state;
	do
	{
		e.set();
		delay_us(2);
		state = d7.state();
		e.clear();
	}
	while (state);
	d7.setOut(Gpio::out::PushPull);
}

void Hd44780::Shift(Shifter s, Direction d, uint8_t val)
{
	command(turn_off_display);
	uint8_t shift_= shift|s|d;
	for (uint8_t i=0;i<val;++i)
	{
		command(shift_);
	}
	command(turn_on_display);
	if (d == Left)
	{
		uint8_t temp = 40 - position;
		if (s > (temp))position = s - temp;
		else position += s;
	}
	else
	{

		if (s > position)
		{
			uint8_t temp = s - position;
			position = 40 - temp;
		}
		else position -= s;
	}

}
void Hd44780::setShiftPosition (uint8_t pos)
{
	command (clear_counter);
	for (uint8_t i=0; i< pos;++i)command (shift|Left|Window);
	position = pos;
}

uint8_t & Hd44780::getShiftPosition ()
{
	return position;
}
