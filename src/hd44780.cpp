#include "hd44780.h"


Hd44780::Hd44780()
:pin (Gpio::D)
{
	pin.settingPort ((0x0F<<shift_data)|1 << RS | 1 << E);
	init();
	position=0;
}

void Hd44780::init ()
{
	delay_ms (16);
	tetra (0x03);
	delay_ms (5);
	tetra (0x03);
	delay_us (110);
	tetra (0x03);
	delay_ms (1);
	tetra (0x02);
	delay_ms (1);

	// 2 strings
	command (0x28);
	delay_ms (1);
	//turn on display
	command (0x0C);
	delay_ms (1);
	command (0x06);
	delay_ms (1);
	clear();
}

void Hd44780::tetra (uint8_t t)
{
	E_assert ();
	pin.clearValPort (0x0F<<shift_data);
	t &= 0x0F;
	pin.setValPort(t<<shift_data);
	delay_us (2);
	E_disassert ();
}

void Hd44780::command (uint8_t b)
{
	RW_assert();
	check_busy();
	uint8_t hb = 0;
	hb = b >> 4;
	RS_disassert ();
	tetra (hb);
	delay_us(1);
	tetra (b);
	delay_us(1);
	RW_disassert();
}

void Hd44780::data (char b)
{
	RW_assert();
	check_busy();
	uint8_t hb = 0;
	hb = b >> 4;
	RS_assert ();
	tetra (hb);
	delay_us(1);
	tetra (b);
	delay_us(1);
	RW_disassert();
}

void Hd44780::send_string (const char *str)
{
	while (*str) data (*str++);
}

void Hd44780::send_string (uint8_t n, const char *str)
{
	for (uint8_t i=0;i<n;++i) data (*str++);
}

void Hd44780::clear ()
{
	command (clear_dram);
	delay_us (1500);
}

void Hd44780::set_position (uint8_t col, uint8_t row)
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

void Hd44780::check_busy ()
{
	pin.settingPinDirection(D7, Gpio::Input);
	pin.PuPdPin(D7, Gpio::On, Gpio::PullUp);
	RW_disassert();
	uint8_t state;
	do
	{
		E_assert();
		delay_us(2);
		state = pin.pinState(D7);
		E_disassert();
		delay_us(1);
		E_assert();
		delay_us(2);
		E_disassert();
	}
	while (state);
	pin.PuPdPin(D7, Gpio::Off, Gpio::PullUp);
	pin.settingPinDirection(D7, Gpio::Output);
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
void Hd44780::set_shift_position (uint8_t pos)
{
	if (pos<position) Shift(Hd44780::Window, Hd44780::Right, position - pos);
	if (pos > position) Shift(Hd44780::Window, Hd44780::Left, pos - position);
}

uint8_t Hd44780::get_Shift_position ()
{
	return position;
}

void Hd44780::RS_assert ()
{
	pin.setPin(RS);
}

void Hd44780::RS_disassert ()
{
	pin.clearPin(RS);
}

void Hd44780::E_assert ()
{
	pin.setPin(E);
}

void Hd44780::E_disassert ()
{
	pin.clearPin(E);
}

void Hd44780::RW_assert ()
{
	pin.setPin(RW);
}

void Hd44780::RW_disassert ()
{
	pin.clearPin(RW);
}


