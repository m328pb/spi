#include "spi.h"
#include <avr/io.h>

SPI::SPI() {}

/*
 * @brief initialize SPI with select clock division
 * @param CPU freq divider, default 16
 * @details
 * set bus speed by simply dividing CPU clock,
 * allowed values are 2,4,8,16,32,64,128.
 * then initialize SPI with CS pin and CS logic as per default
 */
void SPI::init(uint8_t clock_div) {
  this->clock_div = clock_div;
  init();
}

/*
 * @brief initialize SPI with select clock paremeters
 * @param clock_div - CPU freq divider, default 16
 * @param cs - chip select pin, default PB2
 * @param auto_cs - automatically swith cs pin at start/end of transmission
 * @details
 * set bus speed by simply dividing CPU clock,
 * allowed values are 2,4,8,16,32,64,128.
 * Set cs pin and set cs logic on/off
 */
void SPI::init(uint8_t clock_div, uint8_t cs, uint8_t auto_cs) {
  this->clock_div = clock_div;
  this->cs = cs;
  this->auto_cs = auto_cs;
  init();
}

/*
 * @brief initialize with default parameters
 * @details
 * initialize with defaults parameters or previously set
 */
void SPI::init() {
  // The PRSPI bit must be written to zero to enable SPI module
  _PRR &= ~(1 << _PRSPI);

  // Set SCK, MOSI and CS as output
  // CS as output in master mode is not affecting the SPI
  // when set as input CS can switch SPI mode to slave
  // pull up CS before switching to output, other way slave may wake up
  cs_off();                                     // CS pullup: disable slave
  DDRB |= (1 << SCK) | (1 << MOSI) | (1 << cs); // SCK, MOSI, CS output
  DDRB &= ~(1 << MISO);                         // MISO input

  SPIE_backup = _SPCR & (1 << SPIE);
  _SPCR = ((0 &             // control register (p.256 ATmega328PB)
            ~(1 << SPIE)) | // disable interrupts SPIE=0
           (1 << SPE) |     // SPI Enable =1,
           (1 << MSTR)) &   // set Master mode MSTR=1,
          ~(1 << DORD) &    // Data order:  MSB first when DORD=0
          ~(1 << CPOL) &    // SPI mode: CPOL=0: lead edge - Rising;
                            // trailing edge: Falling
          ~(1 << CPHA);     // SPI mode: CPHA=0: lead edge - Sample;
                            // trailing edge: Setup

  // Set clock rate
  uint8_t reg_div;
  //				SPI2X SPR1[1] SPR1[0]
  reg_div = (clock_div == 2)     ? 0b100
            : (clock_div == 4)   ? 0b000
            : (clock_div == 8)   ? 0b101
            : (clock_div == 16)  ? 0b001
            : (clock_div == 32)  ? 0b110
            : (clock_div == 64)  ? 0b010
            : (clock_div == 128) ? 0b011
                                 : 0b111; // default 64

  _SPSR = (_SPSR & ~(1 << SPI2X)) | (reg_div & 0b100);
  _SPCR = (_SPCR & ~(1 << SPR1)) | (reg_div & 0b010);
  _SPCR = (_SPCR & ~(1 << SPR0)) | (reg_div & 0b001);
}

/*
 * @brief Send single byte to the slave device over SPI
 * @param data 8 bit data to send
 * @return recived data
 * @details
 * This function sends single byte data to the slave device over I2C.
 * if auto_cs==1, handle chip selection line pullup/pulldown
 */
uint8_t SPI::send(uint8_t cData) {
  if (auto_cs)
    cs_on();

  /* Start transmission */
  SPDR = cData;
  /* Wait for transmission complete */
  do {
  } while (!(_SPSR & (1 << SPIF)));

  if (auto_cs)
    cs_off();

  return SPDR0;
}

/*
 * @brief Send data to the slave device over SPI
 * @param data - data to send
 * @param len - number of bytes to send
 * @details
 * This function sends data to the slave device over SPI.
 * if auto_cs==1, handle chip selection line pullup/pulldown.
 * Handle cs pin only at begining/end of the whole data (not every byte)
 */
void SPI::send_ln(uint8_t *data, uint8_t len) {
  uint8_t auto_cs_bckp = auto_cs;
  auto_cs = false;
  cs_on();
  while (len--) {
    send(*data++);
  }
  auto_cs = auto_cs_bckp;
  cs_off();
}

void SPI::cs_on() {
  PORTB &= ~(1 << cs); // pull low: activate slave
}

void SPI::cs_off() {
  PORTB |= (1 << cs); // pull up: deactivate slave
}

/*
 * @brief turn off SPI and restore interrupts status
 */
void SPI::off() {
  _PRR |= (1 << _PRSPI);
  _SPCR = (_SPCR & ~(1 << SPIE)) | SPIE_backup; // restore interrupts
}
