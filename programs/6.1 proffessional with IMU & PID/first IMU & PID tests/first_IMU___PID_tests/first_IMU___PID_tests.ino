//IMU & PID test

//////PPM READING PRE SETUP/////

#define RECEIVE_PIN 2
#define CHANNEL_AMOUNT 8
#define DETECTION_SPACE 2500
#define METHOD RISING

int ch[CHANNEL_AMOUNT + 1];

/*//////OSD PRE SETUP/////
#include <SPI.h>
#include <max7456.h>
Max7456 osd;*/

//////FOR ALL GAMERS OUT THERE/////
#include <Servo.h>

//////IMU//////////
#include <Wire.h>
#include <Kalman.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>  // not used in this demo but required!
// i2c
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();

//////PID////////
#include <PID_v1.h>

void setup()
{
  Serial.begin(115200);

  pinMode(RECEIVE_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(RECEIVE_PIN), ppm_interrupt, METHOD);

  setupIMU();
  setupPID();
}


void loop()
{
ppm_write();
runIMU();
runPID();
steer();

}
