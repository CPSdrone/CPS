unsigned long int ft, lt, x, tfail;
int ch, chx[9][13];
const int idx   = 10;
const int total = 11;
const int val   = 12; //End of specifying the integers for ppm signal reading 

int valSTOP;
int valLUP;
int valRUP;
int valLLR;

//int valCAM;
//int cameraset = 0;
int varCAM;
int conCAM = 90;

int varDOL;
int varGOR;
int varPRA;
int varLEW;
int conDOL = 90;
int conGOR = 90;
int conPRA = 90;
int conLEW = 90;

int activation = 5;

#include <Wire.h>
#include <SPI.h>
#include <max7456.h>
//#define redLed 3
//#define greenLed 4

Max7456       osd;
//unsigned long counter = 0;
//byte          tab[] = {0xC8, 0xC9};
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>  // not used in this demo but required!



// i2c
Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();

#define LSM9DS1_SCK A5
#define LSM9DS1_MISO 12
#define LSM9DS1_MOSI A4
#define LSM9DS1_XGCS 6
#define LSM9DS1_MCS 5
// You can also use software SPI
//Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1(LSM9DS1_SCK, LSM9DS1_MISO, LSM9DS1_MOSI, LSM9DS1_XGCS, LSM9DS1_MCS);
// Or hardware SPI! In this case, only CS pins are passed in
//Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1(LSM9DS1_XGCS, LSM9DS1_MCS);


#include <Servo.h>;
Servo motorfront;
Servo motorleft;
Servo motorright;
Servo motorcamera;
Servo motorlapa;
Servo motordol;
Servo motorgora;
Servo motorprawo;
Servo motorlewo;

void decodePPM() {  
  lt    = micros();
  tfail = millis();
  x     = lt-ft;
  ft    = lt;
if (x>3000) {
    ch          = 0;
    chx[0][val] = x;
  } else {
    ch+=1;
int indx       = chx[ch][idx];
    chx[ch][total] = chx[ch][total] - chx[ch][indx];
    chx[ch][indx]  = x;
    chx[ch][total] = chx[ch][total] + chx[ch][indx];
    chx[ch][idx]   = chx[ch][idx] + 1;
if (chx[ch][idx]>9) {
      chx[ch][idx] = 0;
    }
    chx[ch][val] = chx[ch][total]/10;
  }
}//end of code for ppm reading

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

void setupmax7456() {
  SPI.begin();
  
  osd.init(10);
  osd.setDisplayOffsets(60, 18);
  osd.activateOSD();

  
  osd.print("activation in", 10, 4);
  osd.print(activation, 13, 8, 1, 0, false, true);

}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  ft=0; lt=0; x=0; tfail=0; ch=0;
for (int i=0; i<9; i++) {
for (int j=0; j<13; j++) {
      chx[i][j]=0;
    }
  }
pinMode(2, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(2), decodePPM, FALLING);

//while (!Serial) {
//    delay(1); // will pause Zero, Leonardo, etc until serial console opens
//  }
//  
//  Serial.println("LSM9DS1 data read demo");
//  
//  // Try to initialise and warn if we couldn't detect the chip
//  if (!lsm.begin())
//  {
//    Serial.println("Oops ... unable to initialize the LSM9DS1. Check your wiring!");
//    while (1);
//  }
//  Serial.println("Found LSM9DS1 9DOF");
//
//  // helper to just set the default scaling we want, see above!
//  setupSensor();

  setupmax7456();
 

//  osd.init(10);
//  osd.setDisplayOffsets(60, 18);
//  osd.setBlinkParams(_8fields, _BT_BT);
//
//  osd.activateOSD();
//  osd.printMax7456Char(0x7a, 0, 12);
//  osd.print("makabra w moim miescie", 1, 2);
//  osd.print("zacznie sie za :", 1, 4);
//  osd.print("0",12,8);
//
//  osd.printMax7456Char(0xD1, 9, 6, true);
//  osd.printMax7456Chars(tab, 2, 2, 7);
//  pinMode(redLed, OUTPUT);
//  pinMode(greenLed, OUTPUT);

  //base time = 160ms,time on = time off.


decodePPM();
}



