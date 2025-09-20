/*
 Used to measure memory usage: env:mem_usage in platformio.ini
 then do avr-size on firmware.elf
 */
#include "spi.h"

int main() {
  uint8_t data[] = {0};
  SPI spi;

  spi.init(16, PB4, 1);
  spi.send(0);
  spi.send_ln(data, 1);
  spi.cs_off();
  spi.cs_on();
  spi.off();
}
