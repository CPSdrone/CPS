
// sketch for submarine
// using mega with L298N motor drivers,
// four vectored side thrusters (same as mecanum wheel car)
// two up and down thrusters
// uses depth gauge and compass to maintain depth and heading
// sends telemetry info up to control box via RS485 using Serial1
// camera video sent directly by cam over cable to control box
// uses CMPS12 via I2C to get compass heading

// char foo;


#include <PinChangeInt.h>  //for PPM sensing
#include <Adafruit_Sensor.h>
//#include "DHT.h"  //inside thermometer
#include <SPI.h>  //for depth sensor
#include <Wire.h>
#include <MedianFilter.h>  // median filter - noise on RC input

// CMPS12 compass;  //uses I2C, pins 20 (SDA) and 21 (SCL)

// MS5540 depth gauge uses SPI pins 50,51,52 and 11

#define PPMviaRC  // versus via wired to buddy box port on xmitter

// note that on the Mega, PinChangeInt only works for A8-A15, 
// D10-D15, D50-D53 & D0

    //  motor pins
    // motor number       0   1   2  3   4  ,RF,LF,RR,LR,UD 
int motorFwdPwmPin[] = { 12, 8,  4, 2, 6}; //pins 0-13 OK for PWM on Mega
int motorRevPwmPin[] = { 13, 9,  5, 3, 7}; //pins 0-13 OK for PWM on Mega

#define numMotors 5  //(2 Up/Down motors are run in parallel)

// telemetry via pins 18 & 19 to tether Green-White/Green via RS485
                  // video from camera to tether Brown-White
#define PPMpin A8 // PPM signal from tether Blue-White 
#define voltPin A9  //volt from power module
#define ampsPin A10 //amps from power module
// #define thermPin A11 //inside temp from DHT thermometer

#define liteRelayPin 34  //

#define rcSlack 60 //threshold for rc control

int setDepth = 0;  // cm depth user drove it down to
int setHeading = 0;  // compass hdg from last control
unsigned long setTwistTime = 0;
boolean autoTwist = false;
boolean rcTwist = false;
boolean armAuto = false;  //don't do auto depth & Heading until armed

long waterPressure = 0; //millibars
long waterDepth = 0;  // latest reading cvtd to cm
long airPressure = 0;
int waterTemp = 0;  //degrees C

float volts = 0;
float amps = 0;
float tempF;
int iTempC;
String liteStatus = "off";
unsigned long prevTele = 0;
int counter = 0;

    // rc stuff

  // control stick tuning parameters
#ifdef PPMviaRC
  #define highUD 1970
  #define neutUD 1560
  #define lowUD 1104 

  #define rightLR 1732
  #define neutLR 1424
  #define leftLR 1112 

  #define cwTW 1820
  #define neutTW 1514
  #define ccwTW 1192 

  #define highFB 1812
  #define neutFB 1504
  #define lowFB 1184 

  #define offLI 1970  //use with rcSlack 
  #define middleLI 1544
  #define onHiLI 1132 

  #define offCA 2000  //use with rcSlack 
  #define on1CA 1588
  #define on2CA 1168 

  #define offx6 1970  //use with rcSlack 
  #define onx6 1136
#else  //wired to xmitter
  #define highUD 1904
  #define neutUD 1520
  #define lowUD 1084 

  #define rightLR 1780
  #define neutLR 1500
  #define leftLR 984 

  #define cwTW 1856
  #define neutTW 1500
  #define ccwTW 1060 

  #define highFB 1856
  #define neutFB 1456
  #define lowFB 1060 

  #define offLI 1900  //use with rcSlack 
  #define middleLI 1500
  #define onHiLI 1104 

  #define offCA 1900  //use with rcSlack 
  #define on1CA 1500
  #define on2CA 1104 

  #define offx6 1970  //use with rcSlack 
  #define onx6 1136
#endif

