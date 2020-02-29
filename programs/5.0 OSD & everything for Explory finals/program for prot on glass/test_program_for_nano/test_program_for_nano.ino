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

int valLUP;
int valRUP;
int valLLR;
int valRLR;


/////////////////////////////////MODULES PRE SETUP/////////////////////////////////
Servo motorcamera;
Servo motorlapa;
Servo motorprobe;

float valbat;

float valup;
float valright;
float valdown;
float valleft;

float voltup;
float voltright;
float voltdown;
float voltleft;

int varCAM;
int conCAM = 90;
int varLAP;
int conLAP = 90;
int varPRO;
int conPRO = 180;

int lapapin;
int probepin;


int pHpin = 5;
float calibration = 22.25;
int pHreading = 0;
unsigned long int avgValue;
int buf[10],temper;

/////////////////////////////////OSD PRE SETUP/////////////////////////////////

#include <SPI.h>
#include <max7456.h>
Max7456 osd;

unsigned long counter = 0;

float volt;
//int clearval = 1;

/////////////////////////////////IMU PRE SETUP/////////////////////////////////

#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>  // not used in this demo but required!

Adafruit_LSM9DS1 lsm = Adafruit_LSM9DS1();

#define LSM9DS1_SCK A5
#define LSM9DS1_MISO 12
#define LSM9DS1_MOSI A4
#define LSM9DS1_XGCS 6
#define LSM9DS1_MCS 5


/////////////////////////////////THERMOMETER PRE SETUP/////////////////////////////////

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 3
#define TEMPERATURE_PRECISION 12

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int numberOfDevices;
DeviceAddress tempDeviceAddress;

int vartemp;
int contemp;
//DeviceAddress deviceAddress;
//
//DeviceAddress printAddress;


///////////////////////////////////////LED/////////////////////////////////////////

//int ledPinGreen = 9;
//int ledPinRed = 11;

int ledlights = 8;
int vallights;
int valBlink;

int conlights;











void OSDstart() {
  SPI.begin();
  
    osd.init(10);
    osd.setDisplayOffsets(40,50);
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






void thermometerSetup(void){
  Serial.println("Dallas Temperature IC Control Library Demo");
  sensors.begin();
  
  numberOfDevices = sensors.getDeviceCount();
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");
  
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");
  
  for(int i=0;i<numberOfDevices; i++)
  {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i)){
      Serial.print("Found device ");
      Serial.print(i, DEC);
      Serial.print(" with address: ");
      printAddress(tempDeviceAddress);
      Serial.println();
    
      Serial.print("Setting resolution to ");
      Serial.println(TEMPERATURE_PRECISION, DEC);
    
      // set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions)
      sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);
    
      Serial.print("Resolution actually set to: ");
      Serial.print(sensors.getResolution(tempDeviceAddress), DEC); 
      Serial.println();
    }else{
      Serial.print("Found ghost device at ");
      Serial.print(i, DEC);
      Serial.print(" but could not detect address. Check power and cabling");
    }
  }
}




void ledStart(){
//pinMode(ledPinGreen, OUTPUT);
//pinMode(ledPinRed, OUTPUT);
pinMode(ledlights, OUTPUT);
  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 1) {
    //analogWrite(ledPinGreen, fadeValue);
    //analogWrite(ledPinRed, fadeValue);
    analogWrite(ledlights, fadeValue);
    delay(10);
  }
/*
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
*/
}








void setup() {
Serial.begin(115200);
pinMode(RECEIVE_PIN, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(RECEIVE_PIN), ppm_interrupt, METHOD);
                                                                                  ///////////////////////////////
OSDstart();
//IMUsetup();
thermometerSetup();
ledStart();
}










