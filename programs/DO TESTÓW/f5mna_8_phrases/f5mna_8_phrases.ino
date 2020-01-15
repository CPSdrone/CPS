

//F5MNA    Site http://f5mna.free.fr
//Génération de 8 phrases au choix
// Cde entrees digitale 5,6 et 7


#define DATAOUT 11//MOSI
#define DATAIN  12//MISO
#define SPICLOCK  13//sck
#define MAX7456SELECT 10//ss
#define VSYNC 2// INT0

//MAX7456 opcodes
#define DMM_reg   0x04
#define DMAH_reg  0x05
#define DMAL_reg  0x06
#define DMDI_reg  0x07
#define VM0_reg   0x00
#define VM1_reg   0x01

// video mode register 0 bits
#define VIDEO_BUFFER_DISABLE 0x01
#define MAX7456_RESET 0x02
#define VERTICAL_SYNC_NEXT_VSYNC 0x04
#define OSD_ENABLE 0x08
#define SYNC_MODE_AUTO 0x00
#define SYNC_MODE_INTERNAL 0x30
#define SYNC_MODE_EXTERNAL 0x20
#define VIDEO_MODE_PAL 0x40
#define VIDEO_MODE_NTSC 0x00

// video mode register 1 bits
// duty cycle is on_off
#define BLINK_DUTY_CYCLE_50_50 0x00
#define BLINK_DUTY_CYCLE_33_66 0x01
#define BLINK_DUTY_CYCLE_25_75 0x02
#define BLINK_DUTY_CYCLE_75_25 0x03

// blinking time
#define BLINK_TIME_0 0x00
#define BLINK_TIME_1 0x04
#define BLINK_TIME_2 0x08
#define BLINK_TIME_3 0x0C

// background mode brightness (percent)
#define BACKGROUND_BRIGHTNESS_0 0x00
#define BACKGROUND_BRIGHTNESS_7 0x01
#define BACKGROUND_BRIGHTNESS_14 0x02
#define BACKGROUND_BRIGHTNESS_21 0x03
#define BACKGROUND_BRIGHTNESS_28 0x04
#define BACKGROUND_BRIGHTNESS_35 0x05
#define BACKGROUND_BRIGHTNESS_42 0x06
#define BACKGROUND_BRIGHTNESS_49 0x07

#define BACKGROUND_MODE_GRAY 0x40

//MAX7456 commands
#define CLEAR_display 0x04
#define CLEAR_display_vert 0x06
#define END_string 0xff

// with PAL
// all VM0_reg commands need bit 6 set
#define ENABLE_display 0x48
#define ENABLE_display_vert 0x4c
#define MAX7456_reset 0x42
#define DISABLE_display 0x40


#define WHITE_level_80 0x03
#define WHITE_level_90 0x02
#define WHITE_level_100 0x01
#define WHITE_level_120 0x00

// with PAL
#define MAX_screen_size 480
#define MAX_screen_rows 16


//////////////////////////////////////////////////////////////
void setup()
{
  byte spi_junk, eeprom_junk;
  int x;
  Serial.begin(9600);
  Serial.flush();

  pinMode(MAX7456SELECT,OUTPUT);
  digitalWrite(MAX7456SELECT,HIGH); //disable device

  pinMode(DATAOUT, OUTPUT);
  pinMode(DATAIN, INPUT);
  pinMode(SPICLOCK,OUTPUT);
  pinMode(VSYNC, INPUT);

  // SPCR = 01010000
  //interrupt disabled,spi enabled,msb 1st,master,clk low when idle,
  //sample on leading edge of clk,system clock/4 rate (4 meg)
  SPCR = (1<<SPE)|(1<<MSTR);
  spi_junk=SPSR;
  spi_junk=SPDR;
  delay(250);

  // force soft reset on Max7456
  digitalWrite(MAX7456SELECT,LOW);
  spi_transfer(VM0_reg);
  spi_transfer(MAX7456_reset);
  digitalWrite(MAX7456SELECT,HIGH);
  delay(500);

  // set all rows to same charactor white level, 90%
  digitalWrite(MAX7456SELECT,LOW);
  for (x = 0; x < MAX_screen_rows; x++)
  {
    spi_transfer(x + 0x10);
    spi_transfer(WHITE_level_90);
  }

  // make sure the Max7456 is enabled
  spi_transfer(VM0_reg);
  spi_transfer(VERTICAL_SYNC_NEXT_VSYNC|OSD_ENABLE);
  digitalWrite(MAX7456SELECT,HIGH);
  delay(100);

  spi_transfer(VM1_reg);
  spi_transfer(BLINK_DUTY_CYCLE_50_50|BLINK_TIME_3);
  digitalWrite(MAX7456SELECT,HIGH);

  Serial.println("OSD starting");
  delay(100);
}
 /////////////////////////////////////////////////////////////
