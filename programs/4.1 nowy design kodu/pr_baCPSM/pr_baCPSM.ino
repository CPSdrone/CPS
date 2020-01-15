unsigned long int ft, lt, x, tfail;
int ch, chx[9][13];
const int idx   = 10;
const int total = 11;
const int val   = 12; //End of specifying the integers for ppm signal reading 

int varCAM;
int conCAM = 90;


#include <Servo.h>;
Servo motorcamera;

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


void setup() {
  Serial.begin(9600);
  
  ft=0; lt=0; x=0; tfail=0; ch=0;
for (int i=0; i<9; i++) {
for (int j=0; j<13; j++) {
      chx[i][j]=0;
    }
  }
pinMode(2, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(2), decodePPM, FALLING);

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
//    delay(6000);
 }
 

motorcamera.attach(5,500,2500);

varCAM = map(chx[5][val], 1000, 2000, 0, 180);

steer();
valcheck();
}


void steer(){
conCAM = constrain(conCAM,0, 180);
conCAM=conCAM+(90-varCAM)/4;
motorcamera.write(conCAM);
}



void valcheck(){
Serial.print(" camera : ");
Serial.println(conCAM);
}