void loop() {
ppm_write();

//Serial.print(ch[1]);Serial.print("\t");
//Serial.print(ch[2]);Serial.print("\t");
//Serial.print(ch[3]);Serial.print("\t");
//Serial.print(ch[4]);Serial.print("\t");
//Serial.print(ch[5]);Serial.print("\t");
//Serial.print(ch[6]);Serial.print("\n");
                                                                     /////////////                              //////////
counter = millis()/1000;
                                                                    
analogreading();

steer();

//      vartemp = map(ch[4], 1000, 2000, 0, 180);
//      
//      contemp = constrain(contemp, 0, 180);
//      contemp = contemp+(90-vartemp)/2;

  //Serial.print(contemp);
  //if(contemp>10){
    //temperatureGet();
  //}
  
ledRun();
OSDrun();

//valcheck();
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
  //valbat=analogRead(A2);
  
  valup=analogRead(A0);
  valright=analogRead(A2);
  valdown=analogRead(A1);
  valleft=analogRead(A3);

  //volt = valbat * 5.0/1023;
  
  voltup = valup * 5.0/1023;  
  voltright = valright * 5.0/1023;
  voltdown = valdown * 5.0/1023;
  voltleft = valleft * 5.0/1023;

//  Serial.print("volt ");
//  Serial.print(volt);
//  Serial.print(" voltup ");
//  Serial.print(voltup);
//  Serial.print(" voltright ");
//  Serial.print(voltright);
//  Serial.print(" voltdown ");
//  Serial.print(voltdown);
//  Serial.print(" voltleft ");
//  Serial.print(probepin);
//  Serial.println(voltleft);
}






void steer(){
if(voltup>3.3 && voltup<3.5) lapapin = 5;
else if (voltup>4.45 && voltup<4.65) probepin = 5;
else if (voltup>2.4 && voltup<2.6){ 
  pHpin = A5;
  pHget();
}

//if(voltright>3.3 && voltright<3.5) lapapin = 10;
//else if (voltright>4.45 && voltright<4.65) probepin = 10;
//else if (voltright>2.4 && voltright<2.6){
//  pHpin = A6;
//  pHget();
//}
//
//if(voltdown>3.3 && voltdown<3.5) lapapin = 11;
//else if (voltdown>4.45 && voltdown<4.65) probepin = 11;
//else if (voltdown>2.4 && voltdown<2.6){
//  pHpin = A7;
//  pHget();
//}
//
//if(voltleft>3.3 && voltleft<3.5) lapapin = 12;
//else if (voltleft>4.45 && voltleft<4.65) probepin = 12;
//else if (voltleft>2.4 && voltleft<2.6){
//  pHpin = A8;
//  pHget();
//}

              motorcamera.attach(3,500,2500);
              motorlapa.attach(lapapin,1000,2000);
              motorprobe.attach(probepin,1000,2000);
            
              varCAM = map(ch[5], 1000, 2000, 0, 180);
              varLAP = map(ch[6], 1000, 2000, 0, 180);
              varPRO = map(ch[7], 1000, 2000, 0, 180);


      conCAM = constrain(conCAM,0, 180);
      conCAM=conCAM+(90-varCAM)/26;
      motorcamera.write(conCAM);

      conLAP = constrain(conLAP,0, 180);
      conLAP=conLAP+(90-varLAP)/50;
      motorlapa.write(varLAP);
      
      conPRO = constrain(conPRO,0, 180);
      conPRO=conPRO+(90-varPRO)/2;
      motorprobe.write(conPRO);
}




void pHget(){
 
 for(int i=0;i<10;i++) 
 { 
 buf[i]=analogRead(pHpin);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(buf[i]>buf[j])
 {
 temper=buf[i];
 buf[i]=buf[j];
 buf[j]=temper;
 }
 }
 }
 avgValue=0;
 for(int i=2;i<8;i++)
 avgValue+=buf[i];
 float pHVol=(float)avgValue*5.0/1024/6;
 float phValue = -5.70 * pHVol + calibration;
 Serial.print("sensor = ");
 Serial.println(phValue);
 
 delay(1);
}















void temperatureGet(void){
// call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  
  
  // Loop through each device, print out temperature data
  for(int i=0;i<numberOfDevices; i++)
  {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i))
  {
    // Output the device ID
    Serial.print("Temperature for device: ");
    Serial.println(i,DEC);
    
    // It responds almost immediately. Let's print out the data
    printTemperature(tempDeviceAddress); // Use a simple function to print out the data
//    float tempC = sensors.getTempC(deviceAddress);
//  osd.print(tempC, 23, 3, 2, 1, false, true);
//  osd.printMax7456Char(0xD0, 27, 3);
//  Serial.print("Temp C: ");
//  Serial.print(tempC);
  } 
  //else ghost device! Check your power requirements and cabling
  
  }
}





