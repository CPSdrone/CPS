#include <PID_v1.h>

#include <Servo.h>

//Define Variables we'll be connecting to
double Setpoint[2];//ghost thrust [prob 0]
double Input[7];//error
double Output[7];//thrust
double InStored[7];//stored error for writing
double Out[7];//stored Output

//Define the aggressive and conservative Tuning Parameters
double aggKp=1, aggKi=0, aggKd=0;// aggKi=0.01, aggKd=0.01
double consKp=0.2, consKi=0.05, consKd=0.1;// consKi=0.001, consKd=0.001;

PID motorPID[]={
PID(&Input[0], &Output[0], &Setpoint[0], aggKp, aggKi, aggKd, DIRECT),
PID(&Input[1], &Output[1], &Setpoint[1], aggKp, aggKi, aggKd, DIRECT),
PID(&Input[2], &Output[2], &Setpoint[2], aggKp, aggKi, aggKd, DIRECT),
PID(&Input[3], &Output[3], &Setpoint[3], aggKp, aggKi, aggKd, DIRECT),
PID(&Input[4], &Output[4], &Setpoint[4], aggKp, aggKi, aggKd, DIRECT),
PID(&Input[5], &Output[5], &Setpoint[5], aggKp, aggKi, aggKd, DIRECT),
};

#define motorApin 12
#define motorBpin 6
#define motorCpin 8
#define motorRpin 7
#define motorLpin 6

Servo motorA;
Servo motorB;
Servo motorC;
Servo motorR;
Servo motorL;

int valLUP;
int valRUP;
int valLLR;
int valRLR=0;

int stick;

void setupPID()
{
  
  //initialize the variables we're linked to
  Input[0] = 0;
  Input[1] = 0;
  Input[2] = 0;
  Input[3] = 0;
  Input[4] = 0;
  Input[5] = 0;
  
  Setpoint[0] = 90;
  Setpoint[1] = 0;

  //turn the PID on
  for(byte i=0; i<=5; i++){
  motorPID[i].SetMode(AUTOMATIC);
  motorPID[i].SetOutputLimits(-90,90);
  Serial.print(i);}
}

void runPID(float compRoll, float compPitch, float compYaw, float accX, float accY, float accZ){

  //Serial.println(compRoll);
  
  motorA.attach(motorApin,1000,2000);
  motorB.attach(motorBpin,1000,2000);
  motorC.attach(motorCpin,1000,2000);
  motorR.attach(motorRpin,840,2040);
  motorL.attach(motorLpin,1080,2280);

  valLLR = map(ch[4], 1000, 2000, 0, 180);
  valRUP = 0;//map(ch[2], 1000, 2000, 0, 180);
  valLUP = 0;//map(ch[3], 1000, 2000, 0, 180);
  stick=analogRead(0);
  valRLR = map(stick, 0, 1023, 0, 180);

  
  Input[0] = compRoll + valRLR;
  Input[1] = compPitch + sqrt(valRUP*valLUP);
  Input[2] = compYaw + valLLR;
  Input[3] = accX + valRUP;
  Input[4] = 0; //accY + no stick available
  Input[5] = accZ - (valLUP*(-abs(valRUP)+90));


  /*for(byte i=1; i<=6; i++){  //do for all PIDs
    double gap = abs(Setpoint[1]-Input[i]); //distance away from setpoint
    if (gap < 10)
    {  //we're close to setpoint, use conservative tuning parameters
    motorPID[i].SetTunings(consKp, consKi, consKd);
    }else{
     //we're far from setpoint, use aggressive tuning parameters
     motorPID[i].SetTunings(aggKp, aggKi, aggKd);
     }
  }*/

  for(byte i=0; i<=5; i++){
    motorPID[i].Compute();

    Out[i]=Output[i]+90;
  }
    
      motorA.write((Out[0]-Out[1]+Out[5])/3);
      motorB.write((-Out[0]-Out[1]+Out[5])/3);
      motorC.write((Out[1]+Out[5])/2);
      motorR.write((Out[2]+Out[3])/2);
      motorL.write((-Out[2]+Out[3])/2);
  

      printmotors(Out[0], Out[1], Out[2], Out[3], Out[4], Out[5],accX,accY,accZ);
}

void printmotors(double roll, double pitch, double yaw, double x, double y, double z,double accX,double accY,double accZ){

  Serial.print("\t");
  Serial.print("A ");
  Serial.print(roll);
  Serial.print(" ");
  Serial.print(pitch);
  //Serial.print((roll-pitch+z)/3);
  Serial.print("\t");
  Serial.print("B ");
  Serial.print(-roll+180);
  Serial.print(" ");
  Serial.print(-pitch+180);
  //Serial.print((-roll-pitch+z)/3);
  Serial.print("\t");
  Serial.print("C ");
  Serial.print((pitch+z)/2);
  Serial.print("\t");
  Serial.print("R ");
  Serial.print((yaw+x)/2);
  Serial.print("\t");
  Serial.print("L ");
  Serial.print((-yaw+x)/2);

  Serial.print("\t\t\t");
  //Serial.pritln();
  
  Serial.print("Pitch ");
  Serial.print(accPitch);
  Serial.print(" ");
  Serial.print(compPitch);
  Serial.print("\t\t");
  Serial.print("Roll ");
  Serial.print(accRoll);
  Serial.print(" ");
  Serial.print(compRoll);
  Serial.print("\t");
  Serial.print("Yaw ");
  Serial.print(accmagYaw);
  Serial.print(" ");
  Serial.print(compYaw);
  Serial.print("\t");
  Serial.print("x ");
  Serial.print(accX);
  Serial.print("\t");
  Serial.print("y ");
  Serial.print(accY);
  Serial.print("\t");
  Serial.print("z ");
  Serial.print(accZ);

  Serial.println(); 

  //double tiger = temp.temperature / 210 + 20 ;
  //Serial.print("Temp: "); Serial.print(tiger); Serial.println(" celcius");
  
  delay(00);
}
