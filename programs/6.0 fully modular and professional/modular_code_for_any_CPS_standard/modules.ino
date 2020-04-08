/*Servo motorlapa;
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

#define UL 2.7
#define DL 2.5
#define UP 4.3
#define DP 4.1
#define UH 1.8
#define DH 1.6   

int varLAP;
int conLAP;
int varPRO;
int conPRO;

int lapapin;
int probepin;
int pHpin;





void modulerun(){
  
  valup=analogRead(A6);
  valright=analogRead(A0);
  valdown=analogRead(A4);
  valleft=analogRead(A2);

  voltup = valup * 5.0/1023;  
  voltright = valright * 5.0/1023;
  voltdown = valdown * 5.0/1023;
  voltleft = valleft * 5.0/1023;

    if(voltup>DL && voltup<UL) lapapin = 9;
    else if (voltright>DL && voltright<UL) lapapin = 4;
    else if (voltdown>DL && voltdown<UL) lapapin = 8;
    else if (voltleft>DL && voltleft<UL) lapapin = 7;
    else lapapin = 0;

    if(voltup>DP && voltup<UP) probepin = 9;
    else if (voltright>DP && voltright<UP) probepin = 4;
    else if (voltdown>DP && voltdown<UP) probepin = 8;
    else if (voltleft>DP && voltleft<UP) probepin = 7;
    else probepin = 0;

    if(voltup>DH && voltup<UH) pHpin = A7;
    else if (voltright>DH && voltright<UH) pHpin = A1;
    else if (voltdown>DH && voltdown<UH) pHpin = A5;
    else if (voltleft>DH && voltleft<UH) pHpin = A3;
    else pHpin = 0;

      varLAP = map(ch[6], 1000, 2000, 0, 180);
      varPRO = map(ch[7], 500, 2500, 0, 180);
    
      motorlapa.attach(lapapin,1000,2000);
      motorprobe.attach(probepin,1000,2000);

      conLAP = constrain(conLAP,0, 180);
      conLAP=conLAP+(90-varLAP)/50;
      motorlapa.write(varLAP);
      
      conPRO = constrain(conPRO,0, 175);
      conPRO=conPRO+(90-varPRO)/10;
      motorprobe.write(conPRO);

      pHget();                                  // need to pass pHpin value

  if((voltup>DL)&&(voltup<UL)||(voltright>DL)&&(voltright<UL)||(voltdown>DL)&&(voltdown<UL)||(voltleft>DL)&&(voltleft<UL)){
    osd.print("arm", 0, 13);
      if(varLAP>100){
        osd.printMax7456Char(0xD9,4,13,false,true);
        osd.printMax7456Char(0xDD,5,13,false,true);
      }else if (varLAP<80){
        osd.printMax7456Char(0xDD,4,13,false,true);
        osd.printMax7456Char(0xD9,5,13,false,true);
      }else if (varLAP=90){
        osd.printMax7456Char(0xCE,4,13,false,true);
        osd.printMax7456Char(0xCE,5,13,false,true);
      }
  }else{
    for(int f = 0; f <= 6; f++){
    osd.printMax7456Char(0x00,f,13);
    }
  }
  
  if((voltup>DP)&&(voltup<UP)||(voltright>DP)&&(voltright<UP)||(voltdown>DP)&&(voltdown<UP)||(voltleft>DP)&&(voltleft<UP)){
    osd.print("pro", 0, 11);
      if(conPRO>175){
        osd.print("closed", 4, 11, false, true);  
      }else{
        osd.print("open", 4, 11, false, true);
        osd.printMax7456Char(0x00,8,11);
        osd.printMax7456Char(0x00,9,11);
      }                                         //pojemnik
  }else{
    for(int f = 0; f <= 10; f++){
    osd.printMax7456Char(0x00,f,11);
    }
  }
  
//  printmodules();
}






void pHget(){
 
 for(int i=0;i<10;i++) 
 { 
 buf[i]=analogRead(pHpin);
 delay(0);
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

    Serial.print("pH:");
    Serial.print("\t");
    Serial.print(phValue);
    Serial.print("\t");
    Serial.println(pHpin);
    delay(000);
  

 
   if((voltup>DH)&&(voltup<UH)||(voltright>DH)&&(voltright<UH)||(voltdown>DH)&&(voltdown<UH)||(voltleft>DH)&&(voltleft<UH)){
    osd.print("pH", 21, 13);
    osd.print(phValue, 24, 13, 2, 1, false, true);
  }else{
    for(int f = 21; f <= 27; f++){
    osd.printMax7456Char(0x00,f,13);
    }
  }
}



void printmodules(){
  Serial.print("\t");
  Serial.print("voltup: ");
  Serial.print(voltup);
  Serial.print("\t");
  Serial.print("voltright: ");
  Serial.print(voltright);
  Serial.print("\t");
  Serial.print("voltdown: ");
  Serial.print(voltdown);
  Serial.print("\t");
  Serial.print("volleft: ");
  Serial.println(voltleft);
  Serial.print("lapa: ");
  Serial.print(varLAP);
  Serial.print("\t");
  Serial.println(lapapin);
  Serial.print("probe: ");
  Serial.print(conPRO);
  Serial.print("\t");
  Serial.println(probepin);
  delay(000);
}*/
