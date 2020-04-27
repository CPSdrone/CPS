#include <Wire.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();

#include <KalmanFilter.h>

#define LSM9DS1_SCK A5
#define LSM9DS1_MISO 12
#define LSM9DS1_MOSI A4
#define LSM9DS1_XGCS 6
#define LSM9DS1_MCS 5

float accPitch = 0;
float accRoll = 0;
float accmagYaw = 0;

/*float kalPitch = 0;
float kalRoll = 0;
float kalYaw = 0;

KalmanFilter kalmanX(0.001, 0.003, 0.03);     //(0.001, 0.003, 0.03)
KalmanFilter kalmanY(0.001, 0.003, 0.03);
KalmanFilter kalmanZ(0.001, 0.003, 0.03);*/

float compPitch = 0;
float compRoll = 0;
float compYaw = 0;

void setup()
{
  Serial.begin(115200);
  
  while (!Serial) {delay(1);}
  if (!lsm.begin()){while (1);}
  
  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_16G);
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_16GAUSS);
  lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_2000DPS);

}

void loop()
{
  lsm.read();
  sensors_event_t a, m, g, temp;
  lsm.getEvent(&a, &m, &g, &temp);

  double kt = (double)micros();
  double dt = (double)(micros() - kt) / 1000000;
  
  // Calculate Pitch & Roll from accelerometer (deg)
  accPitch = -(atan2(a.acceleration.x, sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z)) * 180.0) / M_PI;
  accRoll  = (atan2(a.acceleration.y, a.acceleration.z) * 180.0) / M_PI;

  double Bfy = m.magnetic.z * sin(accRoll) - m.magnetic.y * cos(accRoll);
  double Bfx = m.magnetic.x * cos(accPitch) + m.magnetic.y * sin(accPitch) * sin(accRoll) + m.magnetic.z * sin(accPitch) * cos(accRoll);
  accmagYaw = atan2(-Bfy, Bfx) * RAD_TO_DEG;

  //Complementary filter
  compPitch = 0.5 * (compPitch + g.gyro.y * dt) + 0.5 * accPitch;
  compRoll = 0.5 * (compRoll + g.gyro.x * dt) + 0.5 * accRoll;
  compYaw = 0.5 * (compYaw + g.gyro.z * dt) + 0.5 * accmagYaw;
  
  /*// Kalman filter
  kalPitch = kalmanY.update(accPitch, g.gyro.y);
  kalRoll = kalmanX.update(accRoll, g.gyro.x);
  kalYaw = kalmanZ.update(accmagYaw, g.gyro.z);

  if (kalPitch < -180 || kalPitch > 180)
    kalPitch = accPitch;
  if (kalRoll < -180 || kalRoll > 180)
    kalRoll = accRoll;
  if (kalYaw < -180 || kalYaw > 180)
    kalYaw = accmagYaw;*/

  Serial.print("Pitch");
  Serial.print("\t");
  Serial.print(accPitch);
  Serial.print("\t");
  Serial.print(compPitch);
  Serial.print("\t");
  Serial.print("Roll");
  Serial.print("\t");
  Serial.print(accRoll);
  Serial.print("\t");
  Serial.print(compRoll);
  Serial.print("\t");
  Serial.print("Yaw");
  Serial.print("\t");
  Serial.print(accmagYaw);
  Serial.print("\t");
  Serial.print(compYaw);

  Serial.println();

  delay(00);
}
