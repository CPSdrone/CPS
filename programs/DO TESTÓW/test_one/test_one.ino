void setup() {
  Serial.begin(9600);
  Serial.print("Program started");
}

void loop() {
  for(int x;x<5;x++)
  
  switch(x){
   case 0:
    Serial.print("Hi");
    break;
   case 1:
    Serial.print(" I'm");
    break;
   case 2:
    Serial.print(" your");
    break;
   case 3:
    Serial.print(" host");
    break;
   case 4:
    Serial.print(" this");
    break;
   case 5:
    Serial.print(" time.");
    break;
   default:
    Serial.print(" not this time");
    break;
  }
  
}
