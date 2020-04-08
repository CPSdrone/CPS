#define motorupdownPIN 12
#define motorsidePIN 6
#define motorleftPIN 8
#define motorrightPIN 9

Servo motorupdown;
Servo motorside;
Servo motorleft;
Servo motorright;

int valLUP;
int valRUP;
int valLLR;
int valRLR;


void steer(){
      motorupdown.attach(motorupdownPIN,1000,2000);
      motorside.attach(motorsidePIN,1000,2000);
      motorleft.attach(motorleftPIN,1000,2000);
      motorright.attach(motorrightPIN,1000,2000);

      valLLR = map(ch[4], 1000, 2000, 0, 180);
      valRUP = map(ch[2], 1000, 2000, 0, 180);
      valLUP = map(ch[3], 1000, 2000, 0, 180);
      valRLR = map(ch[1], 1000, 2000, 0, 180);

      motorupdown.write(180-valLUP+38);
      motorside.write(valRLR);
      motorleft.write(valRUP+(valLLR-90)+38);
      motorright.write(valRUP-(valLLR-90)+34);


  printmotors();

}



void printmotors(){
  Serial.print("motor updown:");
  Serial.print(analogRead(motorupdownPIN));
  Serial.print("\t");
  Serial.print("motor side:");
  Serial.print(analogRead(motorsidePIN));
  Serial.print("\t");
  Serial.print("motor left:");
  Serial.print(analogRead(motorleftPIN));
  Serial.print("\t");
  Serial.print("motor right:");
  Serial.println(analogRead(motorrightPIN));
  delay(000);
}
