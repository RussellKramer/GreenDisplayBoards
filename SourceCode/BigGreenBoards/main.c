#include "p18f4550.h"

#include "pixelgirl.h"


#pragma config PLLDIV   = 5         // 20Mhz external oscillator
#pragma config CPUDIV   = OSC1_PLL2   
#pragma config USBDIV   = 2         // Clock source from 96MHz PLL/2
#pragma config FOSC     = HSPLL_HS
#pragma config IESO     = OFF
#pragma config PWRT     = OFF
#pragma config BOR      = OFF
#pragma config BORV     = 3
#pragma config VREGEN   = ON
#pragma config WDT      = OFF
#pragma config WDTPS    = 128
#pragma config MCLRE    = ON
#pragma config LPT1OSC  = OFF
#pragma config PBADEN   = OFF
#pragma config CCP2MX   = ON
#pragma config STVREN   = ON
#pragma config LVP      = OFF
#pragma config ICPRT    = OFF
#pragma config XINST    = OFF
#pragma config CP0      = OFF
#pragma config CP1      = OFF
#pragma config CP2      = OFF
#pragma config CP3      = OFF
#pragma config CPB      = OFF
#pragma config CPD      = OFF
#pragma config WRT0     = OFF
#pragma config WRT1     = OFF
#pragma config WRT2     = OFF
#pragma config WRT3     = OFF
#pragma config WRTB     = OFF
#pragma config WRTC     = OFF
#pragma config WRTD     = OFF
#pragma config EBTR0    = OFF
#pragma config EBTR1    = OFF
#pragma config EBTR2    = OFF
#pragma config EBTR3    = OFF
#pragma config EBTRB    = OFF

//#include <spi.h>
#include <adc.h> 

#define INDICATOR1 PORTCbits.RC1
#define INDICATOR2 PORTDbits.RD1
#define INDICATOR3 PORTDbits.RD0

#define NPN1 PORTBbits.RB5
#define NPN2 PORTBbits.RB4
#define NPN3 PORTBbits.RB3
#define NPN4 PORTBbits.RB2
#define NPN5 PORTDbits.RD7
#define NPN6 PORTDbits.RD6
#define NPN7 PORTDbits.RD5
#define NPN8 PORTDbits.RD4


#define NPN1_ON PORTB = PORTB | 0b00100000//PORTBbits.RB5
#define NPN2_ON PORTB = PORTB | 0b00010000//PORTBbits.RB4
#define NPN3_ON PORTB = PORTB | 0b00001000//PORTBbits.RB3
#define NPN4_ON PORTB = PORTB | 0b00000100//PORTBbits.RB2
#define NPN5_ON PORTD = PORTD | 0b10000000//PORTDbits.RD7
#define NPN6_ON PORTD = PORTD | 0b01000000 //PORTDbits.RD6
#define NPN7_ON PORTD = PORTD | 0b00100000//PORTDbits.RD5
#define NPN8_ON PORTD = PORTD | 0b00010000//PORTDbits.RD4



#define NPN_OFF PORTB = PORTB & 0b11000011;\
                PORTD = PORTD & 0b00001111


//#define CLOCK PORTDbits.RD2
//#define DATA1 PORTDbits.RD3
//#define DATA2 PORTDbits.RD0
//#define DATA3 PORTAbits.RA4
//#define DATA4 PORTAbits.RA5
//#define DATA5 PORTEbits.RB1
//#define DATA6 PORTEbits.RE1


#define CLOCK_PULSE PORTD = PORTD|0b00000110;\
					Delay100TCYx(1);\
					PORTD = PORTD&0b11111001;\
					Delay100TCYx(1);
					
					


#define DATA1_ON PORTD = PORTD & 0b11110111//PORTDbits.RD3 = 
#define DATA2_ON PORTD = PORTD & 0b11111110//PORTAbits.RD0
#define DATA3_ON PORTA = PORTA & 0b11101111 //PORTAbits.RA4
#define DATA4_ON PORTA = PORTA & 0b11011111 //PORTAbits.RA5
#define DATA5_ON PORTA = PORTA & 0b11111011 //PORTEbits.RB1//A2
#define DATA6_ON PORTA = PORTA & 0b11110111 //RA3


