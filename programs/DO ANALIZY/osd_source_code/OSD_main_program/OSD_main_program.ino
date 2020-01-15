/*Decalre variables*/
byte screen_buffer[480];
byte serial_input_buffer[100];
int  common_counter, home_lock, home_lock_counter;
byte garbage_byte, search_index;

byte numSV, gpsFix;
unsigned long gSpeed;
long heading, lon, lat, height, hMSL, totHeight;
long home_lon, home_lat, home_totHeight;
int flight_altitude, ground_distance, line_of_sight, heading_to_home, bat_voltage;

float latitude_compensation;


unsigned long loop_timer;

//////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(57600);               //Enable UART on ATmega328 @ 57600
  //pinMode(0, INPUT);
  pinMode(6,OUTPUT);                 //To SC of MAX7456
  pinMode(10,OUTPUT);                //Not SS of the ATmega328. Needs to be declared as output to get the SPI to work
  pinMode(11, OUTPUT);               //To DIN of MAX7456
  //pinMode(12, INPUT);                //To DOUT of MAX7456
  pinMode(13,OUTPUT);                //To SCK of MAX7456
  
  digitalWrite(6,HIGH);              //Disable device
  
  SPCR = (1<<SPE)|(1<<MSTR);         //Set SPI enable and SPI Master bit
  delay(250);
  
  /*Reset the MAX7456*/
  digitalWrite(6,LOW);               //Enable device
  spi_transfer(0x00, 0x42);          //Write 0b01000010 (PAL & software reset bits) to register h00 (Video Mode 0)
  digitalWrite(6,HIGH);              //disable device
  delay(250);                        //Wait for the MAX7456 to complete reset
  
  digitalWrite(6,LOW);               //Enable device
  /*Horizontal position offset*/
  spi_transfer(0x02, 0x2C);          //0x20 = no correction, min = 0x00 (-32 pixels), max = 0x3F (+31 pixels)
  /*Vertical position offset*/
  spi_transfer(0x03, 0x16);          //0x10 = no correction, min = 0x00 (+16 pixels), max = 0x1F (-15 pixels)
  /*Enable MAX7456*/
  spi_transfer(0x00, 0x48);          //Write 0b01001000 (PAL & enable OSD) to register h00 (Video Mode 0)
  digitalWrite(6,HIGH);              //disable device
  
  /*Clear the screen buffer array*/
  //for (common_counter = 0; common_counter < 480; common_counter++)screen_buffer[common_counter] = 0x00;
}
//////////////////////////////////////////////////////////////
void loop()
{
  loop_timer = millis();
  /*Filter the incomming GPS data stream*/
  search_index = 0x00;               //Nothing found yet so set the search index to 0x00
  common_counter = 0;                //Start at the beginning of the serial_input_buffer
  
  /*The refreshrate of the screen will be 1 second (1000ms) so we have approximately 900ms to check the incomming gps data*/
  while(loop_timer + 900 > millis()){
    while(Serial.available()){
      serial_input_buffer[common_counter] = Serial.read();
      /*if there are more than 3 bytes received in the buffer we can start checking for the ubx header sync*/
      if(common_counter > 2 && search_index == 0x00){
        /*Check to see if we received a ubx header sync (0xB5 & 0x62)*/
        if(serial_input_buffer[common_counter - 3] == 0xB5 && serial_input_buffer[common_counter - 2] == 0x62){
          /*In case of a header sync check for NAV class*/
          if(serial_input_buffer[common_counter - 1] == 0x01){
            /*Valid message start received so check for ID 0x06*/
            if(serial_input_buffer[common_counter] == 0x06){ //Receiving message: Navigation Solution Information
              common_counter = 0;                            //Reset counter to 0
              search_index = 0x06;                           //Activate search index
            }
            /*Valid message start received so check for ID 0x03*/
            if(serial_input_buffer[common_counter] == 0x03){ //Receiving message: Receiver Navigation Status
              common_counter = 0;                            //Reset counter to 0
              search_index = 0x03;                           //Activate search index
            }
            /*Valid message start received so check for ID 0x12*/
            if(serial_input_buffer[common_counter] == 0x12){ //Receiving message: Velocity Solution in NED
              common_counter = 0;                            //Reset counter to 0
              search_index = 0x12;                           //Activate search index
            }
            /*Valid message start received so check for ID 0x02*/
            if(serial_input_buffer[common_counter] == 0x02){ //Receiving message: Geodetic Position Solution
              common_counter = 0;                            //Reset counter to 0
              search_index = 0x02;                           //Activate search index
            }
          }
        }
      }
      /*Valid message start and ID received so let's scan the rest of the message for information*/
      if(search_index > 0x00){
        /*0x06 Navigation Solution Information message*/
        if(search_index == 0x06 && common_counter == 50){
          numSV = serial_input_buffer[common_counter]; //Number of satellites used for the navigation solution
          common_counter = 0;
          search_index = 0x00;
        }
        /*0x03 Receiver Navigation Status message*/
        if(search_index == 0x03 && common_counter == 7){
          gpsFix = serial_input_buffer[common_counter];
          common_counter = 0;
          search_index = 0x00;
        }
        /*0x12 Velocity Solution in NED (North/East/Down) message*/
        if(search_index == 0x12 && common_counter == 30){
          gSpeed = ((unsigned long)serial_input_buffer[common_counter - 4] << 24) | ((unsigned long)serial_input_buffer[common_counter - 5] << 16) | ((unsigned long)serial_input_buffer[common_counter - 6] << 8) | (unsigned long)serial_input_buffer[common_counter - 7];
          gSpeed *= 0.036; //Convert cm/s to km/h
          heading = ((unsigned long)serial_input_buffer[common_counter] << 24) | ((unsigned long)serial_input_buffer[common_counter - 1] << 16) | ((unsigned long)serial_input_buffer[common_counter - 2] << 8) | (unsigned long)serial_input_buffer[common_counter - 3];
          heading /= 100000; //Remove decimal point
          common_counter = 0;
          search_index = 0x00;
        }
        /*0x02 Geodetic Position Solution message*/
        if(search_index == 0x02 && common_counter == 22){
          hMSL = ((unsigned long)serial_input_buffer[common_counter] << 24) | ((unsigned long)serial_input_buffer[common_counter - 1] << 16) | ((unsigned long)serial_input_buffer[common_counter - 2] << 8) | (unsigned long)serial_input_buffer[common_counter - 3];
          height = ((unsigned long)serial_input_buffer[common_counter- 4] << 24) | ((unsigned long)serial_input_buffer[common_counter - 5] << 16) | ((unsigned long)serial_input_buffer[common_counter - 6] << 8) | (unsigned long)serial_input_buffer[common_counter - 7];
          totHeight = (hMSL + height) / 1000; //Convert mm to meters
          lat = ((unsigned long)serial_input_buffer[common_counter - 8] << 24) | ((unsigned long)serial_input_buffer[common_counter - 9] << 16) | ((unsigned long)serial_input_buffer[common_counter - 10] << 8) | (unsigned long)serial_input_buffer[common_counter - 11];
          lat /= 10; //Remove one decimal to reduce noise
          lon = ((unsigned long)serial_input_buffer[common_counter - 12] << 24) | ((unsigned long)serial_input_buffer[common_counter - 13] << 16) | ((unsigned long)serial_input_buffer[common_counter - 14] << 8) | (unsigned long)serial_input_buffer[common_counter - 15];
          lon /= 10; //Remove one decimal to reduce noise
          common_counter = 0;
          search_index = 0x00;
        }
      }
      common_counter ++;
      if(common_counter > 99)common_counter = 0;
    }
  }
  
  //If we have more than 7 satellites in view and a 3D fix for more than 60 second set the home lock
  if(numSV > 7 && home_lock_counter < 60 && gpsFix == 0x03)home_lock_counter ++;
  else if(home_lock_counter < 60)home_lock_counter = 0;
  
  if(home_lock == 0 && home_lock_counter == 60){
    home_lon = lon;               //Set the home lon position
    home_lat = lat;               //Set the home lat position
    home_totHeight = totHeight;   //Set the current height position
    home_lock = 1;                //Home lock is set
    /*Calculate the difference in distance between two longitude points at the current latitude*/
    latitude_compensation = cos((lat / 1000000)* 0.017453);
  }
  

  if(home_lock == 1){
    /*Calculate the altitude of the multicopter*/
    flight_altitude = totHeight - home_totHeight;
    if(flight_altitude < 0){
      flight_altitude = 0;
      home_totHeight --;
    }
    /*Calculate the ground distance of the multicopter to the home point*/
    ground_distance = sqrt( pow(((float)(lat - home_lat) * (float)0.111),2) + pow(((float)(lon - home_lon) * ((float)0.111) * latitude_compensation),2));
    /*Calculate the line of sight of the multicopter to the home point*/
    line_of_sight = sqrt( pow(ground_distance,2) + pow(flight_altitude, 2));
    /*Calculate the heading to home of the multicopter to home point*/
    if(ground_distance > 20){
      /*This will work if the latitude is north and the longitude is east*/
      /*Otherwise this might not work and you need to adjust it to your own location*/
      /*The 57.2958 is phi to radians*/
      heading_to_home = 57.2958 * atan2((home_lon - lon) * latitude_compensation, home_lat - lat);
      if(heading_to_home < 0)heading_to_home += 360;
    }
    else heading_to_home = 0;
  }
  else{
   /*Keep everything to zero until home lock is set*/
   flight_altitude = 0;
   gSpeed = 0;
   ground_distance = 0;
   line_of_sight = 0;
   heading_to_home = 0;
  }
  
  /*Calculate the voltage of the battery*/
  bat_voltage = ((float)analogRead(A0) * (206.0 / 1023.0)) + 3;
  
  /*Clear the on screen display buffer*/
  for (common_counter = 0; common_counter < 480; common_counter++)
  {
    screen_buffer[common_counter] = 0x00;
  }
  
  /*Display homelock set symbol*/
  if(home_lock == 1)screen_buffer[36] = 0xC7; //Home lock symbol
  else screen_buffer[36] = 0xC8; //Dash symbol
  
  /*Display the satellite symbol and the number of satellites*/
  screen_buffer[33] = 0xC1; //Satellite symbol
  screen_buffer[34] = convert_dec(numSV/10);
  screen_buffer[35] = convert_dec(numSV%10);
  
  /*Display speed and the kph symbol*/
  screen_buffer[184] = 0xC0; //kph symbol
  screen_buffer[181] = convert_dec(gSpeed/100);
  screen_buffer[182] = convert_dec((gSpeed - ((gSpeed/100)*100)) / 10);
  screen_buffer[183] = convert_dec((gSpeed - ((gSpeed/100)*100)) % 10);
  
  /*Display course*/
  screen_buffer[42] = convert_dec(heading/100);
  screen_buffer[43] = convert_dec((heading - ((heading/100)*100)) / 10);
  screen_buffer[44] = convert_dec((heading - ((heading/100)*100)) % 10);
  
  /*Display altitude and the meter symbol*/
  screen_buffer[206] = 0xC6; //Meter symbol
  screen_buffer[203] = convert_dec(flight_altitude/100);
  screen_buffer[204] = convert_dec((flight_altitude - ((flight_altitude/100)*100)) / 10);
  screen_buffer[205] = convert_dec((flight_altitude - ((flight_altitude/100)*100)) % 10);
  
  /*Display distance and the meter symbol*/
  screen_buffer[393] = 0xC3; //Ground distance symbol
  screen_buffer[394] = convert_dec(ground_distance/1000);
  ground_distance -= (ground_distance/1000) * 1000;
  screen_buffer[395] = convert_dec(ground_distance / 100);
  ground_distance -= (ground_distance/100) * 100;
  screen_buffer[396] = convert_dec(ground_distance / 10);
  screen_buffer[397] = convert_dec(ground_distance % 10);
  screen_buffer[398] = 0xC6; //Meter symbol
  
  /*Display home course and the home symbol*/
  screen_buffer[402] = 0xC5; //Home course symbol
  screen_buffer[403] = convert_dec(heading_to_home/100);
  screen_buffer[404] = convert_dec((heading_to_home - ((heading_to_home/100)*100)) / 10);
  screen_buffer[405] = convert_dec((heading_to_home - ((heading_to_home/100)*100)) % 10);
  
  /*Display line of sight*/
  screen_buffer[409] = 0xC4; //Line of sight symbol
  screen_buffer[410] = convert_dec(line_of_sight/1000);
  line_of_sight -= (line_of_sight/1000) * 1000;
  screen_buffer[411] = convert_dec(line_of_sight / 100);
  line_of_sight -= (line_of_sight/100) * 100;
  screen_buffer[412] = convert_dec(line_of_sight / 10);
  screen_buffer[413] = convert_dec(line_of_sight % 10);
  screen_buffer[414] = 0xC6;
  
  /*Display bat voltage*/
  screen_buffer[50] = 0xC2; //Battery symbol
  screen_buffer[51] = convert_dec(bat_voltage / 100);
  bat_voltage -= (bat_voltage/100) * 100;
  screen_buffer[52] = convert_dec(bat_voltage / 10);
  screen_buffer[53] = 0xC9; //Dot symbol
  screen_buffer[54] = convert_dec(bat_voltage % 10);
  
  /*Write the new screen*/
  write_new_screen();
}

