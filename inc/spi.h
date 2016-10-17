#include "MK02F12810.h"                  // Device header
#include "gpio.h"


/*
 Spi0
 Alt2
 E: CS0-16, SCK-17, MOSI-18,MISO-19
 C: CS4-0, CS3-1, CS2-2, CS1-3, CS0-4
 SCK-5, MOSI-6,MISO-7
 D: CS0-0, SCK-1, MOSI-2,MISO-3
 CS1-4, CS2-5, CS3-6
*/

#ifndef SPI_H
#define SPI_H


//typedef (Ctar_set*) Spi::setptr;



class Spi
{


//variables
public:
  enum Division :uint8_t {div2 , div4 , div8 , div16 , div32 , div64 , div128 , div256, div512};
  enum Role {slave , master};
  enum Cpol : uint8_t {neg, pos};
  enum Cpha : uint8_t {first, second};
  enum CS_number : uint8_t {CS0, CS1, CS2, CS3, CS4};
  enum CTAR_number : uint8_t {CTAR0, CTAR1};
  enum State : bool {off, on};
  enum Fsize {bit_4=3, bit_5, bit_6, bit_7, bit_8, bit_9, bit_10, bit_11, bit_12, bit_13, bit_14, bit_15, bit_16};

private:

static struct Ctar_set
  {
	  uint8_t cpol;
	  uint8_t cpha;
	  uint8_t f_size;
	  uint8_t lsbfe;
	  uint8_t dbr;
	  uint8_t pcssck;
	  uint8_t pask;
	  uint8_t pdt;
	  uint8_t pbr;
	  uint8_t cssck;
	  uint8_t asc;
	  uint8_t dt;
	  uint8_t br;
  }C0, C1;
  using ctarPtr = Spi::Ctar_set*;
  Gpio Cs, Sck, Mosi, Miso;
  static ctarPtr s_ctar [2];
	uint8_t ctar_N;

//functions
public:

  Spi(Role r=master);

  void set_cpol (Cpol c = neg);
  void set_cpha (Cpha c = first);
  void set_f_size (Fsize f = bit_8);
  void set_baudrate (Division d);
  void set_ctar (uint8_t n);
  void update_ctar ();

  static void set_cpol (Spi &, Cpol c);
  static void set_cpha (Spi &, Cpha c);
  static void set_ctar (Spi &, uint8_t c);
  static void set_baudrate (Spi &, Division d);
  static void set_f_size (Spi &, Fsize f = bit_8);

  void set_CS (Gpio::Port p, const uint8_t & pin, Gpio::mux m, Spi::CS_number);
  void set_SCK (Gpio::Port p, const uint8_t & pin, Gpio::mux m);
  void set_MOSI (Gpio::Port p, const uint8_t & pin, Gpio::mux m);
  void set_MISO (Gpio::Port p, const uint8_t & pin, Gpio::mux m);

  void settings ();
  void transmit (uint16_t data);
  uint8_t receive ();
  uint8_t exchange (uint8_t data);

  void put_data (uint16_t data, uint8_t cs, uint8_t ctar, State cont = off);
  uint16_t get_data ();
  bool flag_tcf ();
  bool flag_tfff ();
  bool flag_tfuf ();
  bool flag_txctr ();
  bool flag_rfof ();
  bool flag_rfdf ();
  void clear_flag_tcf();
  void clear_flag_tfuf();
  void clear_flag_rfof();
  void clear_flag_rfdf();


};



typedef uint16_t(Spi::*PotMemFn)() ;
typedef uint16_t(Spi::*ptr_ex)(uint16_t) ;

typedef void(Spi::*PotMemF)(uint16_t) ;

#endif

