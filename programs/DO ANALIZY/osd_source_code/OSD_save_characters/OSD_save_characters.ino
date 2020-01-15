/*Decalre variables*/
byte garbage_byte, start_upload;

//////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(57600);               //Enable UART on ATmega328 @ 57600

  pinMode(6,OUTPUT);                 //To SC of MAX7456
  pinMode(10,OUTPUT);                //Not SS of the ATmega328. Needs to be declared as output to get the SPI to work
  pinMode(11, OUTPUT);               //To DIN of MAX7456
  pinMode(12, INPUT);                //To DOUT of MAX7456
  pinMode(13,OUTPUT);                //To SCK of MAX7456
  
  digitalWrite(6,HIGH);              //Disable MAX7456
  
  SPCR = (1<<SPE)|(1<<MSTR);         //Set SPI enable and SPI Master bit
  garbage_byte=SPSR;                 //Clear SPSR register
  garbage_byte=SPDR;                 //Clear SPDR register
  delay(250);
    
  /*Reset the MAX7456*/
  digitalWrite(6,LOW);               //Enable MAX7456
  spi_transfer(0x00, 0x42);          //Write 0b01000010 (PAL & software reset bits) to register h00 (Video Mode 0)
  digitalWrite(6,HIGH);              //Disable MAX7456
  delay(250);                        //Wait for the MAX7456 to complete reset
  
  /*Wait untill the character 'A' is receiverd via the serial port*/
  Serial.println("Send A to start.");
  while(start_upload == 0){
    if(Serial.available()){
      garbage_byte = Serial.read();
      if(garbage_byte == 65)start_upload = 1;
    }
  }
  /*Upload the characters to the MAX7456 buffer*/
  Serial.println("Start loading characters.......");
  load_characters_1();
  load_characters_2();
  load_numbers_1();
  load_numbers_2();
  Serial.println("Ready");

}
//////////////////////////////////////////////////////////////
void loop()
{
  /*Do nothing in the main loop*/
  delay(1000);
}


//////////////////////////////////////////////////////////////
byte spi_transfer(byte adress, byte data)
{
  SPDR = adress;                    //Load SPI data register
  while (!(SPSR & (1<<SPIF)));      //Wait until the byte is send
  SPDR = data;                      //Load SPI data register
  while (!(SPSR & (1<<SPIF)));      //Wait until the byte is send
}


