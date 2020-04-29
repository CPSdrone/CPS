#include <Wire.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>  // not used in this demo but required!
// i2c
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();

#define LSM9DS1_SCK A5
#define LSM9DS1_MISO 12
#define LSM9DS1_MOSI A4
#define LSM9DS1_XGCS 6
#define LSM9DS1_MCS 5

float accPitch = 0;
float accRoll = 0;
float accmagYaw = 0;

float compPitch = 0;
float compRoll = 0;
float compYaw = 0;


#include <PID_v1.h>

//Define Variables we'll be connecting to
double Setpoint, Input, Output, Out, Inmate;

//Define the aggressive and conservative Tuning Parameters
double aggKp=1, aggKi=0.001, aggKd=0.01;

double consKp=0.2, consKi=0.001, consKd=0.001;                      //double consKp=1, consKi=0.05, consKd=0.25;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, aggKp, aggKi, aggKd, DIRECT);

int val;
int stick;

void setup()
{
  Serial.begin(115200);
  
  while (!Serial) {delay(1);}
  if (!lsm.begin()){while (1);}
  
  lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_16G);
  lsm.setupMag(lsm.LSM9DS1_MAGGAIN_16GAUSS);
  lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_2000DPS);
  
  //initialize the variables we're linked to
  Input = 0;
  Setpoint = 0;

  //turn the PID on
  myPID.SetMode(AUTOMATIC);

  myPID.SetOutputLimits(-90,90);
}

void loop()
{
  lsm.read();
  sensors_event_t a, m, g, temp;
  lsm.getEvent(&a, &m, &g, &temp);

  double kt = (double)micros();
  double dt = (double)(micros() - kt) / 1000000;
  
  accRoll  = (atan2(a.acceleration.y, a.acceleration.z) * 180.0) / M_PI;
  compRoll = 0.5 * (compRoll + g.gyro.x * dt) + 0.5 * accRoll;

  accPitch = -(atan2(a.acceleration.x, sqrt(a.acceleration.y * a.acceleration.y + a.acceleration.z * a.acceleration.z)) * 180.0) / M_PI;
  //accPitch  = (atan2(a.acceleration.x, a.acceleration.z) * 180.0) / M_PI;
  compPitch = 0.5 * (compPitch + g.gyro.y * dt) + 0.5 * accPitch;


  val = analogRead(A0);
  stick = map(val,0,1023,0,180);

  Setpoint = 90;

  Input = compRoll + stick;

    /*double gap = abs(Setpoint-Input); //distance away from setpoint
    if (gap < 10)
    {  //we're close to setpoint, use conservative tuning parameters
    myPID.SetTunings(consKp, consKi, consKd);
    }else{
     //we're far from setpoint, use aggressive tuning parameters
     myPID.SetTunings(aggKp, aggKi, aggKd);
     }*/

  /*if(Setpoint-Inmate<0){
    Input=-Inmate+180;}*/

  myPID.Compute();
  
  /*if(Setpoint-Inmate<0){
    Out=-Output+90;
  }else{
    Out=Output+90;}*/

  Serial.print("Thrust ");
  Serial.print(Output+90);
  Serial.print("\t");
  Serial.print("Roll ");
  Serial.print(compRoll);
  Serial.print("\t");
  Serial.print("Pitch ");
  Serial.print(compPitch);
  Serial.print("\t");
  Serial.print("Stick ");
  Serial.print(stick);
  Serial.print("\t");

  Serial.println();
}