void printTemperature(DeviceAddress deviceAddress) 
{
  // method 1 - slower
  //Serial.print("Temp C: ");
  //Serial.print(sensors.getTempC(deviceAddress));
  //Serial.print(" Temp F: ");
  //Serial.print(sensors.getTempF(deviceAddress)); // Makes a second call to getTempC and then converts to Fahrenheit

  // method 2 - faster
  float tempC = sensors.getTempC(deviceAddress);
  osd.print(tempC, 23, 3, 2, 1, false, true);
  osd.printMax7456Char(0xD0, 27, 3);
  Serial.print("Temp C: ");
  Serial.print(tempC);
}





void printAddress(DeviceAddress deviceAddress){
  for (uint8_t i = 0; i < 8; i++){
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}






void ledRun(){
      vallights = map(ch[8], 1000, 2000, 0, 100);
    
      Serial.println(vallights);

  if(vallights>90) analogWrite(ledlights,255);
  else if(vallights>30) analogWrite(ledlights,70);
  else if(vallights>=0) analogWrite(ledlights,0);
  }

/*  if((valBlink=0)&&(micros()%0)) digitalWrite(ledlights, LOW);

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
*/




void OSDrun(){

osd.printMax7456Char(0x90,4,1,false,false);
osd.print("100",0,1,false,true);
osd.printMax7456Char(0x81,3,1,false,true);

//osd.print("throt",0,15);
//osd.print(max((valRUP+(valLLR-90)),(valRUP-(valLLR-90))),16,1,1,0,false,true);

//osd.print("temp", 24, 3);
//osd.print(tempC, 23, 3, 2, 1, false, true);
//osd.printMax7456Char(0xD0, 27, 3);
//in the temperature function

osd.print(int(counter%60), 15, 1, 2, 0, false, true);
osd.print(":", 14, 1, false, true);
osd.print(int(counter/60), 12, 1, 2, 0, false, true);

osd.printMax7456Char(0xF5, 22, 1);
osd.printMax7456Char(0xF6, 23, 1);
osd.print(vallights, 24, 1, 2, 1, false, true);

if((voltup>3.3)&&(voltup<3.5)||(voltright>3.3)&&(voltright<3.5)||(voltdown>3.3)&&(voltdown<3.5)||(voltleft>3.3)&&(voltleft<3.5)){
  osd.print("arm", 21, 13);
  osd.print(conLAP, 25, 13, 3, 0, false, true);                                                                                               //arrow needed
}else{
  for(int f = 21; f <= 27; f++){
  osd.printMax7456Char(0x00,f,13);
  }
}

if((voltup>4.45)&&(voltup<4.65)||(voltright>4.45)&&(voltright<4.65)||(voltdown>4.45)&&(voltdown<4.65)||(voltleft>4.45)&&(voltleft<4.65)){
  osd.print("pro", 21, 11);
  osd.print(conPRO, 25, 11, 3, 0, false, true);
}else{
  for(int f = 21; f <= 27; f++){
  osd.printMax7456Char(0x00,f,11);
  }
}

if((voltup>2.4)&&(voltup<2.6)||(voltright>2.4)&&(voltright<2.6)||(voltdown>2.4)&&(voltdown<2.6)||(voltleft>2.4)&&(voltleft<2.6)){
  osd.print("pH", 21, 12);
  osd.print(analogRead(pHpin), 24, 12, 2, 1, false, true);
}else{
  for(int f = 21; f <= 27; f++){//                                idk if f can be for all
  osd.printMax7456Char(0x00,f,12);
  }
}
}










void valcheck(){
    //Serial.print(" camera : ");
    //Serial.print(conCAM);
    Serial.print(" lapa : ");
    Serial.print(varLAP);
    //Serial.print(" probe : ");
    //Serial.println(conPRO);
//
//  delay(3000);
}