MedianFilter medUD(5, neutUD); //size(odd nbr) & seed
MedianFilter medLR(5, neutLR); //size & seed
MedianFilter medFB(5, neutFB); //size & seed
MedianFilter medTW(5, neutTW); //size & seed

    //globals set by PPM signal and used for navigation ctl
  int rcUD = neutUD;     // up/down (throttle)
  int rcTW = neutTW;     // twist (yaw)
  int rcFB = neutFB;     // forward/bkwd (pitch)
  int rcLR = neutLR;     // left/right (roll)
  int rcLI = 1500;     // lites 3 way sw (flt mode)
  int rcx6 = 1500;     // push button sw (bind)
  int rcCA = 1500;     // cam 3 way sw (flaps)
  int rcx8 = 1500;     // Ch8 2 way sw (aux) only avail on wired mode

    // vector calculation factors
float Vd = 0; //desired speed
float theta = 0; //desired angle
float Vtwist = 0;  // rotation speed
 
    //compass stuff - also used for inside temperature
#define CMPS12_ADDRESS 0x60  // Address of CMPS12 shifted right one bit for arduino wire library
#define ANGLE_16  2           // Register to read 16bit angle from
#define TEMP_16  24           // Register to read 16bit temp from

unsigned char high_byte, low_byte;
unsigned int angle16;
unsigned int temp16;
//#define DHTTYPE DHT11   // DHT 11. this must be the kind I have
//DHT dht(thermPin, DHTTYPE);


void  setup()
{

  Serial.begin(9600);   // console hardware serial D0 & D1
  Serial.print("Starting SubRun");
  Serial.println(" Compiled: " __DATE__ ", " __TIME__ );
  Serial1.begin(9600);   // telemetry data to subbase

  getMS5540Calib();  // get calib factors for depth sensor
  airPressure = getMS5540Pressure();  //Arduino s/b turned on before
  if (airPressure > 1200) {            // sub goes into the water
    airPressure = 1000;                // else assume 1000 mbar
  }
  setDepth = 0;    // set initial depth

  Wire.begin();  //I2C for CMPS12 compass
   
     // enable interrupt for remote controls
  pinMode(PPMpin, INPUT_PULLUP);
  attachPinChangeInterrupt(PPMpin, readPPM, FALLING); // RISING makes no diff
  delay(200);
  
     // initialize motor pins
  for (int x=0; x<numMotors; x++) {
    pinMode (motorFwdPwmPin[x], OUTPUT);
    pinMode (motorRevPwmPin[x], OUTPUT);
  }

  pinMode (voltPin, INPUT);
  pinMode (ampsPin, INPUT);

  stopAllMotors();  // stopped at the beginning
        // get starting control values
  getPPM();   //convert PPM signal to PWM values

  pinMode(liteRelayPin, OUTPUT);   // set pin as OUTPUT
  digitalWrite(liteRelayPin, LOW);  //off to start

  setHeading = getHeading();  //get initial heading

}

void loop()
{
 /* Serial.print(" rcLR=");Serial.print(rcLR);
  Serial.print(" rcFB=");Serial.print(rcFB); //ctls set in pin chg ISR
  Serial.print(" rcUD=");Serial.print(rcUD);
  Serial.print(" rcTW=");Serial.print(rcTW);
  Serial.print(" rcLI=");Serial.print(rcLI);
  Serial.print(" rcCA=");Serial.println(rcCA);
*/
    if (rcFB == 0) {  // if xmitter not turned on we do nothing
     stopAllMotors();  
     return;  // go around again
    }  
    
      // main part of loop
   getPPM();   //convert PPM signal to PWM values
 //  testMotors();  //testing only
   waterPressure = getMS5540Pressure() - airPressure;  //get depth and water temp
   waterDepth = waterPressure;  //millibars = cm
   
      
   if (setTwistTime > millis()) {  //done waiting for it to stop twisting?
     if (rcTwist) { 
       setHeading = getHeading(); //set desired heading
       rcTwist = false;
     }
     else autoTwist = false;
   } 
   runByRc();   //let the operator drive using rc Xmitter 
                //with Rcvr in ctl box & cable to sub 
                // depth and heading are automatic with user override
   getPowerData(); //read sensors in power unit
 
      // only do this every 1/4 sec to avoid stepping on prev send
   if (millis() - prevTele > 250) { 
     iTempC = getTempData();
     putTelemetry(); //
     prevTele = millis();
     counter++;  // used only to show on screen that feed is alive
     if (counter > 9) counter = 0;
   }

      //check for light control
   if (rcLI - middleLI > rcSlack*2) {
     digitalWrite(liteRelayPin, LOW); // turn it on
     liteStatus = "on";
   } else if (middleLI - rcLI > rcSlack*2) {
     digitalWrite(liteRelayPin, HIGH); // turn it off
     liteStatus = "off";
   }
   
} // end loop