void load_characters_1(){
  /*================================================================================*/
  Serial.println("Load: kmh symbol");
  digitalWrite(6,LOW);          //Enable device
  byte kmh[54] = {
    0b01000101, 0b01010101, 0b01010101, 0b00100001, 0b01010101, 0b01010101,
    0b00100001, 0b00010101, 0b01010101, 0b00100000, 0b10000101, 0b01010101,
    0b00100010, 0b00010101, 0b01010101, 0b00101000, 0b01010101, 0b00010101,
    0b00100010, 0b00010100, 0b10000101, 0b00100000, 0b10000010, 0b00010101,
    0b01000101, 0b00001000, 0b01010101, 0b01010101, 0b00100000, 0b01010101,
    0b01010100, 0b10000010, 0b00010101, 0b01010010, 0b00010010, 0b00010101,
    0b01010100, 0b01010010, 0b00000101, 0b01010101, 0b01010010, 0b10100001,
    0b01010101, 0b01010010, 0b00001000, 0b01010101, 0b01010010, 0b00001000,
    0b01010101, 0b01010010, 0b00001000, 0b01010101, 0b01010100, 0b01010001
  };
  
  spi_transfer(0x00, 0x40);     //Disable OSD
  spi_transfer(0x09, 0xC0);     //Select the character you want to overwrite
  
  for(byte i = 0; i < 54; i ++){
    spi_transfer(0x0A, i);
    spi_transfer(0x0B, kmh[i]);
  }
  spi_transfer(0x08 , 0xA0);    //Copy the character to Non Volatile Memory
  delay(250);
  digitalWrite(6,HIGH);          //Disable device
  delay(250);
  
  /*================================================================================*/
  Serial.println("Load: satellite symbol");
  digitalWrite(6,LOW);          //Enable device
  
  byte sat[54] = {
    0b01010001, 0b01010101, 0b01010101, 0b01001000, 0b01010100, 0b01010101,
    0b00101010, 0b00010010, 0b00010101, 0b10101000, 0b01001010, 0b10000101,
    0b00100010, 0b00101010, 0b10100001, 0b01000100, 0b10101010, 0b10000101,
    0b01010010, 0b10101010, 0b00010101, 0b01001010, 0b10101000, 0b01000101,
    0b00000010, 0b10100010, 0b00100001, 0b10101000, 0b10000100, 0b10101000,
    0b00101000, 0b00010010, 0b10100001, 0b01001000, 0b01010100, 0b10000101,
    0b01010001, 0b01010101, 0b00010101, 0b01010101, 0b01010101, 0b01010101,
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101

  };
  
  spi_transfer(0x09, 0xC1);     //Select the character you want to overwrite
  
  for(byte i = 0; i < 54; i ++){
    spi_transfer(0x0A, i);
    spi_transfer(0x0B, sat[i]);
  }
  spi_transfer(0x08 , 0xA0);    //Copy the character to Non Volatile Memory
  delay(250);
  digitalWrite(6,HIGH);          //Disable device
  delay(250);
  
  /*================================================================================*/
  Serial.println("Load: battery symbol");
  digitalWrite(6,LOW);          //Enable device
  
  /*Battery symbol*/
  byte bat_symbol[54] = {
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b00000000, 0b01010101,
    0b01010100, 0b10101010, 0b00010101, 0b01010100, 0b10101010, 0b00010101,
    0b01010010, 0b10101010, 0b10000101, 0b01010010, 0b00000000, 0b10000101,
    0b01010010, 0b10101010, 0b10000101, 0b01010010, 0b00000000, 0b10000101,
    0b01010010, 0b10101010, 0b10000101, 0b01010010, 0b00000000, 0b10000101,
    0b01010010, 0b10101010, 0b10000101, 0b01010010, 0b00000000, 0b10000101,
    0b01010010, 0b10101010, 0b10000101, 0b01010010, 0b00000000, 0b10000101,
    0b01010010, 0b10101010, 0b10000101, 0b01010100, 0b00000000, 0b00010101
  };
  
  spi_transfer(0x09, 0xC2);     //Select the character you want to overwrite
  
  for(byte i = 0; i < 54; i ++){
    spi_transfer(0x0A, i);
    spi_transfer(0x0B, bat_symbol[i]);
  }
  spi_transfer(0x08 , 0xA0);    //Copy the character to Non Volatile Memory
  delay(250);
  digitalWrite(6,HIGH);          //Disable device
  delay(250);
  
  /*================================================================================*/
  Serial.println("Load: ground distance symbol");
  digitalWrite(6,LOW);          //Enable device
  
  /*Ground distance symbol*/
  byte ground_distance_symbol[54] = {
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
    0b01010101, 0b01010100, 0b01010101, 0b01010101, 0b01010010, 0b00010101,
    0b01010000, 0b00000000, 0b10000101, 0b01001010, 0b10101010, 0b10100001,
    0b01010000, 0b00000000, 0b10000101, 0b01010101, 0b01010010, 0b00010101,
    0b01010101, 0b01010100, 0b01010101, 0b01010101, 0b01010101, 0b01010101
  };
  
  spi_transfer(0x09, 0xC3);     //Select the character you want to overwrite
  
  for(byte i = 0; i < 54; i ++){
    spi_transfer(0x0A, i);
    spi_transfer(0x0B, ground_distance_symbol[i]);
  }
  spi_transfer(0x08 , 0xA0);    //Copy the character to Non Volatile Memory
  delay(250);
  digitalWrite(6,HIGH);          //Disable device
  delay(250);
  
  /*================================================================================*/
  Serial.println("Load: line of sight symbol");
  digitalWrite(6,LOW);          //Enable device
  
  /*Line of sight symbol*/
  byte line_of_sight_symbol[54] = {
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01000000, 0b00000101,
    0b01010101, 0b00101010, 0b10100001, 0b01010101, 0b01000000, 0b10100001,
    0b01010101, 0b01010010, 0b00100001, 0b01010101, 0b01001000, 0b00100001,
    0b01010101, 0b00100001, 0b00100001, 0b01010100, 0b10000101, 0b01000101,
    0b01010010, 0b00010101, 0b01010101, 0b01010100, 0b01010101, 0b01010101,
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101
  };
  
  spi_transfer(0x09, 0xC4);     //Select the character you want to overwrite
  
  for(byte i = 0; i < 54; i ++){
    spi_transfer(0x0A, i);
    spi_transfer(0x0B, line_of_sight_symbol[i]);
  }
  spi_transfer(0x08 , 0xA0);    //Copy the character to Non Volatile Memory
  delay(250);
  digitalWrite(6,HIGH);          //Disable device
  delay(250);
}
  
  
void load_characters_2(){
  /*================================================================================*/
  Serial.println("Load: home compass symbol");
  digitalWrite(6,LOW);          //Enable device
  
  /*Home compass symbol*/
  byte home_compass_symbol[54] = {
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010000, 0b00000101,
    0b01010101, 0b01001010, 0b10100001, 0b01010101, 0b01010000, 0b00100001,
    0b01010101, 0b01010010, 0b00100001, 0b01010101, 0b01001000, 0b00100001,
    0b01010101, 0b00100001, 0b01000101, 0b01010100, 0b10000101, 0b01010101,
    0b01010101, 0b00000101, 0b01010101, 0b01010101, 0b00100001, 0b01010101,
    0b01010100, 0b10101000, 0b01010101, 0b01010010, 0b10101010, 0b00010101,
    0b01001010, 0b10101010, 0b10000101, 0b01010010, 0b10101010, 0b00010101,
    0b01010010, 0b10101010, 0b00010101, 0b01010010, 0b10101010, 0b00010101,
    0b01010010, 0b10101010, 0b00010101, 0b01010100, 0b00000000, 0b01010101
  };
  
  spi_transfer(0x09, 0xC5);     //Select the character you want to overwrite
  
  for(byte i = 0; i < 54; i ++){
    spi_transfer(0x0A, i);
    spi_transfer(0x0B, home_compass_symbol[i]);
  }
  spi_transfer(0x08 , 0xA0);    //Copy the character to Non Volatile Memory
  delay(250);
  digitalWrite(6,HIGH);          //Disable device
  delay(250);
  
  /*================================================================================*/
  Serial.println("Load: meter symbol");
  digitalWrite(6,LOW);          //Enable device
  
  /*Meters symbol*/
  byte mtr_symbol[54] = {
    0b01010001, 0b00010101, 0b01010101, 0b01001000, 0b10000101, 0b01010101,
    0b00100010, 0b00100001, 0b01010101, 0b00100010, 0b00100001, 0b01010101,
    0b00100010, 0b00100001, 0b01010101, 0b00100010, 0b00100001, 0b01010101,
    0b01000100, 0b01000101, 0b01010101, 0b01010101, 0b00100001, 0b01010101,
    0b01010101, 0b00101000, 0b01010101, 0b01010101, 0b00100000, 0b01010101,
    0b01010101, 0b00100010, 0b00010101, 0b01010101, 0b01001000, 0b01010101,
    0b01010101, 0b01010001, 0b00010101, 0b01010101, 0b01010100, 0b10000101,
    0b01010101, 0b01010010, 0b00100001, 0b01010101, 0b01010010, 0b00000101,
    0b01010101, 0b01010010, 0b00010101, 0b01010101, 0b01010010, 0b00010101
  };
  
  spi_transfer(0x09, 0xC6);     //Select the character you want to overwrite
  
  for(byte i = 0; i < 54; i ++){
    spi_transfer(0x0A, i);
    spi_transfer(0x0B, mtr_symbol[i]);
  }
  spi_transfer(0x08 , 0xA0);    //Copy the character to Non Volatile Memory
  delay(250);
  digitalWrite(6,HIGH);          //Disable device
  delay(250);
  
  /*================================================================================*/
  Serial.println("Load: home lock symbol");
  digitalWrite(6,LOW);          //Enable device
  
  /*Home lock symbol*/
  byte home_lock[54] = {
    0b01010101, 0b00000101, 0b01010101, 0b01010100, 0b10100001, 0b01010101,
    0b01010010, 0b00001000, 0b01010101, 0b01010010, 0b00001000, 0b01010101,
    0b01001010, 0b10101010, 0b00010101, 0b01001010, 0b10101010, 0b00010101,
    0b01001010, 0b10101010, 0b00010101, 0b01010000, 0b00000000, 0b01010101,
    0b01010101, 0b01010001, 0b01010101, 0b01010101, 0b01001000, 0b01010101,
    0b01010101, 0b00101010, 0b00010101, 0b01010100, 0b10101010, 0b10000101,
    0b01010010, 0b10101010, 0b10100001, 0b01001010, 0b10101010, 0b10101000,
    0b01010010, 0b10101010, 0b10100001, 0b01010010, 0b10101010, 0b10100001,
    0b01010010, 0b10101010, 0b10100001, 0b01010100, 0b00000000, 0b00000101
  };
  
  spi_transfer(0x09, 0xC7);     //Select the character you want to overwrite
  
  for(byte i = 0; i < 54; i ++){
    spi_transfer(0x0A, i);
    spi_transfer(0x0B, home_lock[i]);
  }
  spi_transfer(0x08 , 0xA0);    //Copy the character to Non Volatile Memory
  delay(250);
  digitalWrite(6,HIGH);          //Disable device
  delay(250);
  
  /*================================================================================*/
  Serial.println("Load: no home lock symbol");
  digitalWrite(6,LOW);          //Enable device
  
  /*No home lock symbol*/
  byte no_home_lock[54] = {
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
    0b01010000, 0b00000000, 0b00000101, 0b01001010, 0b10101010, 0b10100001,
    0b01010000, 0b00000000, 0b00000101, 0b01010101, 0b01010101, 0b01010101,
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
  };
  
  spi_transfer(0x09, 0xC8);     //Select the character you want to overwrite
  
  for(byte i = 0; i < 54; i ++){
    spi_transfer(0x0A, i);
    spi_transfer(0x0B, no_home_lock[i]);
  }
  spi_transfer(0x08 , 0xA0);    //Copy the character to Non Volatile Memory
  delay(250);
  digitalWrite(6,HIGH);          //Disable device
  delay(250);
  
  /*================================================================================*/
  Serial.println("Load: decimal dot symbol.");
  digitalWrite(6,LOW);          //Enable device
  
  /*Decimal dot symbol*/
  byte decimal_dot[54] = {
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
    0b01010101, 0b01000001, 0b01010101, 0b01010101, 0b00101000, 0b01010101,
    0b01010101, 0b00101000, 0b01010101, 0b01010101, 0b01000001, 0b01010101,
  };
  
  spi_transfer(0x09, 0xC9);     //Select the character you want to overwrite
  
  for(byte i = 0; i < 54; i ++){
    spi_transfer(0x0A, i);
    spi_transfer(0x0B, decimal_dot[i]);
  }
  spi_transfer(0x08 , 0xA0);    //Copy the character to Non Volatile Memory
  delay(250);
  digitalWrite(6,HIGH);          //Disable device
  delay(250);  
}







