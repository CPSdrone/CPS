/////////////////////////////////PPM READING PRE SETUP/////////////////////////////////

#define RECEIVE_PIN 2
#define CHANNEL_AMOUNT 8
#define DETECTION_SPACE 2500
#define METHOD RISING

int ch[CHANNEL_AMOUNT + 1];

/////////////////////////////////STEERING PRE SETUP/////////////////////////////////

#include <Servo.h>;
Servo motorupdown;
Servo motorside;
Servo motorleft;
Servo motorright;

Servo motorcamera;

int valLUP;
int valRUP;
int valLLR;
int valRLR;

int varCAM;
int conCAM;
/////////////////////////////////OSD PRE SETUP/////////////////////////////////

#include <SPI.h>
#include <max7456.h>
Max7456 osd;

unsigned long counter = 0;

float volt;
float valbat;

/////////////////////////////////IMU PRE SETUP/////////////////////////////////

#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>  // not used in this demo but required!

Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();

#define LSM9DS1_SCK A5
#define LSM9DS1_MISO 12
#define LSM9DS1_MOSI A4
#define LSM9DS1_XGCS 6
#define LSM9DS1_MCS 5

///////////////////////////////////////LED/////////////////////////////////////////

int ledPinGreen = 9;
int ledPinRed = 11;

int ledlights = 4;
int vallights;
int valBlink;









void OSDstart() {
  SPI.begin();
  
    osd.init(10);
    osd.setDisplayOffsets(35,0);
    osd.activateOSD();
}










void IMUsetup(){
      while (!Serial) {
          delay(1); // will pause Zero, Leonardo, etc until serial console opens
        }
        
        Serial.println("LSM9DS1 data read demo");
        
        // Try to initialise and warn if we couldn't detect the chip
        if (!lsm.begin())
        {
          Serial.println("Oops ... unable to initialize the LSM9DS1. Check your wiring!");
          while (1);
        }
        Serial.println("Found LSM9DS1 9DOF");
      
        // helper to just set the default scaling we want, see above!
        setupSensor();
}
void setupSensor()
    {
      // 1.) Set the accelerometer range
      lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_2G);
      //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_4G);
      //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_8G);
      //lsm.setupAccel(lsm.LSM9DS1_ACCELRANGE_16G);
      
      // 2.) Set the magnetometer sensitivity
      lsm.setupMag(lsm.LSM9DS1_MAGGAIN_4GAUSS);
      //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_8GAUSS);
      //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_12GAUSS);
      //lsm.setupMag(lsm.LSM9DS1_MAGGAIN_16GAUSS);
    
      // 3.) Setup the gyroscope
      //lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_245DPS);
      lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_500DPS);
      //lsm.setupGyro(lsm.LSM9DS1_GYROSCALE_2000DPS);
    }











void ledStart(){
pinMode(ledPinGreen, OUTPUT);
pinMode(ledPinRed, OUTPUT);
pinMode(ledlights, OUTPUT);
  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 1) {
    analogWrite(ledPinGreen, fadeValue);
    analogWrite(ledPinRed, fadeValue);
    analogWrite(ledlights, fadeValue);
    delay(10);
  }
delay(100);
digitalWrite(ledPinRed, LOW);
digitalWrite(ledPinGreen, HIGH);
delay(100);
digitalWrite(ledPinRed, HIGH);
digitalWrite(ledPinGreen, LOW);
delay(100);
digitalWrite(ledPinRed, LOW);
digitalWrite(ledPinGreen, HIGH);
delay(100);
digitalWrite(ledPinRed, HIGH);
digitalWrite(ledPinGreen, LOW);
delay(100);
digitalWrite(ledPinRed, LOW);
digitalWrite(ledPinGreen, HIGH);
delay(100);
digitalWrite(ledPinRed, HIGH);
digitalWrite(ledPinGreen, LOW);
delay(100);
digitalWrite(ledPinGreen, HIGH);
}












void setup() {
Serial.begin(115200);
pinMode(RECEIVE_PIN, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(RECEIVE_PIN), ppm_interrupt, METHOD);

                                                                                  ///////////////////////////////
//OSDstart();
//IMUsetup();
//ledStart();
}












void loop(){
ppm_write();

//Serial.print(ch[1]);Serial.print("\t");
//Serial.print(ch[2]);Serial.print("\t");
//Serial.print(ch[3]);Serial.print("\t");
//Serial.print(ch[4]);Serial.print("\t");
//Serial.print(ch[5]);Serial.print("\t");
//Serial.print(ch[6]);Serial.print("\t");
//Serial.print(ch[7]);Serial.print("\t");
//Serial.print(ch[8]);Serial.print("\n");
                                                                     /////////////                              //////////
counter = millis()/1000;
                                                                     
//analogreading();
//IMUrun();
steer();
//ledRun();
//OSDrun();

valcheck();
}











void ppm_write()
{
static unsigned long int t;
if (millis() - t < 100)
return 0;
for (byte i = 0; i < CHANNEL_AMOUNT + 1; i++)
{
Serial.print(ch[i]);
Serial.print("\t");
}
Serial.print("\n");
t = millis();
}

void ppm_interrupt()
{
static byte i;
static unsigned long int t_old;
unsigned long int t = micros(); //store time value a when pin value falling/rising
unsigned long int dt = t - t_old; //calculating time inbetween two peaks
t_old = t;

if ((dt > DETECTION_SPACE) || (i > CHANNEL_AMOUNT))
{
i = 0;
}
ch[i++] = dt;
}










