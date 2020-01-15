float calibration = 22.25; //change this value to calibrate
const int analogInPin = A5; 
int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int buf[10],temp;
void setup() {
 Serial.begin(9600);
}
