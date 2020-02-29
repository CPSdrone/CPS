//////PPM READING PRE SETUP/////

#define RECEIVE_PIN 2
#define CHANNEL_AMOUNT 8
#define DETECTION_SPACE 2
#define METHOD RISING

//////OSD PRE SETUP/////
#include <SPI.h>
#include <max7456.h>
Max7456 osd;

//////FOR ALL GAMERS OUT THERE/////
#include <Servo.h>;



void setup()
{
  Serial.begin(9600);

  pinMode(RECEIVE_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(RECEIVE_PIN), ppm_interrupt, METHOD);

  internalStart();
}


void loop()
{
ppm_write();
steer();
steerint();
modulerun();

unsigned long counter = 0;
counter = millis()/1000;

osd.print(int(counter%60), 15, 1, 2, 0, false, true);
osd.print(":", 14, 1, false, true);
osd.print(int(counter/60), 12, 1, 2, 0, false, true);

}
