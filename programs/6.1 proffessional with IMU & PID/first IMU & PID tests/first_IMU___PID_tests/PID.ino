#include <PID_v1.h>

#include <Servo.h>
Servo motorA;
Servo motorB;
Servo motorC;
Servo motorright;
Servo motorleft;

//Define Variables we'll be connecting to
double Setpoint[2];//ghost thrust [prob 0]
double Input[7];//error
double Output[7];//thrust

//Define the aggressive and conservative Tuning Parameters
double aggKp=4, aggKi=0.2, aggKd=1;
double consKp=1, consKi=0.05, consKd=0.25;

PID motorPID[]={
PID(&Input[1], &Output[1], &Setpoint[1], consKp, consKi, consKd, DIRECT),
PID(&Input[2], &Output[2], &Setpoint[1], consKp, consKi, consKd, DIRECT),
PID(&Input[3], &Output[3], &Setpoint[1], consKp, consKi, consKd, DIRECT),
PID(&Input[4], &Output[4], &Setpoint[1], consKp, consKi, consKd, DIRECT),
PID(&Input[5], &Output[5], &Setpoint[1], consKp, consKi, consKd, DIRECT),
PID(&Input[6], &Output[6], &Setpoint[1], consKp, consKi, consKd, DIRECT),
};

void setup()
{
  //initialize the variables we're linked to
  Input[1] = 0;//compAngleX + valRLR;
  Input[2] = 0;//compAngleY + valRUP*valLUP;
  Input[3] = 0;//compAngleZ + valLLR;
  Input[4] = 0;//a.acceleration.x*dt + valRUP;//not sure if *dt needed
  Input[5] = 0; //a.acceleration.y*dt + no stick available
  Input[6] = 0;//a.acceleration.z*dt - valLUP*(-abs(valRUP)+90);
  
  Setpoint[0] = 10;
  Setpoint[1] = 0;

  //turn the PID on
  for(byte i; i<=6; i++){
  motorPID[i].SetMode(AUTOMATIC);}
}

void loop()
{
  Input[1] = 0;//compAngleX + valRLR;
  Input[2] = 0;//compAngleY + valRUP*valLUP;
  Input[3] = 0;//compAngleZ + valLLR;
  Input[4] = 0;//a.acceleration.x*dt + valRUP;
  Input[5] = 0; //a.acceleration.y*dt + no stick available
  Input[6] = 0;//a.acceleration.z*dt - valLUP*(-abs(valRUP)+90);

  for(byte i; i<=6; i++){  //do for all PIDs
    double gap = abs(Setpoint[1]-Input[i]); //distance away from setpoint
    if (gap < 10)
    {  //we're close to setpoint, use conservative tuning parameters
    motorPID[i].SetTunings(consKp, consKi, consKd);
    }else{
     //we're far from setpoint, use aggressive tuning parameters
     motorPID[i].SetTunings(aggKp, aggKi, aggKd);
     }
  }
  
  for(byte i; i<=6; i++){
  motorPID[i].Compute();}
  
  motorA.write((Output[1]-Output[2]+Output[6])/3);
  motorB.write((-Output[1]-Output[2]+Output[6])/3);
  motorC.write((Output[2]+Output[6])/2);
  motorright.write((Output[3]+Output[4])/2);
  motorleft.write((-Output[3]+Output[4])/2);
}