void analogreading(){
  valbat=analogRead(A2);

  volt = valbat * 5.0/1023;
}










void IMUrun(){
        lsm.read();

      sensors_event_t a, m, g, temp;
    
      lsm.getEvent(&a, &m, &g, &temp); 
    
      Serial.print("Accel X: "); Serial.print(a.acceleration.x); Serial.print(" m/s^2");
      Serial.print("\tY: "); Serial.print(a.acceleration.y);     Serial.print(" m/s^2 ");
      Serial.print("\tZ: "); Serial.print(a.acceleration.z);     Serial.println(" m/s^2 ");
    
      Serial.print("Mag X: "); Serial.print(m.magnetic.x);   Serial.print(" gauss");
      Serial.print("\tY: "); Serial.print(m.magnetic.y);     Serial.print(" gauss");
      Serial.print("\tZ: "); Serial.print(m.magnetic.z);     Serial.println(" gauss");
    
      Serial.print("Gyro X: "); Serial.print(g.gyro.x);   Serial.print(" dps");
      Serial.print("\tY: "); Serial.print(g.gyro.y);      Serial.print(" dps");
      Serial.print("\tZ: "); Serial.print(g.gyro.z);      Serial.println(" dps");

    delay(200);
}










void steer(){

  
              motorupdown.attach(12,1000,2000);
              motorleft.attach(10,1000,2000);
              motorright.attach(9,1000,2000);
              motorcamera.attach(8,500,2500);
              
              valLLR = map(ch[4], 1000, 2000, 0, 180);
              valRUP = map(ch[2], 1000, 2000, 0, 180);
              valLUP = map(ch[3], 1000, 2000, 0, 180);
              //valRLR = map(ch[1], 1000, 2000, 0, 180);
              
                     
      motorupdown.write(180-valLUP);
      //motorside.write(valRLR);
      motorleft.write(valRUP+(valLLR-90));
      motorright.write(valRUP-(valLLR-90));
      
      conCAM = constrain(conCAM,0, 180);
      conCAM=conCAM+(90-varCAM)/8;
      motorcamera.write(conCAM);

      //vallights = map(ch[10], 1000, 2000, 0, 180);
      //Serial.print(vallights);
}











void ledRun(){
    vallights = map(ch[7], 1000, 2000, 0, 180);
    Serial.println(vallights);

  if(vallights=2) analogWrite(ledlights, 980);
  else if(vallights=1) analogWrite(ledlights, 490);
  else if(vallights=0) analogWrite(ledlights, 0);

  if((valBlink=0)&&(micros()%0)) digitalWrite(ledlights, LOW);

  if(volt>=10.5){
  digitalWrite(ledPinGreen, HIGH);
  digitalWrite(ledPinRed, HIGH);
  }else if(volt<10.5){
    if(counter%10){
    digitalWrite(ledPinGreen, LOW);
    digitalWrite(ledPinRed, LOW);
    }
  }else if(volt<10.2){
    if(counter%4){
    digitalWrite(ledPinGreen, LOW);
    digitalWrite(ledPinRed, LOW);
    }
  }
}









void OSDrun(){

if(volt>11.7){
  osd.printMax7456Char(0x90,0,1);
  osd.printMax7456Char(0x00,1,1);
  osd.printMax7456Char(0x00,2,1);
  osd.printMax7456Char(0x00,3,1);
}else if(volt>11.25){
  osd.printMax7456Char(0x91,0,1);
  osd.printMax7456Char(0x00,1,1);
  osd.printMax7456Char(0x00,2,1);
  osd.printMax7456Char(0x00,3,1);
}else if(volt>10.8){
  osd.printMax7456Char(0x92,0,1);
  osd.printMax7456Char(0x00,1,1);
  osd.printMax7456Char(0x00,2,1);
  osd.printMax7456Char(0x00,3,1);
}else if(volt>10.5){
  osd.printMax7456Char(0x93,0,1);
  osd.printMax7456Char(0x00,1,1);
  osd.printMax7456Char(0x00,2,1);
  osd.printMax7456Char(0x00,3,1);
}else if(volt>10.35){
  osd.printMax7456Char(0x94,0,1);
  osd.printMax7456Char(0x00,1,1);
  osd.printMax7456Char(0x00,2,1);
  osd.printMax7456Char(0x00,3,1);
}else if(volt>10.05){
  osd.printMax7456Char(0x95,0,1);
  osd.printMax7456Char(0x00,1,1);
  osd.printMax7456Char(0x00,2,1);
  osd.printMax7456Char(0x00,3,1);
}else if(volt>9){
  osd.printMax7456Char(0x96,0,1);
  osd.printMax7456Char(0x00,1,1);
  osd.printMax7456Char(0x00,2,1);
  osd.printMax7456Char(0x00,3,1);
}else if(volt<=9){
  osd.print("dead",0,1);
}

osd.print(int(counter%60), 15, 1, 2, 0, false, true);
osd.print(":", 14, 1, false, true);
osd.print(int(counter/60), 12, 1, 2, 0, false, true);

osd.printMax7456Char(0xF5, 22, 1);
osd.printMax7456Char(0xF6, 23, 1);
osd.print(analogRead(ledlights), 24, 1, 2, 1, false, true);
}











void valcheck(){
    Serial.print(" left : ");
    Serial.print(valRUP+(valLLR-90));
    Serial.print(" right : ");
    Serial.print(valRUP-(valLLR-90));
    Serial.print(" front : ");
    Serial.print(180-valLUP);
    Serial.print(" camera : ");
    Serial.print(conCAM);
}
