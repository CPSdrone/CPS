#include <SPI.h>
#include <max7456.h>

#define redLed 3
#define greenLed 4

Max7456       osd;
unsigned long counter = 0;
byte          tab[] = {0xC8, 0xC9};

void setup() {
  SPI.begin();

  osd.init(10);
  osd.setDisplayOffsets(60, 18);
  osd.setBlinkParams(_8fields, _BT_BT);

  osd.activateOSD();
  osd.printMax7456Char(0x7a, 0, 12);
  osd.print("makabra w moim miescie", 1, 2);
  osd.print("zacznie sie za :", 1, 4);
  osd.print("0",12,8);

  osd.printMax7456Char(0xD1, 9, 6, true);
  osd.printMax7456Chars(tab, 2, 2, 7);
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);

  //base time = 160ms,time on = time off.
}

void loop() {
  if (counter % 2 == 0) {
    digitalWrite(redLed, LOW);
    digitalWrite(greenLed, HIGH);
  } else {
    digitalWrite(redLed, HIGH);
    digitalWrite(greenLed, LOW);
  }

  counter = millis() / 100;

  osd.print(int(counter / 60), 10, 10, 3, 1, false, false);
  osd.print(int(counter % 60), 13, 11, 2, 0, false, true);

  osd.print("ALE JAJA", 13,14);

  delay(100);
}
