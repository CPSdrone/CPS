//////PPM READING PRE SETUP/////

#define RECEIVE_PIN 2
#define CHANNEL_AMOUNT 6
#define DETECTION_SPACE 2500
#define METHOD RISING

int ch[CHANNEL_AMOUNT + 1];

//////FOR ALL GAMERS OUT THERE/////
#include <Servo.h>;

#define motorApin 8
#define motorBpin 9
#define motorCpin 10
#define motorRpin 11
#define motorLpin 12

Servo motorA;
Servo motorB;
Servo motorC;
Servo motorR;
Servo motorL;

int valLUP;
int valRUP;
int valLLR;
int valRLR;

void setup() {
  Serial.begin(115200);

  pinMode(RECEIVE_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(RECEIVE_PIN), ppm_interrupt, METHOD);
}

void loop() {
ppm_write();
steer();
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
  motorA.attach(motorApin,805,2075);//135
  motorB.attach(motorBpin,1040,2310);//135
  motorC.attach(motorCpin,1030,2300);//135
  motorR.attach(motorRpin,1185-135,2185+135);
  motorL.attach(motorLpin,945-135,1945+135);

  valLLR = map(ch[4], 1000, 2000, 90, -90);
  valRUP = map(ch[2], 1000, 2000, -90, 90); //900,1100
  valLUP = map(ch[3], 1000, 2000, -90, 90);
  valRLR = map(ch[1], 1000, 2000, -90, 90);

  motorA.write(valRUP+90);   
  motorB.write(valRUP+90);
  motorC.write(valRUP+90);
  motorR.write(valRUP+90);
  motorL.write(valRUP+90);

  Serial.print(ch[1]);
  Serial.print("\t");
  Serial.print(ch[2]);
  Serial.print("\t");
  Serial.print(valRUP+90);
  Serial.println("\t");
}