void runByRc() {
      getTransVectors();     // desired angle & speed of LR and FB movement
      checkArming();         // see if user is arming or disarming
      maintainHeading();     // get twist amt using rc & compass
 //     Serial.print(" Vd="); Serial.print(Vd);
  //    Serial.print(" theta="); Serial.print(theta);
 //     Serial.print(" Vtwist="); Serial.println(Vtwist);
      if(Vd != 0 || Vtwist != 0) {
        runVectThrusters();  // calc & run four vectored thrusters 
      } else {
        stopVectMotors();
      }
      maintainDepth();  // adjust depth based on rc & depth gauge
}  



void getPowerData() {
    int wkVolts = analogRead(voltPin);
    volts = wkVolts/22.2;
 //   volts = wkVolts/18;  //4/16/18
    int wkAmps = analogRead(ampsPin);
    amps = wkAmps/9.7;  //need to do an average over some seconds
}

int getTempData() {
  Wire.beginTransmission(CMPS12_ADDRESS);  //starts communication with CMPS12
  Wire.write(TEMP_16);                     //Sends the register we wish to start reading from
  Wire.endTransmission();
 
  // Request 2 bytes from the CMPS12
  Wire.requestFrom(CMPS12_ADDRESS, 2);       
  while(Wire.available() < 2);        // Wait for all bytes to come back
  high_byte = Wire.read();
  low_byte = Wire.read();
  temp16 = high_byte;                 // Calculate 16 bit angle
  temp16 <<= 8;
  temp16 += low_byte;
  return temp16-9;  //slight adjustment
//    float tempTemp = dht.readTemperature();  //
//    if (!isnan(tempTemp)) {
//       iTempC = tempTemp;
//       tempF = (tempTemp*9/5)+32;  //move it in Celsius
//    }   
}

void putTelemetry() {
 //   Serial.print(" d=");Serial.print(setDepth);              
 //   Serial.print(" hdg=");Serial.print(setHeading);              
//    Serial.print(" l=");Serial.print(liteStatus);              
 //   Serial.print(" wC=");Serial.print(waterTemp);              
//    Serial.print(" iC=");Serial.print(iTempC);              
  //  Serial.print(" V=");Serial.print(volts,2);              
 //   Serial.print(" A=");Serial.println(amps,2);  

          // this goes via serial and RS485 board to base  
    Serial1.print("$");  // start character          
    Serial1.print("D:");Serial1.print(setDepth);              
    Serial1.print(" HDG:");Serial1.print(getHeading());              
    Serial1.print(" wC:");Serial1.print(waterTemp);              
    Serial1.print(" iC:");Serial1.print(iTempC);              
    Serial1.print("   L:");Serial1.print(liteStatus);              
    Serial1.print(" V:");Serial1.print(volts,1);              
    Serial1.print(" A:");Serial1.print(amps,1); 
    if (armAuto) Serial1.print(" ARMD");
    else Serial1.print(" NOGO"); 
    // counter so viewer can see if anything changes          
    Serial1.print(" ");Serial1.print(counter);  
    Serial1.print("        "); 
    Serial1.println("%");  //  end character 
}
