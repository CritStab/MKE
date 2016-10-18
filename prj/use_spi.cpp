#include "MKE04Z1284.h"                 // Device header
#include "spi.h"
#include "tact.h"
#include "delay.h"
#include "pit.h"
#include "shift_registr.h"


Tact frq;
Spi spi0 (Spi::spi0, Spi::hardware, Spi::set0);
Shift reg (spi0);


Pit pit1 (Pit::ch1, 500, Pit::ms);
extern "C" {
	void PIT1_IRQHandler();
}


void PIT1_IRQHandler()
{
	pit1.clear_flag();
}

//void send_shift ();

int main ()
{
	/*pit1.interrupt_enable();
	pit1.start();*/

	while (1)
	{
		reg.send(0x0F);
		delay_ms(1000);
		reg.send(0xF0);
		delay_ms(1000);
	}
}
/*
void send_shift ()
{
	static bool flag;
	if (flag)
	{
		reg.send(0xF0);
		flag = false;
	}
	else
	{
		reg.send(0x0F);
		flag = true;
	}

}*/