#define DATA1_OFF PORTD = PORTD | 0b00001000//PORTDbits.RD3 = 
#define DATA2_OFF PORTD = PORTD | 0b00000001//PORTAbits.RD0
#define DATA3_OFF PORTA = PORTA | 0b00010000 //PORTAbits.RA4
#define DATA4_OFF PORTA = PORTA | 0b00100000 //PORTAbits.RA5
#define DATA5_OFF PORTA = PORTA | 0b00000100 //PORTEbits.RB1//A2
#define DATA6_OFF PORTA = PORTA | 0b00001000 //RA3






unsigned int channel[2][7] = {{0,20,40,60,80,100,120},{120,100,80,60,40,20,0}};

unsigned int total;
unsigned int Htotal;
unsigned char MaxColor;
#define RESET PORTCbits.RC0
#define STROBE PORTCbits.RC2


#define  RIGHT ADC_CH0
#define  LEFT  ADC_CH1  

unsigned int loadADCchannel(unsigned char channel){
	 SetChanADC (channel);//was 11
   	   //Delay10TCYx(10);
       ConvertADC ();	// Start an A/D conversion				
       while(BusyADC()){	// Wait for the conversion to complete
       //maybe some code for while were waiting (could turn an led on maybe)
        }
       // Delay10TCYx(10);
        return  ReadADC();
}
	

void LoadVolume(){
   unsigned char i,j;
   RESET = 1;
   // Delay10TCYx(10);
   STROBE=1;
   //Delay10TCYx(10);
   STROBE=0;
  // Delay10TCYx(10); 
   
   RESET = 0;

 //STROBE=1;
  // Delay10TCYx(100); 
   
  
  
   for(i=0; i< 7; i++){
	  	STROBE=1;
   		Delay10TCYx(10);
        STROBE=0;
        Delay10TCYx(10); 
	    

        channel[0][i] = loadADCchannel(RIGHT);
        channel[1][i] = loadADCchannel(RIGHT);
	}
}






unsigned int safesub(unsigned int a, unsigned int b){
if(b>a)
  return 0;
return a-b;	
}

unsigned char SafeByte(unsigned int input){
   if(input > 255)
      return 255;
      return input;	
	
}
	
	/*
unsigned char matrix[48][3] =  {

{0b00011111 ,0b11110000 ,0b00000000},//0
{0b00011111 ,0b11110000 ,0b00000000},//1
{0b00011111 ,0b11110000 ,0b00000000},//2

{0b00000000 ,0b01111110 ,0b00000000},//3
{0b00000000 ,0b01111110 ,0b00000000},//4
{0b00000000 ,0b01111110 ,0b00000000},//5

{0b00011111 ,0b11111111 ,0b11000111},//6
{0b00011111 ,0b11111111 ,0b11000111},//7
{0b00011111 ,0b11111111 ,0b11000111},//8

{0b11100011 ,0b11110001 ,0b11111000},//9
{0b11100011 ,0b11110001 ,0b11111000},//10
{0b11100011 ,0b11110001 ,0b11111000},//11

{0b11100011 ,0b11111111 ,0b11000000},//12
{0b11100011 ,0b11111111 ,0b11000000},//13
{0b11100011 ,0b11111111 ,0b11000000},//14

{0b00000011 ,0b11111111 ,0b11000000},//15
{0b00000011 ,0b11111111 ,0b11000000},//16
{0b00000011 ,0b11111111 ,0b11000000},//17

{0b11100011 ,0b11111111 ,0b11000000},//18
{0b11100011 ,0b11111111 ,0b11000000},//19
{0b11100011 ,0b11111111 ,0b11000000},//20

{0b11100011 ,0b11110001 ,0b11111000},//21
{0b11100011 ,0b11110001 ,0b11111000},//22
{0b11100011 ,0b11110001 ,0b11111000},//23

{0b00011111 ,0b11111111 ,0b11000111},//24
{0b00011111 ,0b11111111 ,0b11000111},//25
{0b00011111 ,0b11111111 ,0b11000111},//26

{0b00000000 ,0b01111110 ,0b00000000},//27
{0b00000000 ,0b01111110 ,0b00000000},//28
{0b00000000 ,0b01111110 ,0b00000000},//29

{0b00011111 ,0b11110000 ,0b00000000},//30
{0b00011111 ,0b11110000 ,0b00000000},//31
{0b00011111 ,0b11110000 ,0b00000000},//32
								
								
								
								{0b00000000,0b00000000,0b00000000},//33
								{0b00000000,0b00000000,0b00000000},//34
								{0b00000000,0b00000000,0b00000000},//35
								{0b00000000,0b00000000,0b00000000},//36
								{0b00000000,0b00000000,0b00000000},//37
								{0b00000000,0b00000000,0b00000000},//38
								{0b00000000,0b00000000,0b00000000},//39
								{0b00000000,0b00000000,0b00000000},//40
								{0b00000000,0b00000000,0b00000000},//41
								{0b00000000,0b00000000,0b00000000},//42
								{0b00000000,0b00000000,0b00000000},//43
								{0b00000000,0b00000000,0b00000000},//44
								{0b00000000,0b00000000,0b00000000},//45
								{0b00000000,0b00000000,0b00000000},//46
								{0b00000000,0b00000000,0b00000000}};//47
						


	



*/

