Servo motorcamera;
#define camerapin 3
int varCAM;
int conCAM;

float valbat;
float volt;

#define ledlights 6
float vallights;
int osdlights;
float conLIGHT;
/*int ledPinGreen = 9;
int ledPinRed = 11;*/



void internalStart(){
  ledStart();
  //thermometerStart();
}

void ledStart(){
  /*pinMode(ledPinGreen, OUTPUT);
  pinMode(ledPinRed, OUTPUT);*/
  pinMode(ledlights, OUTPUT);
  
  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 1) {
  /*analogWrite(ledPinGreen, fadeValue);
  analogWrite(ledPinRed, fadeValue);*/
  analogWrite(ledlights, fadeValue);
  delay(10);
  }
}





void steerint(){
  readbat();
  steercamera();
  steerlights();
  //thermometerRun();
}

void readbat(){
  /*valbat=analogRead(A2);
  volt = valbat * 5.0/1023;*/
  osd.printMax7456Char(0x90,4,1,false,false);
  osd.print("100",0,1,false,true);
  osd.printMax7456Char(0x81,3,1,false,true);
  osd.print("12.6V",0,2,false,true);
}

void steercamera(){
    motorcamera.attach(camerapin, 1000, 2000);
    varCAM = map(ch[5], 1000, 2000, 0, 180);

    conCAM = constrain(conCAM, 0, 180);
    conCAM=conCAM+(90-varCAM)/64;
    motorcamera.write(conCAM);

    osd.print("cam", 0, 15);

    if(conCAM>178){
      osd.printMax7456Char(0xB7, 4, 15);
    }else if(conCAM>148){
      osd.printMax7456Char(0xB8, 4, 15);
    }else if(conCAM>118){
      osd.printMax7456Char(0xB9, 4, 15);
    }else if(conCAM>92){
      osd.printMax7456Char(0xBA, 4, 15);
    }else if(conCAM<=92 && conCAM>=88){
      osd.printMax7456Char(0xBB, 4, 15);
    }else if(conCAM>62){
      osd.printMax7456Char(0xBC, 4, 15);
    }else if(conCAM>32){
      osd.printMax7456Char(0xBD, 4, 15);
    }else if(conCAM>2){
      osd.printMax7456Char(0xBE, 4, 15);
    }else if(conCAM<=2){
      osd.printMax7456Char(0xBF, 4, 15);
    }

    //printcamera();
}

void printcamera(){
    Serial.print("camera: ");
    Serial.print(conCAM);
    Serial.print("\t");
    Serial.println(analogRead(camerapin));
    delay(000);
}






void steerlights(){
  vallights = map(ch[8], 1000, 2000, 0, 255);

  conLIGHT=conLIGHT+(vallights-conLIGHT)/30;

if(conLIGHT>1){
  analogWrite(ledlights,conLIGHT);
}else{
  analogWrite(ledlights,0);
}

if(conLIGHT>200){
  osd.printMax7456Char(0xF6, 27, 1);
  osd.printMax7456Char(0xF5, 26, 1);
}else if(conLIGHT>100){
  osd.printMax7456Char(0xF6, 27, 1);
  osd.printMax7456Char(0x00, 26, 1);
}else{
  osd.printMax7456Char(0x00, 27, 1);
  osd.printMax7456Char(0x00, 26, 1);
}

  //printlights();
}




void printlights(){
  Serial.print("lights:");
  Serial.print("\t");
  Serial.print(conLIGHT);
  Serial.print("\t");
  Serial.println(analogRead(ledlights));
  delay(000);
}
