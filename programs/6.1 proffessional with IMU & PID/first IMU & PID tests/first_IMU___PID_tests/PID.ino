//Define Variables we'll be connecting to
double Setpoint[6];//ghost thrust [prob 0]
double Input[6];//error
double Output[6];//thrust
double Stick[6];

//Define the aggressive and conservative Tuning Parameters
double aggKp=0.5, aggKi=0.1, aggKd=0;
double consKp=0.3, consKi=0, consKd=0;
PID motorPID[]={
PID(&Input[0], &Output[0], &Setpoint[0], aggKp, aggKi, aggKd, DIRECT),
PID(&Input[1], &Output[1], &Setpoint[1], aggKp, aggKi, aggKd, DIRECT),
PID(&Input[2], &Output[2], &Setpoint[2],   0.4,     0,     0, DIRECT),
PID(&Input[3], &Output[3], &Setpoint[3], aggKp, aggKi, aggKd, DIRECT),
PID(&Input[4], &Output[4], &Setpoint[4], aggKp, aggKi, aggKd, DIRECT),
PID(&Input[5], &Output[5], &Setpoint[5], aggKp, aggKi, aggKd, DIRECT),
};

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
int conLLR = 0;

int offYaw;

int FREEMODE;

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
  
  Setpoint[0] = 0;
  Setpoint[1] = 0;
  Setpoint[2] = 0;
  Setpoint[3] = 0;
  Setpoint[4] = 0;
  Setpoint[5] = 0;

  //turn the PID on
  for(byte i=0; i<=5; i++){
  motorPID[i].SetMode(AUTOMATIC);
  motorPID[i].SetOutputLimits(-90,90);
  Serial.print(i);}
}

void runPID(float compRoll, float compPitch, float compYaw, float accX, float accY, float accZ){

  //Serial.println(compRoll);

 FREEMODE = ch[6];
 if(FREEMODE>1500){
  compRoll = 0;
  compPitch = 0;
  compYaw = 0;
  accX = 0;
  accY = 0;
  accZ = 0;
  conLLR=valLLR;
 }

  motorA.attach(motorApin,805,2075);//135
  motorB.attach(motorBpin,1040,2310);//135
  motorC.attach(motorCpin,1030,2300);//135
  motorR.attach(motorRpin,1185-135,2185+135);
  motorL.attach(motorLpin,945-135,1945+135);

  valLLR = map(ch[4], 1000, 2000, -90, 90);
  valRUP = map(ch[2], 1000, 2000, -90, 90); //900,1100
  valLUP = map(ch[3], 1000, 2000, -90, 90);
  valRLR = map(ch[1], 1000, 2000, -90, 90);
  //stick=analogRead(0);
  //valRLR = map(stick, 0, 1023, 0, 180);

  

  if (1){
    Input[2] = 0;
    Stick[2] = valLLR;
  }else{
    if (conLLR<0) conLLR = 360;
    conLLR = (conLLR + valLLR/90)%360;
    Input[2] = conLLR-180 + compYaw;
    if (Input[2]>180) Input[2] = Input[2]-360;
    if (Input[2]<-180) Input[2] = Input[2]+360;//*/
    Stick[2] = 0;
  }//*/

  /*int OFFSET = ch[5];
  if (OFFSET>1500){
    offYaw=conLLR-180;
  }*/
  
  Input[0] = compRoll;
  Input[1] = compPitch;
  Input[3] = 0;// + accX;
  Input[4] = 0; //accY + no stick available
  Input[5] = 0;// + accZ;

  Stick[0] = valRLR;
  Stick[1] = map(valRUP*valLUP,-8100,8100,-90,90);
  
  Stick[3] = valRUP;
  Stick[4] = 0;
  Stick[5] = map(-valLUP*(abs(valRUP)-90),-8100,8100,-90,90);

    if (abs(Setpoint[2]-Input[2]) < 20)
    {       motorPID[2].SetTunings(consKp, consKi, consKd);
    }else{  motorPID[2].SetTunings(0.5, 0, 0);}//*/

  for(byte i=0; i<=5; i++){
    motorPID[i].Compute();

    /*if(abs(Output[i])<10){
      Output[i] = 0;
    }//*/
  }

    
      motorA.write(-Output[0]-(Output[1]/2)-(Output[5]/2)+Stick[0]+(Stick[1]/2)+(Stick[5]/2)+90);
      motorB.write(+Output[0]-(Output[1]/2)-(Output[5]/2)-Stick[0]+(Stick[1]/2)+(Stick[5]/2)+90);
      motorC.write(-(+Output[1]-Output[5]-Stick[1]+Stick[5])+90);
      motorR.write(-(-Output[2]+Output[3]+Stick[2]-Stick[3])+90);
      motorL.write(-(+Output[2]+Output[3]-Stick[2]-Stick[3])+90);//*/
    
      /*Serial.print(ch[3]);
      Serial.print("\t");
      Serial.print(ch[2]);
      Serial.print("\t");//*/
      printmotors(accX,accY,accZ);
}

void printmotors(float accX, float accY, float accZ){

  Serial.print("A ");
  Serial.print(-Output[0]-(Output[1]/2)+Output[5]+Stick[0]+(Stick[1]/2)+Stick[5]+90);
  Serial.print("\t");
  Serial.print("B ");
  Serial.print(+Output[0]-(Output[1]/2)+Output[5]-Stick[0]+(Stick[1]/2)+Stick[5]+90);
  Serial.print("\t");
  Serial.print("C ");
  Serial.print(+Output[1]+Output[5]-Stick[1]+Stick[5]+90);
  Serial.print("\t");
  Serial.print("R ");
  Serial.print(+Output[2]+Output[3]+Stick[2]-Stick[3]+90);
  Serial.print("\t");
  Serial.print("L ");
  Serial.print(-Output[2]+Output[3]-Stick[2]-Stick[3]+90);
  
  /*Serial.print("A ");
  Serial.print(roll+pitch+z+90);
  Serial.print("\t");
  Serial.print("B ");
  Serial.print(-roll+pitch+z+90);
  Serial.print("\t");
  Serial.print("C ");
  Serial.print(-pitch+z+90);
  Serial.print("\t");
  Serial.print("R ");
  Serial.print(yaw+x+90);
  Serial.print("\t");
  Serial.print("L ");
  Serial.print(-yaw+x+90);*/

  Serial.print("\t\t");
  //Serial.pritln();
  
  Serial.print("Pitch ");
  Serial.print(compPitch);
  Serial.print("\t");
  Serial.print("Roll ");
  Serial.print(compRoll);
  Serial.print("\t");
  Serial.print("Yaw ");
  Serial.print(compYaw);

  Serial.println(); 

  //double tiger = temp.temperature / 210 + 20 ;
  //Serial.print("Temp: "); Serial.print(tiger); Serial.println(" celcius");
  
  delay(00);
}