void load_numbers_1(){
  /*================================================================================*/
  Serial.println("Load: 0");
  digitalWrite(6,LOW);          //Enable device
  
  /*Program character 0*/
  byte character_0[54] = {
    0b01010101, 0b01010101, 0b01010101, 0b01010100, 0b00000000, 0b00010101,
    0b01010010, 0b10101010, 0b10000101, 0b01001000, 0b00000000, 0b00100001,
    0b01001000, 0b01010101, 0b00100001, 0b01001000, 0b01010101, 0b00100001,
    0b01001000, 0b01010101, 0b00100001, 0b01001000, 0b01010101, 0b00100001,
    0b01001000, 0b01010101, 0b00100001, 0b01010001, 0b01010101, 0b01000101,
    0b01001000, 0b01010101, 0b00100001, 0b01001000, 0b01010101, 0b00100001,
    0b01001000, 0b01010101, 0b00100001, 0b01001000, 0b01010101, 0b00100001,
    0b01001000, 0b01010101, 0b00100001, 0b01001000, 0b00000000, 0b00100001,
    0b01010010, 0b10101010, 0b10000101, 0b01010100, 0b00000000, 0b00010101
  };
  
  spi_transfer(0x09, 0xD0);     //Select the character you want to overwrite
  
  for(byte i = 0; i < 54; i ++){
    spi_transfer(0x0A, i);
    spi_transfer(0x0B, character_0[i]);
  }
  spi_transfer(0x08 , 0xA0);    //Copy the character to Non Volatile Memory
  delay(250);
  digitalWrite(6,HIGH);          //Disable device
  delay(250);
  Serial.println("Load: 1");
  digitalWrite(6,LOW);          //Enable device
    
  /*Program character 1*/
  byte character_1[54] = {
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
    0b01010101, 0b01010101, 0b01000101, 0b01010101, 0b01010101, 0b00100001,
    0b01010101, 0b01010101, 0b00100001, 0b01010101, 0b01010101, 0b00100001,
    0b01010101, 0b01010101, 0b00100001, 0b01010101, 0b01010101, 0b00100001,
    0b01010101, 0b01010101, 0b00100001, 0b01010101, 0b01010101, 0b01000101,
    0b01010101, 0b01010101, 0b00100001, 0b01010101, 0b01010101, 0b00100001,
    0b01010101, 0b01010101, 0b00100001, 0b01010101, 0b01010101, 0b00100001,
    0b01010101, 0b01010101, 0b00100001, 0b01010101, 0b01010101, 0b00100001,
    0b01010101, 0b01010101, 0b01000101, 0b01010101, 0b01010101, 0b01010101
  };
  
  spi_transfer(0x09, 0xD1);     //Select the character you want to overwrite
  
  for(byte i = 0; i < 54; i ++){
    spi_transfer(0x0A, i);
    spi_transfer(0x0B, character_1[i]);
  }
  spi_transfer(0x08 , 0xA0);    //Copy the character to Non Volatile Memory
  delay(250);
  digitalWrite(6,HIGH);          //Disable device
  delay(250);
  
  /*================================================================================*/
  Serial.println("Load: 2");
  digitalWrite(6,LOW);          //Enable device
  
  /*Program character 2*/
  byte character_2[54] = {
    0b01010101, 0b01010101, 0b01010101, 0b01010100, 0b00000000, 0b00010101,
    0b01010010, 0b10101010, 0b10000101, 0b01010100, 0b00000000, 0b00100001,
    0b01010101, 0b01010101, 0b00100001, 0b01010101, 0b01010101, 0b00100001,
    0b01010101, 0b01010101, 0b00100001, 0b01010101, 0b01010101, 0b00100001,
    0b01010100, 0b00000000, 0b00100001, 0b01010010, 0b10101010, 0b10000101,
    0b01001000, 0b00000000, 0b00010101, 0b01001000, 0b01010101, 0b01010101,
    0b01001000, 0b01010101, 0b01010101, 0b01001000, 0b01010101, 0b01010101,
    0b01001000, 0b01010101, 0b01010101, 0b01001000, 0b00000000, 0b00010101,
    0b01010010, 0b10101010, 0b10000101, 0b01010100, 0b00000000, 0b00010101
  };
  
  spi_transfer(0x09, 0xD2);     //Select the character you want to overwrite
  
  for(byte i = 0; i < 54; i ++){
    spi_transfer(0x0A, i);
    spi_transfer(0x0B, character_2[i]);
  }
  spi_transfer(0x08 , 0xA0);    //Copy the character to Non Volatile Memory
  delay(250);
  digitalWrite(6,HIGH);          //Disable device
  delay(250);
  
  /*================================================================================*/
  Serial.println("Load: 3");
  digitalWrite(6,LOW);          //Enable device
    
  /*Program character 3*/
  byte character_3[54] = {
    0b01010101, 0b01010101, 0b01010101, 0b01010100, 0b00000000, 0b00010101,
    0b01010010, 0b10101010, 0b10000101, 0b01010100, 0b00000000, 0b00100001,
    0b01010101, 0b01010101, 0b00100001, 0b01010101, 0b01010101, 0b00100001,
    0b01010101, 0b01010101, 0b00100001, 0b01010101, 0b01010101, 0b00100001,
    0b01010100, 0b00000000, 0b00100001, 0b01010010, 0b10101010, 0b10000101,
    0b01010100, 0b00000000, 0b00100001, 0b01010101, 0b01010101, 0b00100001,
    0b01010101, 0b01010101, 0b00100001, 0b01010101, 0b01010101, 0b00100001,
    0b01010101, 0b01010101, 0b00100001, 0b01010100, 0b00000000, 0b00100001,
    0b01010010, 0b10101010, 0b10000101, 0b01010100, 0b00000000, 0b00010101
  };
  
  spi_transfer(0x09, 0xD3);     //Select the character you want to overwrite
  
  for(byte i = 0; i < 54; i ++){
    spi_transfer(0x0A, i);
    spi_transfer(0x0B, character_3[i]);
  }
  spi_transfer(0x08 , 0xA0);    //Copy the character to Non Volatile Memory
  delay(250);
  digitalWrite(6,HIGH);          //Disable device
  delay(250);
  
  /*================================================================================*/
  Serial.println("Load: 4");
  digitalWrite(6,LOW);          //Enable device
  
  /*Program character 4*/
  byte character_4[54] = {
    0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101, 0b01010101,
    0b01010001, 0b01010101, 0b01000101, 0b01001000, 0b01010101, 0b00100001,
    0b01001000, 0b01010101, 0b00100001, 0b01001000, 0b01010101, 0b00100001,
    0b01001000, 0b01010101, 0b00100001, 0b01001000, 0b01010101, 0b00100001,
    0b01001000, 0b00000000, 0b00100001, 0b01010010, 0b10101010, 0b10000101,
    0b01010100, 0b00000000, 0b00100001, 0b01010101, 0b01010101, 0b00100001,
    0b01010101, 0b01010101, 0b00100001, 0b01010101, 0b01010101, 0b00100001,
    0b01010101, 0b01010101, 0b00100001, 0b01010101, 0b01010101, 0b00100001,
    0b01010101, 0b01010101, 0b01000101, 0b01010101, 0b01010101, 0b01010101
  };
  
  spi_transfer(0x09, 0xD4);     //Select the character you want to overwrite
  
  for(byte i = 0; i < 54; i ++){
    spi_transfer(0x0A, i);
    spi_transfer(0x0B, character_4[i]);
  }
  spi_transfer(0x08 , 0xA0);    //Copy the character to Non Volatile Memory
  delay(250);
  digitalWrite(6,HIGH);          //Disable device
  delay(250);
}
  
  
  
  
void load_numbers_2(){
  /*================================================================================*/
  Serial.println("Load: 5");
  digitalWrite(6,LOW);          //Enable device
  
  /*Program character 5*/
  byte character_5[54] = {
    0b01010101, 0b01010101, 0b01010101, 0b01010100, 0b00000000, 0b00010101,
    0b01010010, 0b10101010, 0b10000101, 0b01001000, 0b00000000, 0b00010101,
    0b01001000, 0b01010101, 0b01010101, 0b01001000, 0b01010101, 0b01010101,
    0b01001000, 0b01010101, 0b01010101, 0b01001000, 0b01010101, 0b01010101,
    0b01001000, 0b00000000, 0b00010101, 0b01010010, 0b10101010, 0b10000101,
    0b01010100, 0b00000000, 0b00100001, 0b01010101, 0b01010101, 0b00100001,
    0b01010101, 0b01010101, 0b00100001, 0b01010101, 0b01010101, 0b00100001,
    0b01010101, 0b01010101, 0b00100001, 0b01010100, 0b00000000, 0b00100001,
    0b01010010, 0b10101010, 0b10000101, 0b01010100, 0b00000000, 0b00010101
  };
  
  spi_transfer(0x09, 0xD5);     //Select the character you want to overwrite
  
  for(byte i = 0; i < 54; i ++){
    spi_transfer(0x0A, i);
    spi_transfer(0x0B, character_5[i]);
  }
  spi_transfer(0x08 , 0xA0);    //Copy the character to Non Volatile Memory
  delay(250);
  digitalWrite(6,HIGH);          //Disable device
  delay(250);
  
  /*================================================================================*/
  Serial.println("Load: 6");
  digitalWrite(6,LOW);          //Enable device
  
  /*Program character 6*/
  byte character_6[54] = {
    0b01010101, 0b01010101, 0b01010101, 0b01010100, 0b00000000, 0b00010101,
    0b01010010, 0b10101010, 0b10000101, 0b01001000, 0b00000000, 0b00010101,
    0b01001000, 0b01010101, 0b01010101, 0b01001000, 0b01010101, 0b01010101,
    0b01001000, 0b01010101, 0b01010101, 0b01001000, 0b01010101, 0b01010101,
    0b01001000, 0b00000000, 0b00010101, 0b01010010, 0b10101010, 0b10000101,
    0b01001000, 0b00000000, 0b00100001, 0b01001000, 0b01010101, 0b00100001,
    0b01001000, 0b01010101, 0b00100001, 0b01001000, 0b01010101, 0b00100001,
    0b01001000, 0b01010101, 0b00100001, 0b01001000, 0b00000000, 0b00100001,
    0b01010010, 0b10101010, 0b10000101, 0b01010100, 0b00000000, 0b00010101
  };
  
  spi_transfer(0x09, 0xD6);     //Select the character you want to overwrite
  
  for(byte i = 0; i < 54; i ++){
    spi_transfer(0x0A, i);
    spi_transfer(0x0B, character_6[i]);
  }
  spi_transfer(0x08 , 0xA0);    //Copy the character to Non Volatile Memory
  delay(250);
  digitalWrite(6,HIGH);          //Disable device
  delay(250);
  
  /*================================================================================*/
  Serial.println("Load: 7");
  digitalWrite(6,LOW);          //Enable device
  
  /*Program character 7*/
  byte character_7[54] = {
    0b01010101, 0b01010101, 0b01010101, 0b01010100, 0b00000000, 0b00010101,
    0b01010010, 0b10101010, 0b10000101, 0b01010100, 0b00000000, 0b00100001,
    0b01010101, 0b01010101, 0b00100001, 0b01010101, 0b01010101, 0b00100001,
    0b01010101, 0b01010101, 0b00100001, 0b01010101, 0b01010101, 0b00100001,
    0b01010101, 0b01010101, 0b00100001, 0b01010101, 0b01010101, 0b01000101,
    0b01010101, 0b01010101, 0b00100001, 0b01010101, 0b01010101, 0b00100001,
    0b01010101, 0b01010101, 0b00100001, 0b01010101, 0b01010101, 0b00100001,
    0b01010101, 0b01010101, 0b00100001, 0b01010101, 0b01010101, 0b00100001,
    0b01010101, 0b01010101, 0b01000101, 0b01010101, 0b01010101, 0b01010101
  };
  
  spi_transfer(0x09, 0xD7);     //Select the character you want to overwrite
  
  for(byte i = 0; i < 54; i ++){
    spi_transfer(0x0A, i);
    spi_transfer(0x0B, character_7[i]);
  }
  spi_transfer(0x08 , 0xA0);    //Copy the character to Non Volatile Memory
  delay(250);
  digitalWrite(6,HIGH);          //Disable device
  delay(250);
  
  /*================================================================================*/
  Serial.println("Load: 8");
  digitalWrite(6,LOW);          //Enable device
  
  /*Program character 8*/
  byte character_8[54] = {
    0b01010101, 0b01010101, 0b01010101, 0b01010100, 0b00000000, 0b00010101,
    0b01010010, 0b10101010, 0b10000101, 0b01001000, 0b00000000, 0b00100001,
    0b01001000, 0b01010101, 0b00100001, 0b01001000, 0b01010101, 0b00100001,
    0b01001000, 0b01010101, 0b00100001, 0b01001000, 0b01010101, 0b00100001,
    0b01001000, 0b00000000, 0b00100001, 0b01010010, 0b10101010, 0b10000101,
    0b01001000, 0b00000000, 0b00100001, 0b01001000, 0b01010101, 0b00100001,
    0b01001000, 0b01010101, 0b00100001, 0b01001000, 0b01010101, 0b00100001,
    0b01001000, 0b01010101, 0b00100001, 0b01001000, 0b00000000, 0b00100001,
    0b01010010, 0b10101010, 0b10000101, 0b01010100, 0b00000000, 0b00010101
  };
  
  spi_transfer(0x09, 0xD8);     //Select the character you want to overwrite
  
  for(byte i = 0; i < 54; i ++){
    spi_transfer(0x0A, i);
    spi_transfer(0x0B, character_8[i]);
  }
  spi_transfer(0x08 , 0xA0);    //Copy the character to Non Volatile Memory
  delay(250);
  digitalWrite(6,HIGH);          //Disable device
  delay(250);
  
  /*================================================================================*/
  Serial.println("Load: 9");
  digitalWrite(6,LOW);          //Enable device
  
  /*Program character 9*/
  byte character_9[54] = {
    0b01010101, 0b01010101, 0b01010101, 0b01010100, 0b00000000, 0b00010101,
    0b01010010, 0b10101010, 0b10000101, 0b01001000, 0b00000000, 0b00100001,
    0b01001000, 0b01010101, 0b00100001, 0b01001000, 0b01010101, 0b00100001,
    0b01001000, 0b01010101, 0b00100001, 0b01001000, 0b01010101, 0b00100001,
    0b01001000, 0b00000000, 0b00100001, 0b01010010, 0b10101010, 0b10000101,
    0b01010100, 0b00000000, 0b00100001, 0b01010101, 0b01010101, 0b00100001,
    0b01010101, 0b01010101, 0b00100001, 0b01010101, 0b01010101, 0b00100001,
    0b01010101, 0b01010101, 0b00100001, 0b01010100, 0b00000000, 0b00100001,
    0b01010010, 0b10101010, 0b10000101, 0b01010100, 0b00000000, 0b00010101
  };
  
  spi_transfer(0x09, 0xD9);     //Select the character you want to overwrite
  
  for(byte i = 0; i < 54; i ++){
    spi_transfer(0x0A, i);
    spi_transfer(0x0B, character_9[i]);
  }
  spi_transfer(0x08 , 0xA0);    //Copy the character to Non Volatile Memory
  
  delay(250);
  digitalWrite(6,HIGH);         //Disable device
  delay(250);
}

