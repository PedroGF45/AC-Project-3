#include <reg51.h>

#define tenthSecond					2 //time to count 0,1 seconds (2 x 50ms)
#define second 							20 // time to count 1 second (20 x 50ms)

// values for display
#define null							 	0xFF	
#define zeroDot							0x02	
#define oneDot							0x9E	
#define twoDot							0x24	
#define threeDot						0x0C	
#define fourDot							0x98	
#define fiveDot							0x48	
#define sixDot							0x40	
#define sevenDot						0x1E	
#define eightDot						0x00	
#define nineDot							0x08	
#define hifenDot						0xFC	
#define zero								0x03	
#define one									0x9F	
#define two									0x25	
#define three								0x0D	
#define four								0x99	
#define five								0x49	
#define six									0x41	
#define seven								0x1F	
#define eight								0x01	
#define nine								0x09  
#define hifen								0xFD	
#define a										0x11	
#define b										0xC1	
#define c										0x63 	
#define d										0x85	

// display 1 segmentation
sbit segA1 = P1 ^ 0;
sbit segB1 = P1 ^ 1;
sbit segC1 = P1 ^ 2;
sbit segD1 = P1 ^ 3;
sbit segE1 = P1 ^ 4;
sbit segF1 = P1 ^ 5;
sbit segG1 = P1 ^ 6;
sbit segDP1 = P1 ^ 7;

// display 2 segmentation
sbit segA2 = P2 ^ 0;
sbit segB2 = P2 ^ 1;
sbit segC2 = P2 ^ 2;
sbit segD2 = P2 ^ 3;
sbit segE2 = P2 ^ 4;
sbit segF2 = P2 ^ 5;
sbit segG2 = P2 ^ 6;
sbit segDP2 = P2 ^ 7;

// B1 definition
sbit B1 = P3^2;
sbit BA = P3^4;
sbit BB = P3^5;
sbit BC = P3^6;
sbit BD = P3^7;

bit pressM = 0;
bit pressA = 0;
bit pressB = 0;
bit pressC = 0;
bit pressD = 0;
bit waitingState = 1;
bit changeDisplay = 0;

int counter = 0; 		
int counter1 = 0; 
int i = 0;				 


//Function declarations
void Init(void);
void display(unsigned char digitMSB, unsigned char digitLSB);


void main (void)
{	
	Init(); 

    while(1) {
			
			// button has been pressed to start counting
			if (~waitingState) {
				
				unsigned char D1;
				unsigned char D2;
				
				if (changeDisplay) {
					
					if (counter >= (4*second)) {
						
						D1 = zeroDot;
						
					} else if (counter >= (3*second)) {
						
						D1 = oneDot;
						
					} else if (counter >= (2*second)) {
						
						D1 = twoDot;
						
					} else if (counter >= second) {
						
						D1 = threeDot;
						
					} else if (counter >= 0) {
						
						D1 = fourDot;
						
					}
					
					if (counter1 >= (9*tenthSecond)) {
						
						D2 = zero;
						
					} else if (counter1 >= (8*tenthSecond)) {
						
						D2 = one;
						
					} else if (counter1 >= (7*tenthSecond)) {
						
						D2 = two;
						
					} else if (counter1 >= (6*tenthSecond)) {
						
						D2 = three;
						
					} else if (counter1 >= (5*tenthSecond)) {
						
						D2 = four;
						
					} else if (counter1 >= (4*tenthSecond)) {
						
						D2 = five;
						
					} else if (counter1 >= (3*tenthSecond)) {
						
						D2 = six;
						
					} else if (counter1 >= (2*tenthSecond)) {
						
						D2 = seven;
						
					} else if (counter1 >= tenthSecond) {
						
						D2 = eight;
						
					} else if (counter1 >= 0) {
						
						D2 = nine;
						
					}

					display(D1, D2);
					
				}
				
			}
						
		}	
}
void Init(void){
	//Configuration of interruptions
	EA = 1; //ativate global interruptions
	ET0 = 1; // activate timer interruption 0
	EX0 = 1; // activate external interruption 0
	
	//Configure TMOD
	TMOD &= 0xF0; //Clean initial 4 bits of timer 0 and 1
	TMOD |= 0x01; //Set timer 0 with 16 bits
	
	//Configure timer 0
	//Timer 0 - 50ms -> (65536(10000h) - 50000(3E8h) = 15536(3CB0h))
	TH0 = 0x3C;	
	TL0 = 0xB0;


	//Configure TCON
	IT0 = 1; //specify falling edge trigger on external interruption 0
	
	display(fiveDot, zero);
}

void Timer0_ISR (void) interrupt 1 {	
	
	//Configure timer 0
	//Timer 0 - 50ms -> (65536(10000h) - 50000(C350h)= 15536(3CB0h))
	TH0 = 0x3C;	
	TL0 = 0xB0;

	counter++;
	counter1++;
	
	changeDisplay = 1;
	
	if(counter >= (5*second)){			// check if gets past 5s
		counter = 0;									
	}
	
	if(counter >= second) {  // check if pass 1s
		counter1 = 0;
	}
		
}


void External0_ISR (void) interrupt 0 {
	

	if (waitingState) {
		waitingState = ~waitingState; // start counting time
		TR0 = 1;
	}
	
	
}

void display(unsigned char digitMSB, unsigned char digitLSB)
{

	changeDisplay = 0;
	
	P1 = digitMSB;
	P2 = digitLSB;

}