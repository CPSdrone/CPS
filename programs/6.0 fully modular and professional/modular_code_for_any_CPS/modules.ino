Servo motorlapa;
Servo motorprobe;

#define calibration 22.25

int pHreading = 0;
unsigned long int avgValue;
int buf[10],temper;


float valup;
float valright;
float valdown;
float valleft;

float voltup;
float voltright;
float voltdown;
float voltleft;

int varLAP;
int conLAP = 90;
int varPRO;
int conPRO = 180;

int lapapin;
int probepin;
int pHpin;





void modulerun(){
  
  valup=analogRead(A0);
  valright=analogRead(A2);
  valdown=analogRead(A1);
  valleft=analogRead(A3);

  voltup = valup * 5.0/1023;  
  voltright = valright * 5.0/1023;
  voltdown = valdown * 5.0/1023;
  voltleft = valleft * 5.0/1023;

    if(voltup>3.3 && voltup<3.5) lapapin = 5;
    else if (voltup>4.45 && voltup<4.65) probepin = 5;
    else if (voltup>2.4 && voltup<2.6){ 
      pHpin = A5;
    }
    /*
    if(voltright>3.3 && voltright<3.5) lapapin = 10;
    else if (voltright>4.45 && voltright<4.65) probepin = 10;
    else if (voltright>2.4 && voltright<2.6){
      pHpin = A6;
    }
    
    if(voltdown>3.3 && voltdown<3.5) lapapin = 11;
    else if (voltdown>4.45 && voltdown<4.65) probepin = 11;
    else if (voltdown>2.4 && voltdown<2.6){
      pHpin = A7;
    }
    
    if(voltleft>3.3 && voltleft<3.5) lapapin = 12;
    else if (voltleft>4.45 && voltleft<4.65) probepin = 12;
    else if (voltleft>2.4 && voltleft<2.6){
      pHpin = A8;
    }
    */

      varLAP = map(ch[6], 1000, 2000, 0, 180);
      varPRO = map(ch[7], 1000, 2000, 0, 180);
    
      motorlapa.attach(lapapin,1000,2000);
      motorprobe.attach(probepin,1000,2000);

      conLAP = constrain(conLAP,0, 180);
      conLAP=conLAP+(90-varLAP)/50;
      motorlapa.write(varLAP);
      
      conPRO = constrain(conPRO,0, 180);
      conPRO=conPRO+(90-varPRO)/2;
      motorprobe.write(conPRO);

      pHget();                                    // need to pass pHpin value

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
    for(int f = 21; f <= 27; f++){
    osd.printMax7456Char(0x00,f,12);
    }
  }
}//module run






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
