/*
    MAX7456 test code
	Pete Dokter, 6/17/09
	
	This test code blinks the word "SAMPLE" just under
	where you'd expect someone's face to be.
	
*/

#include <avr/io.h>
#include <avr/interrupt.h>


#define RESET 0	//PB0
#define CS 2	//PB2
#define MOSI 3	//PB3
#define MISO 4	//PB4
#define SCK 5	//PB5


//Define functions
//======================
void ioinit(void);      //Initializes IO
void delay_ms(uint16_t x); //General purpose delay
void delay_us(int x);
void SPI_write(char address, char byte);
char SPI_read(char address);

//======================




int main (void)
{
	char x;

    ioinit(); //Setup IO pins and defaults
	
	delay_ms(1000);
	
	//reset
	PORTB &= ~(1<<RESET);
	delay_ms(1000);
	PORTB |= (1<<RESET);
	delay_ms(1000);
	
	SPI_write(0,0x08);	//enable display of OSD image
	delay_ms(1);
	
	//automatic black level control, have to read, augment and rewrite
	//The data sheet is rather specific about this
	x = SPI_read(0xEC);	
	delay_ms(1);
	x &= 0xEF;
	SPI_write(0x6C,x);
	delay_ms(1);
	
	SPI_write(0x04,0);//DMM set to 0
	
	x = 25;
	
	SPI_write(0x05,0x01);//DMAH
	
	SPI_write(0x06,x);//DMAL
	SPI_write(0x07,0x1D);
	
	SPI_write(0x06,x+1);//DMAL
	SPI_write(0x07,0x0B);
	
	SPI_write(0x06,x+2);//DMAL
	SPI_write(0x07,0x17);
	
	SPI_write(0x06,x+3);//DMAL
	SPI_write(0x07,0x1A);
	
	SPI_write(0x06,x+4);//DMAL
	SPI_write(0x07,0x16);
	
	SPI_write(0x06,x+6);//DMAL
	SPI_write(0x07,0x0F);
	
	
	while(1)
	{
		SPI_write(0,0x08);	//enable display of OSD image
		delay_ms(1000);
		SPI_write(0,0);	//disable display of OSD image
		delay_ms(1000);
	}
	
	
}

void ioinit (void)
{
	PORTB = 0xFF;
	DDRB = ((1<<CS) | (1<<MOSI) | (1<<SCK) | (1<<RESET));

    TCCR2B = (1<<CS21); //Set Prescaler to 8. CS21=1

}

//General short delays
void delay_us(int x)
{
	int y, z, a;
	
	y = x/256;
	z = x - y * 256;
	
	for (a = 0; a < y; a++)
	{
		TIFR2 |= 0x01;//Clear any interrupt flags on Timer2
		
		TCNT2 = 0; //256 - 125 = 131 : Preload timer 2 for x clicks. Should be 1us per click
	
		while(!(TIFR2 & 0x01));
		
	}
	
	TIFR2 |= 0x01;//Clear any interrupt flags on Timer2
	
	TCNT2 = 256-z; //256 - 125 = 131 : Preload timer 2 for x clicks. Should be 1us per click

	while(!(TIFR2 & 0x01));
	
}

//General short delays
void delay_ms(uint16_t x)
{
	for (; x > 0 ; x--)
    {
        delay_us(250);
        delay_us(250);
        delay_us(250);
        delay_us(250);
    }
	
}

void SPI_write(char address, char byte)
{
	unsigned char SPICount;                                       // Counter used to clock out the data

	unsigned char SPIData;                                        // Define a data structure for the SPI data
	
	PORTB |= (1<<CS);                                        		// Make sure we start with active-low CS high
	PORTB &= ~(1<<SCK);                                       		// and CK low
	
	SPIData = address;                                            // Preload the data to be sent with Address
	PORTB &= ~(1<<CS);                                           // Set active-low CS low to start the SPI cycle 

	
	for (SPICount = 0; SPICount < 8; SPICount++)                  // Prepare to clock out the Address byte
	{
		if (SPIData & 0x80) PORTB |= (1<<MOSI);           				// Check for a 1 and set the MOSI line appropriately

		else PORTB &= ~(1<<MOSI);
		
		PORTB |= (1<<SCK);                                         // Toggle the clock line
		PORTB &= ~(1<<SCK);
		
		SPIData <<= 1;                                              // Rotate to get the next bit
	}                                                             // and loop back to send the next bit
														
																// Repeat for the Data byte
	SPIData = byte;                                            // Preload the data to be sent with Data
	
	for (SPICount = 0; SPICount < 8; SPICount++)
	{
		if (SPIData & 0x80) PORTB |= (1<<MOSI);

		else PORTB &= ~(1<<MOSI);
		
		PORTB |= (1<<SCK); 
		PORTB &= ~(1<<SCK);
		
		SPIData <<= 1; 
	}           
	
	PORTB |= (1<<CS);
	PORTB &= ~(1<<MOSI);

}


char SPI_read(char address)
{
	unsigned char SPICount;                                       // Counter used to clock out the data
  
	char SPIData; 
	char temp;
	
	PORTB |= (1<<CS);                                        		// Make sure we start with active-low CS high
	PORTB &= ~(1<<SCK);                                       		// and CK low
	
	SPIData = address;                                            // Preload the data to be sent with Address
	PORTB &= ~(1<<CS);                                           // Set active-low CS low to start the SPI cycle 

	
	for (SPICount = 0; SPICount < 8; SPICount++)                  // Prepare to clock out the Address byte
	{
		if (SPIData & 0x80) PORTB |= (1<<MOSI);           				// Check for a 1 and set the MOSI line appropriately

		else PORTB &= ~(1<<MOSI);
		
		PORTB |= (1<<SCK);                                         // Toggle the clock line
		PORTB &= ~(1<<SCK);
		
		SPIData <<= 1;                                              // Rotate to get the next bit
	}                                                             // and loop back to send the next bit
	

	PORTB &= ~(1<<MOSI);                                         // Reset the MOSI data line
	
	SPIData = 0;
	
	for (SPICount = 0; SPICount < 8; SPICount++)                  // Prepare to clock in the data to be read
	{
		SPIData <<=1;                                               // Rotate the data
		PORTB |= (1<<SCK);                                         // Raise the clock to clock the data out of the MAX7456
		
		temp = PINB;
		if (temp & (1<<MISO)) SPIData |= 1;                       // Read the data bit
		
		PORTB &= ~(1<<SCK);                                       // Drop the clock ready for the next bit
	}                                                             // and loop back
	PORTB |= (1<<CS);                                                 // Raise CS
					  
	return SPIData;                              // Finally return the read data


}