void loop() {

decodePPM();

 if ((millis()-tfail)>500){
    Serial.println("Disconnect");
  } else {
//    Serial.print(chx[0][val]);Serial.print("\t");
//    Serial.print(chx[1][val]);Serial.print("\t");
//    Serial.print(chx[2][val]);Serial.print("\t");
//    Serial.print(chx[3][val]);Serial.print("\t");
//    Serial.print(chx[4][val]);Serial.print("\t"); 
//    Serial.print(chx[5][val]);Serial.print("\t");
//    Serial.print(chx[6][val]);Serial.print("\t");
//    Serial.print(chx[7][val]);Serial.print("\t");
//    Serial.println(chx[8][val]);Serial.print("\t");

  lsm.read();  /* ask it to read in the data */ 

  /* Get a new sensor event */ 
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
 
motorfront.attach(10,1000,2000);
motorleft.attach(8,1000,2000) ;
motorright.attach(7,1000,2000);
motorcamera.attach(11,500,2500);
motordol.attach(9,500,2500);
motorgora.attach(6,500,2500);
motorprawo.attach(5,500,2500);
motorlewo.attach(3,500,2500);

valSTOP = map(chx[10][val], 1000, 2000, 0, 1);
valLLR = map(chx[4][val], 1000, 2000, 0, 180);
valRUP = map(chx[2][val], 1000, 2000, 0, 180);
valLUP = map(chx[3][val], 1000, 2000, 0, 180);
//valCAM = map(chx[5][val], 1000, 2000, 0, 180);
varCAM = map(chx[5][val], 1000, 2000, 0, 180);
varDOL = map(chx[6][val], 1000, 2000, 0, 180);
varGOR = map(chx[7][val], 1000, 2000, 0, 180);
varPRA = map(chx[8][val], 1000, 2000, 0, 180);
varLEW = map(chx[9][val], 1000, 2000, 0, 180);
//cameraset = map(chx[10][val], 1000, 2000, 0, 180);

//if (counter % 2 == 0) {
//    digitalWrite(redLed, LOW);
//    digitalWrite(greenLed, HIGH);
//  } else {
//    digitalWrite(redLed, HIGH);
//    digitalWrite(greenLed, LOW);
//  }
//
//  counter = millis() / 100;
//
//  osd.print(int(counter / 60), 10, 10, 3, 1, false, false);
//  osd.print(int(counter % 60), 13, 11, 2, 0, false, true);

  delay(100);



if (valSTOP == 1){
stopallmotors();
return;
}

steer();
valcheck();
}

void stopallmotors(){
motorfront.write(90);
motorcamera.write(90);
motorleft.write(90);
motorright.write(90);
motorlapa.write(90);
}

void steer(){
motorfront.write(180-valLUP);
motorleft.write(valRUP+(valLLR-90));
motorright.write(valRUP-(valLLR-90));

//switch(cameraset){
//case 0:
//motorcamera.write(180-valCAM);
//break;
//case 1:
conCAM = constrain(conCAM,0, 180);
conCAM=conCAM+(90-varCAM)/4;
motorcamera.write(conCAM);
//break;
//default:
//Serial.println("CAMERA ERROR");
//break;
//}

conDOL = constrain(conDOL,35, 131);
conDOL=conDOL+(90-varDOL)/4;
motordol.write(conDOL);

conGOR = constrain(conGOR,60, 100);
conGOR=conGOR+(90-varGOR)/4;
motorgora.write(conGOR);

conPRA = constrain(conPRA,35, 131);
conPRA=conPRA+(90-varPRA)/4;
motorprawo.write(conPRA);

conLEW = constrain(conLEW,35, 131);
conLEW=conLEW+(90-varLEW)/4;
motorlewo.write(conLEW);
}

void valcheck(){
Serial.print(" left : ");
Serial.print(valRUP+(valLLR-90));
Serial.print(" right : ");
Serial.print(valRUP-(valLLR-90));
Serial.print(" front : ");
Serial.print(180-valLUP);
Serial.print(" camera : ");
//Serial.print(180-valCAM);
Serial.print(conCAM);
Serial.print(" dol : ");
Serial.print(conDOL);
Serial.print(" gora : ");
Serial.print(conGOR);
Serial.print(" prawo : ");
Serial.print(conPRA);
Serial.print(" lewo : ");
Serial.println(conLEW);

delay(3000);
}
