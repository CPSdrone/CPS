#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 3
#define TEMPERATURE_PRECISION 12

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int numberOfDevices;
DeviceAddress tempDeviceAddress;

int vartemp;
int contemp;



void thermometerStart(){
 Serial.println("Dallas Temperature IC Control Library Demo");
  sensors.begin();
  
  numberOfDevices = sensors.getDeviceCount();
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");
  
  Serial.print("Parasite power is: "); 
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");
  
  for(int i=0;i<numberOfDevices; i++)
  {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i)){
      Serial.print("Found device ");
      Serial.print(i, DEC);
      Serial.print(" with address: ");
      //printAddress(tempDeviceAddress);
      Serial.println();
    
      Serial.print("Setting resolution to ");
      Serial.println(TEMPERATURE_PRECISION, DEC);
    
      // set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions)
      sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);
    
      Serial.print("Resolution actually set to: ");
      Serial.print(sensors.getResolution(tempDeviceAddress), DEC); 
      Serial.println();
    }else{
      Serial.print("Found ghost device at ");
      Serial.print(i, DEC);
      Serial.print(" but could not detect address. Check power and cabling");
    }
  }    
}


void temperatureGet(){

// call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  
  
  // Loop through each device, print out temperature data
  for(int i=0;i<numberOfDevices; i++)
  {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i))
  {
    // Output the device ID
    Serial.print("Temperature for device: ");
    Serial.println(i,DEC);
    
    // It responds almost immediately. Let's print out the data
    //printTemperature(tempDeviceAddress); // Use a simple function to print out the data
//    float tempC = sensors.getTempC(deviceAddress);
//  osd.print(tempC, 23, 3, 2, 1, false, true);
//  osd.printMax7456Char(0xD0, 27, 3);
//  Serial.print("Temp C: ");
//  Serial.print(tempC);
  } 
  //else ghost device! Check your power requirements and cabling
  
  }

}

/*
void printTemperature(DeviceAddress deviceAddress) 
{
  // method 1 - slower
  //Serial.print("Temp C: ");
  //Serial.print(sensors.getTempC(deviceAddress));
  //Serial.print(" Temp F: ");
  //Serial.print(sensors.getTempF(deviceAddress)); // Makes a second call to getTempC and then converts to Fahrenheit

  // method 2 - faster
  float tempC = sensors.getTempC(deviceAddress);
  osd.print(tempC, 23, 3, 2, 1, false, true);
  osd.printMax7456Char(0xD0, 27, 3);
  Serial.print("Temp C: ");
  Serial.print(tempC);
}


void printAddress(DeviceAddress deviceAddress){
  for (uint8_t i = 0; i < 8; i++){
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
*/
