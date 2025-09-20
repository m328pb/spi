
#include <SPI.h>

void setup() {
  // Initialize SPI as master
  SPI.begin();

  // Set SPI clock to 4 MHz (assuming 16 MHz system clock)
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));

  // Optional: configure CS pin (D10 on Arduino UNO)
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH); // idle high
}

void loop() {
  digitalWrite(10, LOW);   // select slave

  // Send "test"
  SPI.transfer('S');
  SPI.transfer('P');
  SPI.transfer('I');
  SPI.transfer(' ');
  SPI.transfer('t');
  SPI.transfer('e');
  SPI.transfer('s');
  SPI.transfer('t');
  SPI.transfer('t');
  SPI.transfer(':');
  SPI.transfer(' ');
  SPI.transfer('1');
  SPI.transfer('M');
  SPI.transfer('H');
  SPI.transfer('z');

  digitalWrite(10, HIGH);  // deselect slave

  // Set SPI clock to 4 MHz (assuming 16 MHz system clock)
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(10, LOW);   // select slave
  // Send "test"
  SPI.transfer('S');
  SPI.transfer('P');
  SPI.transfer('I');
  SPI.transfer(' ');
  SPI.transfer('t');
  SPI.transfer('e');
  SPI.transfer('s');
  SPI.transfer('t');
  SPI.transfer('t');
  SPI.transfer(':');
  SPI.transfer(' ');
  SPI.transfer('4');
  SPI.transfer('M');
  SPI.transfer('H');
  SPI.transfer('z');

  digitalWrite(10, HIGH);  // deselect slave
  delay(1000);             // send once per second
}
