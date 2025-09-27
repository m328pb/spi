/*
 simply routine to check and demonstarte the library
  - plug in network analyzer
  - check recived text with network analyzer
 */
#include "spi.h"
#include <avr/delay.h>
int main(){
	SPI spi;
	const char *txt_def = "SPI test: 1MHz";
	const char *txt_4MHz = "SPI test: 4MHz";
	while(1){
	spi.init(16,PB2,true);
	spi.send_ln((uint8_t *)txt_def,15);
	spi.init(4);
	spi.send_ln((uint8_t *)txt_4MHz,15);
	_delay_ms(1000);
	}
	spi.off();
}
