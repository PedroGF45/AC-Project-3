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
#define aLetter							0x11	
#define bLetter							0xC1	
#define cLetter							0x63 	
#define dLetter							0x85	


// Buttons definition
sbit BA = P3^4;
sbit BB = P3^5;
sbit BC = P3^6;
sbit BD = P3^7;

bit pressA = 0;
bit pressB = 0;
bit pressC = 0;
bit pressD = 0;
bit waitingState = 1;
bit timeState = 0;
bit answerState = 0;
bit D1changed = 0;
bit D2changed = 0;

unsigned int counter = 0; 		
unsigned int counter1 = 0; 

unsigned int D1;
unsigned int D2;


//Function declarations
void Init(void);
void display(unsigned int Display1, unsigned int Display2);

void main (void)
{	
	Init(); 

    while(1) {
			
			if (waitingState == 0) {
				
				// button has been pressed to start counting
				
				if (timeState == 1) {
					
					if (counter == (4*second) && D1changed == 0) {
						
						D1 = zeroDot;
						D1changed = 1;
						
					} else if (counter == (3*second) && D1changed == 0) {
						
						D1 = oneDot;
						D1changed = 1;
						
					} else if (counter == (2*second) && D1changed == 0) {
						
						D1 = twoDot;
						D1changed = 1;
						
					} else if (counter == second && D1changed == 0) {
						
						D1 = threeDot;
						D1changed = 1;
						
					} else if (counter == 0 && D1changed == 0 && timeState == 1) {
						
						D1 = fourDot;
						D1changed = 1;
						
					}
					
					if (counter1 == (9*tenthSecond) && D2changed == 0) {
						
						D2 = zero;
						D2changed = 1;
							
					} else if (counter1 == (8*tenthSecond) && D2changed == 0) {
						
						D2 = one;
						D2changed = 1;
						
					} else if (counter1 == (7*tenthSecond) && D2changed == 0) {
						
						D2 = two;
						D2changed = 1;
						
					} else if (counter1 == (6*tenthSecond) && D2changed == 0) {
						
						D2 = three;
						D2changed = 1;
						
					} else if (counter1 == (5*tenthSecond) && D2changed == 0) {
						
						D2 = four;
						D2changed = 1;
						
					} else if (counter1 == (4*tenthSecond) && D2changed == 0) {
						
						D2 = five;
						D2changed = 1;
						
					} else if (counter1 == (3*tenthSecond) && D2changed == 0) {
						
						D2 = six;
						D2changed = 1;
						
					} else if (counter1 == (2*tenthSecond) && D2changed == 0) {
						
						D2 = seven;
						D2changed = 1;
						
					} else if (counter1 == tenthSecond && D2changed == 0) {
						
						D2 = eight;
						D2changed = 1;
						
					} else if (counter1 == 0 && D2changed == 0 && timeState == 1) {
						
						D2 = nine;
						D2changed = 1;
						
					}

					if (D1changed == 1 || D2changed == 1) {
						
						display(D1, D2);
						
					}

				}
				
				// answer state
				if (answerState == 1) {
					
					if (counter == (2*second) && D1changed == 0) {
							
							display(D1, D2);
							
							counter = 0; // repeat the cycle
							
						
					} else if (counter == second && D1changed == 0) {
						
						if (pressA == 1) {
							
							display(hifenDot, aLetter);
							
						} else if (pressB == 1) {
							
							display(hifenDot, bLetter);
							
						} else if (pressC == 1) {
							
							display(hifenDot, cLetter);
							
						} else if (pressD == 1) {
							
							display(hifenDot, dLetter);
							
						} else {
							
							display(hifenDot, hifen);
							
						}
						
					} 
					
				}
			
			}
						
		}	
}
void Init(void){
	
	//Configuration of interruptions
	EA = 1; //ativate global interruptions
	ET0 = 1; // activate timer interruption 0
	ET1 = 1; // activate timer interruption 1
	EX0 = 1; // activate external interruption 0
	EX1 = 1, // activate external interruption 1
	
	//Configure TMOD
	TMOD &= 0x00; //Clean initial 4 bits of timer 0 and 1
	TMOD |= 0x11; //Set timer 0 and 1 with 16 bits
	
	//Configure timer 0
	//Timer 0 - 50ms -> (65536(10000h) - 50000(3E8h) = 15536(3CB0h))
	TH0 = 0x3C;	
	TL0 = 0xB0;
	//Timer 1 - 50ms -> (65536(10000h) - 50000(3E8h) = 15536(3CB0h))
	TH1 = 0x3C;	
	TL1 = 0xB0;
	

	//Configure TCON
	IT0 = 1; //specify falling edge trigger on external interruption 0
	IT1 = 1; //specify falling edge trigger on external interruption 1
	
	display(fiveDot, zero);
}

