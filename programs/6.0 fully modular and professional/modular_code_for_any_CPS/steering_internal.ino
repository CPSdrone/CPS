Servo motorcamera;
#define camerapin 3
int varCAM;
int conCAM;

float valbat;
float volt;

#define ledlights 6
int vallights;
int osdlights;
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
  //temperatureGet();
}

void readbat(){
  /*valbat=analogRead(A2);
  volt = valbat * 5.0/1023;*/
  osd.printMax7456Char(0x90,4,1,false,false);
  osd.print("100",0,1,false,true);
  osd.printMax7456Char(0x81,3,1,false,true);
  
}

void steercamera(){
    motorcamera.attach(camerapin, 1000, 2000);
    varCAM = map(ch[5], 1000, 2000, 0, 180);

    conCAM = constrain(conCAM, 0, 180);
    conCAM=conCAM+(90-varCAM)/64;
    motorcamera.write(conCAM);

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
  vallights = map(ch[8], 1000, 2000, 0, 100);
  
  if(vallights>90){ analogWrite(ledlights,255);
    osdlights=100;}
  else if(vallights>30){ analogWrite(ledlights,70);
    osdlights=50;}
  else if(vallights>=0){ analogWrite(ledlights,0);
    osdlights=0;}

  osd.printMax7456Char(0xF5, 22, 1);
  osd.printMax7456Char(0xF6, 23, 1);
  osd.print(osdlights, 24, 1, 2, 1, false, true);

  //printlights();
}




void printlights(){
  Serial.print("lights:");
  Serial.print("\t");
  Serial.print(vallights);
  Serial.print("\t");
  Serial.println(analogRead(ledlights));
  delay(000);
}
