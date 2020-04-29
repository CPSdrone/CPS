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

float offaccX;
float offaccY;
float offaccZ;

void setupIMU()
{
  //while (!Serial) {delay(1);}
  //if (!lsm.begin()){while (1);}
  
  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_16G);
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_16GAUSS);
  lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_2000DPS);
}

void runIMU()
{
  lsm.read();
  sensors_event_t a, m, g, temp;
  lsm.getEvent(&a, &m, &g, &temp);

  double kt = (double)micros();
  double dt = (double)(micros() - kt) / 1000000;

  int OFFSET = ch[5];
  if (OFFSET>1500){
    offaccX = a.acceleration.x;
    offaccY = a.acceleration.y;
    offaccZ = a.acceleration.z;
  }
  
  // Calculate Pitch & Roll from accelerometer (deg)
  //accPitch = (atan2(a.acceleration.x, a.acceleration.z) * 180.0) / M_PI;
  accPitch = (-(atan2(a.acceleration.x-offaccX, sqrt((a.acceleration.y-offaccY) * (a.acceleration.y-offaccY) + (a.acceleration.z-offaccZ+9.81) * (a.acceleration.z-offaccZ+9.81))) * 180.0) / M_PI);
  accRoll  = ((atan2(a.acceleration.y-offaccY, a.acceleration.z-offaccZ+9.81) * 180.0) / M_PI);

  double Bfy = m.magnetic.z * sin(accRoll) - m.magnetic.y * cos(accRoll);
  double Bfx = m.magnetic.x * cos(accPitch) + m.magnetic.y * sin(accPitch) * sin(accRoll) + m.magnetic.z * sin(accPitch) * cos(accRoll);
  accmagYaw = atan2(-Bfy, Bfx) * RAD_TO_DEG;

  //Complementary filter
  compPitch = 0.9 * (compPitch + g.gyro.y * dt) + 0.1 * accPitch; //had 50/50 but while quick accel its bad
  compRoll = 0.9 * (compRoll + g.gyro.x * dt) + 0.1 * accRoll;
  compYaw = 0.9 * (compYaw + g.gyro.z * dt) + 0.1 * accmagYaw;

  
 /*Serial.print(offPitch);
 Serial.print("\t");
 Serial.print(offRoll);
 Serial.print("\t");
 Serial.println(OFFSET);
 Serial.print("");
 Serial.print(compPitch);
 Serial.print("\t");
 Serial.println(compRoll);*/

  /*accX = a.acceleration.x - map(compRoll,-90,90,-1,1)*9.81//maybe sins and tangents
  accY = a.acceleration.y - map(compPitch,-90,90,-1,1)*9.81
  accZ = a.acceleration.z // - Pitch + Roll jakoś*/
  
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


  
#if 0
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
#endif

  runPID(compRoll, compPitch, compYaw, a.acceleration.x, a.acceleration.y, a.acceleration.z );
}