/*
unsigned char matrix[48][3] =  {

{0b11111111 ,0b11111111 ,0b11111111},//0
{0b11111111 ,0b11111111 ,0b11111111},//1
{0b11111111 ,0b11111111 ,0b11111111},//2

{0b11111111 ,0b11111111 ,0b11111111},//3
{0b11111111 ,0b11111111 ,0b11111111},//4
{0b11111111 ,0b11111111 ,0b11111111},//5

{0b11111111 ,0b11111111 ,0b11111111},//6
{0b11111111 ,0b11111111 ,0b11111111},//7
{0b11111111 ,0b11111111 ,0b11111111},//8

{0b11111111 ,0b11111111 ,0b11111111},//9
{0b11111111 ,0b11111111 ,0b11111111},//10
{0b11111111 ,0b11111111 ,0b11111111},//11

{0b11111111 ,0b11111111 ,0b11111111},//12
{0b11111111 ,0b11111111 ,0b11111111},//13
{0b11111111 ,0b11111111 ,0b11111111},//14

{0b11111111 ,0b11111111 ,0b11111111},//15
{0b11111111 ,0b11111111 ,0b11111111},//16
{0b11111111 ,0b11111111 ,0b11111111},//17

{0b11111111 ,0b11111111 ,0b11111111},//18
{0b11111111 ,0b11111111 ,0b11111111},//19
{0b11111111 ,0b11111111 ,0b11111111},//20

{0b11111111 ,0b11111111 ,0b11111111},//21
{0b11111111 ,0b11111111 ,0b11111111},//22
{0b11111111 ,0b11111111 ,0b11111111},//23

{0b11111111 ,0b11111111 ,0b11111111},//24
{0b11111111 ,0b11111111 ,0b11111111},//25
{0b11111111 ,0b11111111 ,0b11111111},//26

{0b11111111 ,0b11111111 ,0b11111111},//27
{0b11111111 ,0b11111111 ,0b11111111},//28
{0b11111111 ,0b11111111 ,0b11111111},//29

{0b11111111 ,0b11111111 ,0b11111111},//30
{0b11111111 ,0b11111111 ,0b11111111},//31
{0b11111111 ,0b11111111 ,0b11111111},//32
								
								
								
							{0b11111111 ,0b11111111 ,0b11111111},//33
							{0b11111111 ,0b11111111 ,0b11111111},//34
							{0b11111111 ,0b11111111 ,0b11111111},//35
							{0b11111111 ,0b11111111 ,0b11111111},//36
							{0b11111111 ,0b11111111 ,0b11111111},//37
							{0b11111111 ,0b11111111 ,0b11111111},//38
							{0b11111111 ,0b11111111 ,0b11111111},//39
							{0b11111111 ,0b11111111 ,0b11111111},//40
							{0b11111111 ,0b11111111 ,0b11111111},//41
							{0b11111111 ,0b11111111 ,0b11111111},//42
							{0b11111111 ,0b11111111 ,0b11111111},//43
							{0b11111111 ,0b11111111 ,0b11111111},//44
							{0b11111111 ,0b11111111 ,0b11111111},//45
							{0b11111111 ,0b11111111 ,0b11111111},//46
								{0b00000000,0b00000000,0b00000000}};//47
						

*/

	
	
	 #pragma udata gpr4 //linker script sets for 512 bytes


