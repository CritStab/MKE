#include "MKE04Z1284.h"                 // Device header
#include "spi.h"
#include "tact.h"
#include "delay.h"
#include "pit.h"
#include "shift_registr.h"


Tact frq;
Spi spi0 (Spi::spi0, Spi::hardware, Spi::set0);
Shift reg (spi0);
Pit pit1 (Pit::ch1, 1000, Pit::ms);


extern "C" {
	void PIT_CH1_IRQHandler();
}


void PIT_CH1_IRQHandler()
{
	static bool flag;
	pit1.clear_flag();
	if (flag)
	{
		reg.send(0x0F);
		flag = false;
	}
	else
	{
		reg.send(0xF0);
		flag = true;
	}

}

//void send_shift ();

int main ()
{

	pit1.interrupt_enable();
	pit1.start();

	while (1)
	{
		/*reg.send(0x0F);
		delay_ms(100);
		reg.send(0xF0);
		delay_ms(100);*/
	}
}

