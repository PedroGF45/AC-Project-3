#include <reg51.h>

#define tenthSecond					2 //time to count 0,1 seconds (2 x 50ms)
#define null							 	0	//Position in matrice null
#define zeroDot							1	//Position of 0. in matrice
#define oneDot							2	//Position of 1. in matrice
#define twoDot							3	//Position of 2. in matrice
#define threeDot						4	//Position of 3. in matrice
#define fourDot							5	//Position of 4. in matrice
#define fiveDot							6	//Position of 5. in matrice
#define sixDot							7	//Position of 6. in matrice
#define sevenDot						8	//Position of 7. in matrice
#define eightDot						9	//Position of 8. in matrice
#define nineDot							10	//Position of 9. in matrice
#define hifenDot						11	//Position of -. in matrice
#define zero								12	//Position of 0 in matrice
#define one									13	//Position of 1 in matrice
#define two									14	//Position of 2 in matrice
#define three								15	//Position of 3 in matrice
#define four								16	//Position of 4 in matrice
#define five								17	//Position of 5 in matrice
#define six									18	//Position of 6 in matrice
#define seven								19	//Position of 7 in matrice
#define eight								20	//Position of 8 in matrice
#define nine								21  //Position of 9 in matrice
#define hifen								22	//Position of - in matrice
#define a										23	//Position of A in matrice
#define b										24	//Position of b in matrice
#define c										25  //Position of C in matrice
#define d										26	//Position of d in matrice

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

unsigned char counter = 0; // variavel que incrementa cada vez que timer conta 50ms
int i = 0;				 // variavel para assinalar o estado da informa??o nos displays


//Function declarations
void Init(void);
void display(unsigned char digitMSB, unsigned char digitLSB);


void main (void)
{	
	Init(); 

    while(1)                // infinite loop
    {
			//----------------------------------atualiza informa??o nos displays
			display(i, i);

			}	
}
void Init(void)
{
	//Configuration of interruptions
	EA = 1; //ativate global interruptions
	ET0 = 1; // activate timer interruption 0
	EX0 = 1; // activate external interruption 0
	
	//Configure TMOD
	TMOD &= 0xF0; //Clean initial 4 bits of timer 0
	TMOD |= 0x01; //Set timer 0 with 16 bits
	
	//Configure timer 0
	//Timer 0 - 5ms -> (65536(10000h) - 50000(C350h)= 15536(3CB0h))
	TH0 = 0x3C;	
	TL0 = 0xB0;
	
	//Configure TCON
	TR0 = 1; //enable timer 0
	IT0 = 0; //specify falling edge trigger on external interruption 0
}

//interrupcao tempo 0 para contar tempo de 5ms de debounce
void Timer0_ISR (void) interrupt 1
{	
	//Timer 0 - 50ms -> (65536(10000h) - 50000(C350h)= 15536(3CB0h))
	//Because its not auto reloaded, need to specify values again
	TH0 = 0x3C;	
	TL0 = 0xB0;
	
	counter++;											//Increase Counter
	
	if(counter > tenthSecond){					// check if gets past 0.1s
		if (i <= d) {
			i++;
		}	else {
			i = 0;
		}
		counter = 0;									//reset ? vari?vel
	}
}

void display(unsigned char digitMSB, unsigned char digitLSB)
{
	code unsigned segments[27][8] = {
		{1, 1, 1, 1, 1, 1, 1, 1}, // null
		{0, 0, 0, 0, 0, 0, 1, 0}, // 0.
		{1, 0, 0, 1, 1, 1, 1, 0}, // 1.
		{0, 0, 1, 0, 0, 1, 0, 0}, // 2.
		{0, 0, 0, 0, 1, 1, 0, 0}, // 3.
		{1, 0, 0, 1, 1, 0, 0, 0}, // 4.
		{0, 1, 0, 0, 1, 0, 0, 0}, // 5.
		{0, 1, 0, 0, 0, 0, 0, 0}, // 6.
		{0, 0, 0, 1, 1, 1, 1, 0}, // 7.
		{0, 0, 0, 0, 0, 0, 0, 0}, // 8.
		{0, 0, 0, 0, 1, 0, 0, 0}, // 9.
		{1, 1, 1, 1, 1, 1, 0, 0}, // -.
		{0, 0, 0, 0, 0, 0, 1, 1}, // 0
		{1, 0, 0, 1, 1, 1, 1, 1}, // 1
		{0, 0, 1, 0, 0, 1, 0, 1}, // 2
		{0, 0, 0, 0, 1, 1, 0, 1}, // 3
		{1, 0, 0, 1, 1, 0, 0, 1}, // 4
		{0, 1, 0, 0, 1, 0, 0, 1}, // 5
		{0, 1, 0, 0, 0, 0, 0, 1}, // 6
		{0, 0, 0, 1, 1, 1, 1, 1}, // 7
		{0, 0, 0, 0, 0, 0, 0, 1}, // 8
		{0, 0, 0, 0, 1, 0, 0, 1}, // 9
		{1, 1, 1, 1, 1, 1, 0, 1}, // -
		{0, 0, 0, 1, 0, 0, 0, 1}, // A
		{1, 1, 0, 0, 0, 0, 0, 1}, // b
		{0, 1, 1, 0, 0, 0, 1, 1}, // C
		{1, 0, 0, 0, 0, 1, 0, 1}, // d
	};

	segA1 = segments[digitMSB][0];
	segB1 = segments[digitMSB][1];
	segC1 = segments[digitMSB][2];
	segD1 = segments[digitMSB][3];
	segE1 = segments[digitMSB][4];
	segF1 = segments[digitMSB][5];
	segG1 = segments[digitMSB][6];
	segDP1 = segments[digitMSB][7];	
	
	segA2 = segments[digitLSB][0];
	segB2 = segments[digitLSB][1];
	segC2 = segments[digitLSB][2];
	segD2 = segments[digitLSB][3];
	segE2 = segments[digitLSB][4];
	segF2 = segments[digitLSB][5];
	segG2 = segments[digitLSB][6];
	segDP2 = segments[digitLSB][7];
	
}