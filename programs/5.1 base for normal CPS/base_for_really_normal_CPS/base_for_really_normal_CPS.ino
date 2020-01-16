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






void setup() {
Serial.begin(115200);
pinMode(RECEIVE_PIN, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(RECEIVE_PIN), ppm_interrupt, METHOD);
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
steer();

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






void steer(){

  
              motorupdown.attach(12,1000,2000);
              motorleft.attach(10,1000,2000);
              motorright.attach(9,1000,2000);
              motorcamera.attach(8,500,2500);
              
              valLLR = map(ch[4], 1000, 2000, 0, 180);
              valRUP = map(ch[2], 1000, 2000, 0, 180);
              valLUP = map(ch[3], 1000, 2000, 0, 180);
              varCAM = map(ch[5], 1000, 2000, 0, 180);
              //valRLR = map(ch[1], 1000, 2000, 0, 180);
              
                     
      motorupdown.write(180-valLUP+30);
      //motorside.write(valRLR);
      motorleft.write(valRUP+(valLLR-90)+42);
      motorright.write(valRUP-(valLLR-90)+38);
      
      conCAM = constrain(conCAM,0, 180);
      conCAM=conCAM+(90-varCAM)/90;
      motorcamera.write(conCAM);

      //vallights = map(ch[10], 1000, 2000, 0, 180);
      //Serial.print(vallights);
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
