Servo motorupdown;
Servo motorside;
Servo motorleft;
Servo motorright;

int valLUP;
int valRUP;
int valLLR;
int valRLR;


void steer(){
      motorupdown.attach(28,1000,2000);
      motorside.attach(30,1000,2000);
      motorleft.attach(26,1000,2000);
      motorright.attach(24,1000,2000);

      valLLR = map(ch[4], 1000, 2000, 0, 180);
      valRUP = map(ch[2], 1000, 2000, 0, 180);
      valLUP = map(ch[3], 1000, 2000, 0, 180);
      valRLR = map(ch[1], 1000, 2000, 0, 180);

      motorupdown.write(180-valLUP+10);
      motorside.write(valRLR);
      motorleft.write(valRUP+(valLLR-90));
      motorright.write(valRUP-(valLLR-90));
      
      Serial.print(0);
}
