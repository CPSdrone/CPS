unsigned long int ft, lt, x, tfail;
int ch, chx[9][13];
const int idx   = 10;
const int total = 11;
const int val   = 12; //End of specifying the integers for ppm signal reading 

int valLUP;
int valRUP;
int valLLR;
int valCAM;
int valMT;
int valMT2;

#include <Servo.h>;
Servo motorfront;
Servo motorleft;
Servo motorright;
Servo motorcamera;

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
void loop() {
 if ((millis()-tfail)>500) {
    Serial.println("Disconnect");
  } else {
    Serial.print(chx[0][val]);Serial.print("\t");
    Serial.print(chx[1][val]);Serial.print("\t");
    Serial.print(chx[2][val]);Serial.print("\t");
    Serial.print(chx[3][val]);Serial.print("\t");
    Serial.print(chx[4][val]);Serial.print("\t"); 
    Serial.print(chx[5][val]);Serial.print("\t");
    Serial.print(chx[6][val]);Serial.print("\t");
    Serial.print(chx[7][val]);Serial.print("\t");
    Serial.println(chx[8][val]);Serial.print("\t");
 //delay(6000);
 }
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
  }                                              //end of code for ppm reading

motorfront.attach(3,1000,2000);                 //beggining of the code for controling motors 
motorleft.attach(5,1000,2000) ;
motorright.attach(6,1000,2000);
motorcamera.attach(5,1000,2000);
 
//valLUP = chx[3][val]; //LUP - left up down,   RUP - right up down,   LLR - Left left right,   RLR - Right left right 
//valRUP = chx[2][val];
//valLLR = chx[4][val]; 

valLLR = map(chx[4][val], 1000, 2000, 0, 180);
valRUP = map(chx[2][val], 1000, 2000, 0, 180);
valLUP = map(chx[3][val], 1000, 2000, 0, 180);
valCAM = map(chx[5][val], 1000, 2000, 0, 180);

if ((valLLR < 100) && (valLLR > 80)) {
 motorright.write(valRUP);
}
if ((valLLR < 100) && (valLLR > 80)){
  motorleft.write(valRUP);
}
  motorfront.write(valLUP);


valMT = (0.5 * (valLLR - 90)); // There is a multiplier in this line, which is responisble for sesnitivity of turning the drone while swimming forward 

if ((valLLR >= 100) && ((valRUP > 91) || (valRUP < 89)) && (valRUP <= 150)){   //Turning right         If I would change the multiplier, I would also need to change this 
motorleft.write(valLUP + valMT);
motorright.write(valLUP - valMT);
}

if ((valRUP > 150) && (valLLR >= 100)) {   //still turning right forwards
motorleft.write(180);
}

if ((valRUP < 30) && (valLLR >= 100)) {   //still turning right backwards
motorleft.write(0);
}

valMT2 = (0.5 * (90 - valLLR)); // There is a second multiplier in this line, which is responisble for sesnitivity of turning the drone while swimming forward 

if ((valLLR <= 80) && ((valRUP > 91) || (valRUP < 89)) && (valRUP >= 30)){   //Turning left         If I would change the multiplier, I would also need to change this 
motorleft.write(valLUP - valMT2);
motorright.write(valLUP + valMT2);
}

if ((valRUP > 150) && (valLLR <= 100)) {   //still turning left forwards
motorright.write(180);
}

if ((valRUP < 30) && (valLLR <= 100)) {   //still turning left backwards 
motorleft.write(0);
}

if(valCAM>90){
  motorcamera.write(140);
}
if(valCAM<90){
  motorcamera.write(40);
}

//Serial.print("shit");
//Serial.println(valMT);

}
