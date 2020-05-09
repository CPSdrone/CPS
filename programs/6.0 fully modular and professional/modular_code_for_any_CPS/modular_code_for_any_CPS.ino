

//////PPM READING PRE SETUP/////

#define RECEIVE_PIN 2
#define CHANNEL_AMOUNT 8
#define DETECTION_SPACE 2500
#define METHOD RISING

int ch[CHANNEL_AMOUNT + 1];

//////OSD PRE SETUP/////
#include <SPI.h>
#include <max7456.h>
Max7456 osd;

//////FOR ALL GAMERS OUT THERE/////
#include <Servo.h>;

//////THERMOMETER PRE SETUP///////
#include <OneWire.h>
#include <DallasTemperature.h>


void setup()
{
  Serial.begin(115200);

  pinMode(RECEIVE_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(RECEIVE_PIN), ppm_interrupt, METHOD);

  SPI.begin();
  
  osd.init(10);
  osd.setDisplayOffsets(40,9);
  osd.activateOSD();
  internalStart();
}


void loop()
{
ppm_write();
steer();
//steerint();
//modulerun();

unsigned long counter = 0;
counter = millis()/1000;

osd.print(int(counter%60), 15, 1, 2, 0, false, true);
osd.print(":", 14, 1, false, true);
osd.print(int(counter/60), 12, 1, 2, 0, false, true);
}
