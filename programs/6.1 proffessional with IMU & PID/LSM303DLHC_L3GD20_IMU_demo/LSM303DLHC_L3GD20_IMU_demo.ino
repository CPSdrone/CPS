#include <Wire.h>
#include <L3G.h>

L3G gyro;

#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);

void setup() {
  #ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif
Serial.begin(9600);
mag.enableAutoRange(true);
accel.begin();
mag.begin();
gyro.init();
gyro.enableDefault();
}

void loop() {
sensors_event_t event;
sensors_event_t evento;
accel.getEvent(&evento);
mag.getEvent(&event);
gyro.read();

  Serial.print("G ");
  Serial.print("X: ");
  Serial.print((int)gyro.g.x*0.00875);
  Serial.print(" Y: ");
  Serial.print((int)gyro.g.y*0.00875);
  Serial.print(" Z: ");
  Serial.print((int)gyro.g.z*0.00875);
  Serial.print("\t");

  Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");
  Serial.print("\t");
  
  Serial.print("X: "); Serial.print(evento.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(evento.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(evento.acceleration.z); Serial.print("  ");Serial.println();
}