unsigned char Asequence[24][8];
unsigned char Dsequence[24][8];
 #pragma udata
//convert matrix to sequence to pump out to shift registers
void MatrixToSequence(unsigned char stage){
	unsigned char level,shifter,i,counter;
	
	
	for(level = 0; level< 8; level++){
		counter  = 0;	
		for(i=0; i<3; i++){//repeat for all three bytes
			for(shifter=0b10000000; shifter!= 0 ; shifter = shifter >> 1){ //repeat for all eight bits
			
				Asequence[counter][level] = 255;//PORTA;
				Dsequence[counter][level] = 0b00001111;
			
				if(pixelgirl[stage][level   ][i] & shifter)
					Dsequence[counter][level] &=  0b11110111; //DATA1_ON;

				if(pixelgirl[stage][level+8][i] & shifter)
					 Dsequence[counter][level] &=  0b11111110;
	
				if(pixelgirl[stage][level+16][i] & shifter)
					Asequence[counter][level] &=  0b11101111;
	
				if(pixelgirl[stage][level+24][i] & shifter)
					Asequence[counter][level] &=  0b11011111;
	
				if(pixelgirl[stage][level+32][i] & shifter)
					Asequence[counter][level] 	&= 0b11111011;
		
				if(pixelgirl[stage][level+40][i] & shifter)
					Asequence[counter][level] &= 0b11110111;
	
				counter++;
			} //level loop
		} //i for 3 shift register loop
	} //shifter byte loop	
}
	

void ClockInSequence(unsigned char level){
	unsigned char shifter,i;
	

	
	//static unsigned char Asequence[24];
	//static unsigned char Dsequence[24];
	//unsigned char counter = 0;
	

	


	NPN_OFF;
//	Delay100TCYx(100);
	for(i=0; i<24; i++){
		PORTA = 	Asequence[i][level];
		PORTD = 	Dsequence[i][level];
		PORTD &= 0b11111001;
	}	

		switch(level){
		case 0:
			NPN8_ON;
		break;
		case 1:
			NPN7_ON;
		break;
		case 2:
		    NPN6_ON;
		break;
		case 3:
			NPN5_ON;
		break;
		case 4:
			NPN4_ON;
		break;
		case 5:
			NPN3_ON;
		break;
		case 6:
			NPN2_ON;
		break;
		case 7:
			NPN1_ON;
		break;
	}	
	
	
}
	
void main(void){

    unsigned char coloumn, row,band,i,j,counter=0,girlstage;
    
    unsigned char temp[3];
    unsigned int a,b;

    
    TRISE = 0;
   	TRISD = 0;
	TRISC = 0;
	TRISB = 0;
    TRISA = 0;
    PORTD = 0;
	PORTC = 0;
	PORTB = 0;
	PORTA = 0;
	PORTE = 0;

  	OSCTUNE = 0b10001111; 
	OSCCON  = 0b01110110;
	
	ADCON1 = 0b00001111;

//	MatrixToSequence();
	
	while(1){
		
		#if 0
			while(1){
			INDICATOR1 = 1;
			for(i = 0; i< 200; i++){
				Delay100TCYx(200);
			}
			INDICATOR1 = 0;
			for(i = 0; i< 200; i++){
				Delay100TCYx(200);
			}
			}

		#endif
	
	/*
	counter++;
	if(counter > 3){
		counter  = 0;
	    	
	    	temp[0] = matrix[0][0];
	    	temp[1] = matrix[0][1];
	    	temp[2] = matrix[0][2];
	    		
	    	for(i=0; i< 47; i++){
		    	for(j=0; j< 3; j++){
		    		matrix[i][j] = matrix[i+1][j];
		    	}
			}
			
			matrix[47][0] = temp[0];
	    	matrix[47][1] = temp[1];
	    	matrix[47][2] = temp[2];
	    		
	    		
	}
		*/
		
		girlstage++;
		if(girlstage >= PgirlFrames)
			girlstage = 0;
		MatrixToSequence(girlstage);
		for(counter = 0; counter < 3; counter++){
		for(i=0; i< 8; i++){
			ClockInSequence(i);

			Delay100TCYx(40);

		
				
	}
		}
		

    }
}