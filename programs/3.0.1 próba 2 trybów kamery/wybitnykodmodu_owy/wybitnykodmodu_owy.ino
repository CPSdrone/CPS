unsigned long int ft, lt, x, tfail;
int ch, chx[9][13];
const int idx   = 10;
const int total = 11;
const int val   = 12; //End of specifying the integers for ppm signal reading 

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
}




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
}                                              //end of code for ppm reading





void loop() {
 if ((millis()-tfail)>500) {
    Serial.println("Disconnect");
  } else {
    //Serial.print(chx[0][val]);Serial.print("\t");
    //Serial.print(chx[1][val]);Serial.print("\t");
    //Serial.print(chx[2][val]);Serial.print("\t");
    //Serial.print(chx[3][val]);Serial.print("\t");
    //Serial.print(chx[4][val]);Serial.print("\t"); 
    //Serial.print(chx[5][val]);Serial.print("\t");
    //Serial.print(chx[6][val]);Serial.print("\t");
    //Serial.print(chx[7][val]);Serial.print("\t");
    //Serial.println(chx[8][val]);Serial.print("\t");
   // delay(6000);
 }
motorfront.attach(10,1000,2000);                 //beggining of the code for controling motors 
motorleft.attach(8,1000,2000) ;
motorright.attach(7,1000,2000);
motorcamera.attach(11,500,2500);
motordol.attach(9,500,2500);
motorgora.attach(6,500,2500);
motorprawo.attach(5,500,2500);
motorlewo.attach(3,500,2500);
//valLUP = chx[3][val]; //LUP - left up down,   RUP - right up down,   LLR - Left left right,   RLR - Right left right 
//valRUP = chx[2][val];
//valLLR = chx[4][val]; 

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
Serial.print(conLEW);

//delay(3000);
}