void Timer0_ISR (void) interrupt 1 {	
	
	//Configure timer 0
	//Timer 0 - 50ms -> (65536(10000h) - 50000(C350h)= 15536(3CB0h))
	TH0 = 0x3C;	
	TL0 = 0xB0;

	counter++;
	
	if (counter % second == 0) {
		
		D1changed = 0;
		
	}
	
	
	
	// start showing answer state with no answer
	if(counter >= (5*second) && waitingState == 0 && timeState == 1){	
		
		// reset timer
		counter = 0;	

		// change stats
		timeState = 0;
		answerState = 1;
		
		// bug fix
		D1 = zeroDot;
		D2 = zero;
		
	}
		
}

void Timer1_ISR (void) interrupt 3 {
	
	//Timer 1 - 50ms -> (65536(10000h) - 50000(3E8h) = 15536(3CB0h))
	TH1 = 0x3C;	
	TL1 = 0xB0;
	
	// increase counter
	counter1++;
	
	if (counter1 % tenthSecond == 0) {
		
		D2changed = 0;
	
	}
	
	
	if (counter1 >= second) {
		
		counter1 = 0;
		
	}
	

}


void External0_ISR (void) interrupt 0 {
	
	// change for non waiting state and start counting timer
	if (waitingState == 1 && answerState == 0) {
		
		waitingState = 0;
		timeState = 1;
			
		TR0 = 1;
		TR1 = 1;
		
		//Timer 0 - 50ms -> (65536(10000h) - 50000(3E8h) = 15536(3CB0h))
		TH0 = 0x3C;	
		TL0 = 0xB0;
		//Timer 1 - 50ms -> (65536(10000h) - 50000(3E8h) = 15536(3CB0h))
		TH1 = 0x3C;	
		TL1 = 0xB0;

		counter = 0;
		counter1 = 0;
		
	}
	
	// beginning state with 5.0s
	if (answerState == 1) {
		
		answerState = 0;
		waitingState = 1;
		
		pressA = 0;
		pressB = 0;
		pressC = 0;
		pressD = 0;
		
		counter = 0;
		counter1 =0;
		
		// bug fix
		D1 = fiveDot;
		D2 = zero;
		
		display(D1, D2);
		
		// stop timers
		TR0 = 0;
		TR1 = 0;
				
	}

	
}

void External1_ISR (void) interrupt 2 {
	
	// check which button has been pressed
	if (waitingState == 0 && answerState == 0) {
		
		if (~BA) {
		
			pressA = 1;
			
			// change states
			timeState = 0;
			answerState = 1;
		
		} else if (~BB) {
		
			pressB = 1;
			
			// change states
			timeState = 0;
			answerState = 1;
		
		} else if (~BC) {
		
			pressC = 1;
			
			// change states
			timeState = 0;
			answerState = 1;
	
		} else if (~BD) {
		
			pressD = 1;

			// change states
			timeState = 0;
			answerState = 1;			
		}
		
		counter = 0;
		counter1 = 0;
		
	}
	
	
}

void display(unsigned int Display1, unsigned int Display2)
{

	P1 = Display1;
	P2 = Display2;

}