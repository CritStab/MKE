#include "MKE04Z1284.h"                // Device header

#ifndef TACT_H
#define TACT_H



class Tact
{
  //variables
public:
  enum mode {FEI,FEE,FBI,FBE,FBELP,FBILP};
  enum class coreDiv :uint8_t {div1, div2, div3, div4};
  enum class busDiv :uint8_t {div1, div2};
  enum class ftmDiv :uint8_t {div1, div2};
  enum class icsDiv :uint8_t {div1, div2};

 private:
  static uint8_t cpu_clock;
  static uint8_t bus_clock;
  static uint8_t pwt_clock;
  static uint8_t icsffclk_clock;
  static uint8_t icsirclk_clock;
  uint8_t src;
  //functions
public:
  Tact ();
  Tact (mode m, uint8_t frq);
  static uint8_t & get_frq_cpu (){return cpu_clock;};
  static uint8_t & get_frq_bus (){return bus_clock;};
  static uint8_t & get_frq_pwt (){return pwt_clock;};
  static uint8_t & get_frq_icsffclk (){return icsffclk_clock;}
  static uint8_t & get_frq_icsirclk (){return icsirclk_clock;}
private:
  //===int_OSC===//
  //FLL engaged
  void initFEI ();
  void initFEI (icsDiv &, coreDiv &, busDiv &, ftmDiv &);

};

 
#endif

