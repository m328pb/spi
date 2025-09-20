#pragma once
#include <avr/io.h>

// clock divider(p.255 ATmega328PB)
// speed is set as F_CPU / div
// possible values 4, 16, 64, 128 for SPEED_MULT=0
// and 2,8,32 for SPEED_MULT =1
#define DEFAULT_DIV 16
#define DEFAULT_SPEED_MULT 0

// m328p and m328pb define registers slightly different
// becouse m328pb has two TWI units
#if defined(__AVR_ATmega328PB__)
#define _PRR PRR0
#define _PRSPI PRSPI0
#define _SPCR SPCR0
#define _SPSR SPSR0
#elif defined(__AVR_ATmega328P__)
#define _PRR PRR
#define _PRSPI PRSPI
#define _SPCR SPCR
#define _SPSR SPSR
#else
#error "Usuported MCU"
#endif

// SPI pins
#define SCK PB5
#define MISO PB4
#define MOSI PB3

class SPI {
private:
  uint8_t auto_cs = false; // automatically select chip
                           // on begining of transmission
                           // and deselect on end. For most cases
                           // must be handled manualy as per
                           // device specific communication

  uint8_t cs = PB2;                // chip select pin
  uint8_t clock_div = DEFAULT_DIV; // freq divider to defnine speed
  uint8_t SPIE_backup;             // to restore interrupt status

public:
  SPI();

  void init(void);
  void init(uint8_t clock_div, uint8_t cs, uint8_t auto_cs);
  void init(uint8_t clock_div);
  uint8_t send(uint8_t data);                  // send single byte
  void send_ln(uint8_t *data, uint8_t len); // send data of len bytes
  void cs_on();                             // chip select (pull low)
  void cs_off();                            // chip unselect (pull up)
  void off(void);
};
