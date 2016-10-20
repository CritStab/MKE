#include "MKE04Z1284.h"                // Device header

#ifndef FILTER_H
#define FILTER_H



class Filter
{
//variables
public:
  enum class lpoclkDivision : uint8_t {div1, div2, div4, div8, div16, div32, div64, div128};
  enum class busclkDivision : uint8_t {div2=0, div4, div8, div16, div32=0, div64, div128, div256, div512, div1024, div2048, div4096};
  enum class clkFilter {off, busclkLow, busclkHigh, lpoclk};
  enum class sourceFilter {PTA=0, PTB=2, PTC=4, PTD=6, PTE=8, PTF=10, PTG=12, PTH=14, RESET=16, KBI0_=18, KBI1_=20, NMI_=22, PTI_=32, IRQ_=34, FTM0_=36, FTM_=38, PWT1_=40, I2C0_=42, I2C1_=44};

private:

//functions
public:
  Filter ();

  void setFltDiv2 (busclkDivision );
  void setFltDiv1 (busclkDivision );
  void setFltDiv3 (lpoclkDivision );
  void setFilter (sourceFilter , clkFilter );
};

#endif