//////////////////////////////////////////////////////////////
byte spi_transfer(byte adress, byte data)
{
  SPDR = adress;                    //Load SPI data register
  while (!(SPSR & (1<<SPIF)));      //Wait until the byte is send
  SPDR = data;                      //Load SPI data register
  while (!(SPSR & (1<<SPIF)));      //Wait until the byte is send
}

//////////////////////////////////////////////////////////////
int convert_dec (int decimal)
{
  int convert_dec;
  if (decimal == 0) convert_dec = 0xD0;
  if (decimal == 1) convert_dec = 0xD1;
  if (decimal == 2) convert_dec = 0xD2;
  if (decimal == 3) convert_dec = 0xD3;
  if (decimal == 4) convert_dec = 0xD4;
  if (decimal == 5) convert_dec = 0xD5;
  if (decimal == 6) convert_dec = 0xD6;
  if (decimal == 7) convert_dec = 0xD7;
  if (decimal == 8) convert_dec = 0xD8;
  if (decimal == 9) convert_dec = 0xD9;
  return (convert_dec);
}
//////////////////////////////////////////////////////////////
void write_new_screen()
{
  /*Declare variables*/
  int local_count;
  
  local_count = 479;
  
  // clear the screen
  digitalWrite(6,LOW); //Enable the MAX7456
  spi_transfer(0x04, 0x04); //Clear the display
  digitalWrite(6,HIGH); //Disable the MAX7456

  // disable display
  digitalWrite(6,LOW); //Enable the MAX7456
  spi_transfer(0x00, 0x40); //Disable the on screen display
  spi_transfer(0x04, 0x01); //Auto increment mode for writing the display characters
  spi_transfer(0x05, 0x00); //Set start address high to the position 0
  spi_transfer(0x06, 0x00); //Set start address low to the position 0

  common_counter = 0;
  while(local_count) // write out full screen
  {
    //screen_char = screen_buffer[common_counter];
    spi_transfer(0x07, screen_buffer[common_counter]); //Load the character in the next buffer of the MAX7456
    common_counter++;
    local_count--;
  }

  spi_transfer(0x07, 0xFF); //Disable the auto increment mode

  spi_transfer(0x00, 0x4C); // Turn on the on screen display
  digitalWrite(6,HIGH); //Disable the MAX7456
}