void Selec_phrases()
{
    //  SELECTION de 8 PHRASES
  //-------------------------
   int inter1 = 5;
   int inter2 = 6;
   int inter3 = 7;
   
   int val1=digitalRead(inter1);
   int val2=digitalRead(inter2);
   int val3=digitalRead(inter3);
   
   pinMode(inter1,INPUT);
   pinMode(inter2,INPUT);
   pinMode(inter3,INPUT);
   {
   
   char phrase1[] =  "   MONT POUPET - JN26WX -    "; 
   char phrase2[] =  "  F5MNA Granges-sur-Baume    ";          
   char phrase3[] =  "  Mon site : f5mna.free.fr   ";
   char phrase4[] =  "    73 QRO de F5MNA          ";
   char phrase5[] =  "      Operateur: Alain       ";
   char phrase6[] =  "      Ceci est un test       ";
   char phrase7[] =  "    via relais du Poupet     ";
   char phrase8[] =  "                             ";  

 if ((val1==LOW) and (val2==LOW) and (val3==LOW))
      OSD_write_to_screen(phrase1, 1, 7, 0,1);
  if ((val1==LOW) and (val2==LOW) and (val3==HIGH))
      OSD_write_to_screen(phrase2, 1, 7, 0,0);
  if ((val1==LOW) and (val2==HIGH) and (val3==LOW))
      OSD_write_to_screen(phrase3, 1, 7, 0,0);
  if ((val1==LOW) and (val2==HIGH) and (val3==HIGH))
      OSD_write_to_screen(phrase4, 1, 7, 0,1);
  if ((val1==HIGH) and (val2==LOW) and (val3==LOW))
      OSD_write_to_screen(phrase5, 1, 7, 0,1);
  if ((val1==HIGH) and (val2==LOW) and (val3==HIGH))
      OSD_write_to_screen(phrase6, 1, 7, 1,1);
  if ((val1==HIGH) and (val2==HIGH) and (val3==LOW))
      OSD_write_to_screen(phrase7, 1, 7, 0,0);
  if ((val1==HIGH) and (val2==HIGH) and (val3==HIGH))
      OSD_write_to_screen(phrase8, 1, 7, 0,0);  
   }      
    delay(10); 
   }  


//////////////////////////////////////////////////////////////
void Max7456 (){
//////////////////////////////////////////////////////////////

spi_transfer(VM0_reg);
spi_transfer(VERTICAL_SYNC_NEXT_VSYNC|OSD_ENABLE|SYNC_MODE_AUTO);
digitalWrite(MAX7456SELECT,HIGH); 
}
byte convert_ascii (int character)
{
// Transposition de la table ASCII (non standard) integrée à l'EEPROM du MAX7456

 byte lookup_char;

  if (character == 32)
    lookup_char = 0x00; // blank space
  else if (character == 48)
    lookup_char = 0x0a; // 0
  else if ((character > 48) && (character < 58))
    lookup_char = (character - 48); // 1-9
  else if ((character > 64) && (character < 91))
    lookup_char = (character - 54); // A-Z
  else if ((character > 96) && (character < 123))
    lookup_char = (character - 60); // a-z
  else if (character == 34)
    lookup_char = 0x48; // "
  else if (character == 39)
    lookup_char = 0x46; // '
  else if (character == 40)
    lookup_char = 0x3f; // (
  else if (character == 41)
    lookup_char = 0x40; // )
  else if (character == 44)
    lookup_char = 0x45; // ,
  else if (character == 45)
    lookup_char = 0x49; // -
  else if (character == 46)
    lookup_char = 0x41; // .
  else if (character == 47)
    lookup_char = 0x47; // /
  else if (character == 58)
    lookup_char = 0x44; // :
  else if (character == 59)
    lookup_char = 0x43; // ;
  else if (character == 60)
    lookup_char = 0x4a; // <
  else if (character == 62)
    lookup_char = 0x4b; // >
  else if (character == 63)
    lookup_char = 0x42; // ?
  else if (character == 64)
    lookup_char = 0x4c; // @
  else
    lookup_char = 0x00; // out of range, blank space

 return (lookup_char);

}
//////////////////////////////////////////////////////////////
byte spi_transfer(volatile byte data)
{
  SPDR = data;			  // Start the transmission
  while (!(SPSR & (1<<SPIF)))     // Wait the end of the transmission
  {
  };
  return SPDR;			  // return the received byte
}




// ============================================================   WRITE TO SCREEN
void OSD_write_to_screen(const char s[], byte x, byte y, byte blink, byte invert){
  
  unsigned int linepos;
  byte local_count;
  byte settings, char_address_hi, char_address_lo;
  byte screen_char;
  
  local_count = 0;
  
  char_address_hi = 0;
  char_address_lo = 0;
  
  // convert x,y to line position
  linepos = y*30+x;

  // divide in to hi & lo byte
  char_address_hi = linepos >> 8;
  char_address_lo = linepos;

  // clear the screen
  //OSD_clear();

  settings = B00000001;

  // set blink bit
  if (blink) {
    settings |= (1 << 4);	 // forces nth bit of x to be 1.  all other bits left alone.
    //x &= ~(1 << n);	// forces nth bit of x to be 0.  all other bits left alone.
  }
  // set invert bit
  if (invert){
    settings |= (1 << 3);	 // forces nth bit of x to be 1.  all other bits left alone.
  }


  digitalWrite(MAX7456SELECT,LOW);

  spi_transfer(DMM_reg); //dmm
  spi_transfer(settings);

  spi_transfer(DMAH_reg); // set start address high
  spi_transfer(char_address_hi);

  spi_transfer(DMAL_reg); // set start address low
  spi_transfer(char_address_lo);

  while(s[local_count]!='\0') // write out full screen
  {
    screen_char = s[local_count];
    screen_char= convert_ascii(screen_char);
    spi_transfer(DMDI_reg);
    spi_transfer(screen_char);
    local_count++;
  }
  
  spi_transfer(DMDI_reg);
  spi_transfer(END_string);

  spi_transfer(VM0_reg); // turn on screen next vertical. En transparence sur écran
  spi_transfer(ENABLE_display_vert);
  digitalWrite(MAX7456SELECT,HIGH);
  }
    
  void loop (){
  Selec_phrases();
  Max7456();
  }  
