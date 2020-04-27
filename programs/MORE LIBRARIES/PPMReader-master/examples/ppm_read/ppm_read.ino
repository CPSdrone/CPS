#include "PPMReader.h"

PPMReader ppmReader(2, 0, false);

void setup()
{
  Serial.begin(115200);
  Serial.println("ready");
}

void loop()
{

  int count;
  while(ppmReader.get(count) != 0){  //print out the servo values
    Serial.print(ppmReader.get(count));
    Serial.print("  ");
    count++;
  }
  Serial.println("");

  delay(500); //you can even use delays!!!
}
