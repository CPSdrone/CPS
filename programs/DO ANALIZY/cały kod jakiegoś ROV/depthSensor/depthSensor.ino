// code for the MS5540C depth/temperature sensor
//  It uses SPI communication, require initial read to get calibration
//  information.

     // MS554x Depth Sensor (and thermometer)
 // SPI Pins for Uno      Mega    Cable
 //   VCC         3v+     3v+     Red
 //   GND         GND     GND     Green
 //   DIN (MOSI)  11       51     Lt Blue  
 //  DOUT (MISO)  12       50     Brown
 //  SCLK  (SCK)  13       52     Blue 
 //  MCLK  (CS?)   9       11     Tan
 int clock = 11; //9;


 unsigned int word1 = 0;
 unsigned int word2 = 0;
 unsigned int word3 = 0;
 unsigned int word4 = 0;
 long c1 = 0;
 long c2 = 0;
 long c3 = 0;
 long c4 = 0;
 long c5 = 0;
 long c6 = 0;

void getMS5540Calib() {
 SPI.begin(); //see SPI library details on arduino.cc for details

 SPI.setBitOrder(MSBFIRST);
 SPI.setClockDivider(SPI_CLOCK_DIV32); //divide 16 MHz to communicate on 500 kHz
 pinMode(clock, OUTPUT);
 delay(100);
 TCCR1B = (TCCR1B & 0xF8) | 1 ; //generates the MCKL signal
 analogWrite (clock, 128) ;
 resetsensor();//resets the sensor - caution: afterwards mode = SPI_MODE0!
 
 //Calibration word 1
 unsigned int word11 = 0;
 SPI.transfer(0x1D); //send first byte of command to get calibration word 1
 SPI.transfer(0x50); //send second byte of command to get calibration word 1
 SPI.setDataMode(SPI_MODE1); //change mode in order to listen
 word1 = SPI.transfer(0x00); //send dummy byte to read first byte of word
 word1 = word1 << 8; //shift returned byte
 word11 = SPI.transfer(0x00); //send dummy byte to read second byte of word
 word1 = word1 | word11; //combine first and second byte of word
 resetsensor();//resets the sensor
 
 //Calibration word 2; see comments on calibration word 1
 byte word22 = 0;
 SPI.transfer(0x1D);
 SPI.transfer(0x60);
 SPI.setDataMode(SPI_MODE1);
 word2 = SPI.transfer(0x00);
 word2 = word2 <<8;
 word22 = SPI.transfer(0x00);
 word2 = word2 | word22;
 resetsensor();//resets the sensor
 
 //Calibration word 3; see comments on calibration word 1
 byte word33 = 0;
 SPI.transfer(0x1D);
 SPI.transfer(0x90);
 SPI.setDataMode(SPI_MODE1);
 word3 = SPI.transfer(0x00);
 word3 = word3 <<8;
 word33 = SPI.transfer(0x00);
 word3 = word3 | word33;
 resetsensor();//resets the sensor
 //Calibration word 4; see comments on calibration word 1
 
 byte word44 = 0;
 SPI.transfer(0x1D);
 SPI.transfer(0xA0);
 SPI.setDataMode(SPI_MODE1);
 word4 = SPI.transfer(0x00);
 word4 = word4 <<8;
 word44 = SPI.transfer(0x00);
 word4 = word4 | word44;

 c1 = word1 >> 1;
 c2 = ((word3 & 0x3F) << 6) | ((word4 & 0x3F));
 c3 = (word4 >> 6) ;
 c4 = (word3 >> 6);
 c5 = (word2 >> 6) | ((word1 & 0x1) >> 10);
 c6 = word2 & 0x3F;
 resetsensor();//resets sensor
}


long getMS5540Pressure() {  //also get water temp into global
 TCCR1B = (TCCR1B & 0xF8) | 1 ; //generates the MCKL signal
 analogWrite (clock, 128) ;
 resetsensor();//resets the sensor - caution: afterwards mode = SPI_MODE0!

 //Temperature:
 unsigned int tempMSB = 0; //first byte of value
 unsigned int tempLSB = 0; //last byte of value
 unsigned int D2 = 0;
 SPI.transfer(0x0F); //send first byte of command to get temperature value
 SPI.transfer(0x20); //send second byte of command to get temperature value
 delay(35); //wait for conversion end
 SPI.setDataMode(SPI_MODE1); //change mode in order to listen
 tempMSB = SPI.transfer(0x00); //send dummy byte to read first byte of value
 tempMSB = tempMSB << 8; //shift first byte
 tempLSB = SPI.transfer(0x00); //send dummy byte to read second byte of value
 D2 = tempMSB | tempLSB; //combine first and second byte of value
 resetsensor();//resets the sensor
 //Pressure:
 unsigned int presMSB = 0; //first byte of value
 unsigned int presLSB =0; //last byte of value
 unsigned int D1 = 0;
 SPI.transfer(0x0F); //send first byte of command to get pressure value
 SPI.transfer(0x40); //send second byte of command to get pressure value
 delay(35); //wait for conversion end
 SPI.setDataMode(SPI_MODE1); //change mode in order to listen
 presMSB = SPI.transfer(0x00); //send dummy byte to read first byte of value
 presMSB = presMSB << 8; //shift first byte
 presLSB = SPI.transfer(0x00); //send dummy byte to read second byte of value
 D1 = presMSB | presLSB;

 const long UT1 = (c5 * 8) + 20224;
 const long dT =(D2 - UT1);
 const long TEMP = 200 + ((dT * (c6 + 50))/1024);
 const long OFF = (c2*4) + (((c4 - 512) * dT)/4096);
 const long SENS = c1 + ((c3 * dT)/1024) + 24576;

 long pressure = ((((SENS * (D1 - 7168))/16384)- OFF)/32)+250;
 float TEMPREAL = TEMP/10;
// Serial.print("pressure = ");
// Serial.print(pressure);
// Serial.println(" mbar");
 const long dT2 = dT - ((dT >> 7 * dT >> 7) >> 3);
 waterTemp = (200 + (dT2*(c6+100) >>11))/10;
// Serial.print("temperature = ");
 //Serial.print(waterTemp);
 //Serial.println(" °C");
 //Serial.println("************************************");
 //delay(1000);
 return pressure;
}
 
void resetsensor() //this function called frequently
{
 SPI.setDataMode(SPI_MODE0);
 SPI.transfer(0x15);
 SPI.transfer(0x55);
 SPI.transfer(0x40);
}

