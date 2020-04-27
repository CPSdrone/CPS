
//////FOR ALL GAMERS OUT THERE/////
#include <Servo.h>
//////THERMOMETER PRE SETUP///////
#include <OneWire.h>
#include <DallasTemperature.h>

#define motorPIN 7
#define steeringPIN A5

Servo motor;
int val;

void setup() {
  Serial.begin(115200);
}

void loop() {
  motor.attach(motorPIN, 1000, 2000);
  //val = map(analogRead(A0),0,1023,0,180);
  //motor.write(val);
  //Serial.println(val);
  val = map(analogRead(steeringPIN),0,1023,1000,2000);
  motor.writeMicroseconds(val);
  Serial.println(val);
}
