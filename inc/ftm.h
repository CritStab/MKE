#include "MKE04Z1284.h"
#include "tact.h"


#ifndef FTM_H
#define FTM_H


class Ftm
{
  //variables
public:
	enum class channel : uint8_t {ch0, ch1, ch2, ch3, ch4, ch5};
	enum class nFtm : uint8_t {FTM_0, FTM_1, FTM_2};
	enum class sourceClock : uint8_t {Off, System_clk, Fixed_clk, Ext_clk};
	enum class division : uint8_t {div1, div2, div4, div8, div16, div32, div64, div128};


private:
protected:
	uint8_t num_ch;
	uint8_t num_ftm;
	sourceClock s_clock;
	static FTM_MemMapPtr ftm_ptr[3];
	static IRQn n_interrupt [3];


public:
	Ftm (nFtm n_, division, sourceClock s = sourceClock::System_clk);
	Ftm (nFtm n_, division, uint16_t p, sourceClock s = sourceClock::System_clk);
	void setChannel (channel & ch);
	void setDivision (division div);
	void setPeriod (const uint16_t &);
	void setChannelValue (const uint16_t &);
	void setInitValue (uint16_t );
	void setFilter (uint8_t val);
	void clearTof ();
	void interruptEnable ();
	void interruptDisable ();
	void start ();
	void stop ();
	FTM_MemMapPtr getPtrTimer (){return ftm_ptr[num_ftm];}

private:

};

#endif
