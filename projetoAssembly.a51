tenthSecond		EQU		2 		;time to count 0,1 seconds (2 x 50ms)
twoTenthSeconds	EQU		4
threeTenthSeconds EQU	6
fourTenthSeconds	EQU 8
fiveTenthSeconds EQU	10
sixTenthSeconds	EQU		12
sevenTenthSeconds EQU	14
eightTenthSeconds EQU	16
nineTenthSeconds EQU	18
second 			EQU		20 		;time to count 1 second (20 x 50ms)
twoSeconds 		EQU 	40		
threeSeconds	EQU		60
fourSeconds 	EQU 	80
fiveSeconds 	EQU 	100
THtimer			EQU 	0x3C	;Timer 0 - 50ms -> (65536(10000h) - 50000(3E8h) = 15536(3CB0h))
TLtimer 		EQU 	0xB0	;Timer 0 - 50ms -> (65536(10000h) - 50000(3E8h) = 15536(3CB0h))

; values for display
null			EQU		0xFF	
zeroDot			EQU		0x02	
oneDot			EQU		0x9E	
twoDot			EQU		0x24	
threeDot 		EQU		0x0C	
fourDot			EQU		0x98
fiveDot			EQU 	0x48	
hifenDot		EQU		0xFC
zero			EQU		0x03	
one				EQU		0x9F	
two				EQU		0x25	
three			EQU		0x0D	
four			EQU		0x99	
five			EQU		0x49	
six				EQU		0x41	
seven			EQU		0x1F	
eight			EQU		0x01	
nine			EQU		0x09  
hifen			EQU		0xFD	
aLetter			EQU		0x11	
bLetter			EQU		0xC1	
cLetter			EQU		0x63 	
dLetter			EQU		0x85	


; Buttons definition
BA         		EQU 	P3^4	;
BB 				EQU 	P3^5	;
BC 				EQU 	P3^6	;
BD 				EQU 	P3^7	;
	
; states
waitingState 	EQU 	1		; R0 bank 0
timeState 		EQU 	0		; R1 bank 0
answerState 	EQU 	0		; R2 bank 0
D1changed 		EQU 	0		; R3 bank 0
D2changed 		EQU 	0		; R4 bank 0

counter 		EQU 	0		; R5 bank 0
counter1 		EQU 	0		; R6 bank 0

D1 				EQU  	0		; R0 bank 1
D2 				EQU 	0		; R1 bank 1
pressA 			EQU 	0		; R2 bank 1
pressB 			EQU 	0		; R3 bank 1
pressC 			EQU 	0		; R4 bank 1
pressD 			EQU 	0		; R5 bank 1
	
StackPointer 	EQU 	0x0F	; need to point to a new location because of the usage of 2 banks
	
CSEG AT 0000H
	JMP Main

CSEG AT 0003H
	JMP ExternalInterruption0
	
CSEG AT 000BH
	JMP TimerInterruption0
	
CSEG AT 0013H
	JMP ExternalInterruption1
	
CSEG AT 001BH
	JMP TimerInterruption1
	
CSEG AT 1000H
	
Main:

	MOV SP, #StackPointer
	CALL Initializations		; routine to set registers
	CALL EnableInterruptions	; routine to enable interruptions and set priorities
	CALL ConfigureInterruptions ; routine to configure tmod, timers and tcon
	
	; select bank 1
	SETB PSW.3					
	
	MOV R0, #fiveDot
	MOV R1, #zero
	
	; select bank 0					
	CLR PSW.3
	
	CALL Display
	
	For:
	
	CJNE R0, #0, CheckAnswerState1; waitingState == 0
	
	CJNE R1, #1, CheckAnswerState1; timeState == 1
	
	CJNE R3, #0, OneSecondR; D1Changed == 0
	MOV A, R5								; A = counter
	CJNE A, #fourSeconds, OneSecondR 		; counter == 4 * seconds
		
		; select bank 1
		SETB PSW.3					
		 
		
		MOV R0, #zeroDot
		
		; select bank 0
							
		CLR PSW.3
		
		MOV R3, #1
		CALL Display
		JMP ZeroTenthSecondsR
		
	OneSecondR:
	CJNE R3, #0, TwoSecondsR; D1Changed == 0
	MOV A, R5								; A = counter
	CJNE A, #threeSeconds, TwoSecondsR 		; counter == 3 * seconds
	
		; select bank 1
		SETB PSW.3					
		 
		
		MOV R0, #oneDot
		
		; select bank 0
							
		CLR PSW.3
		
		MOV R3, #1
		CALL Display
		JMP ZeroTenthSecondsR
		
	TwoSecondsR:
	CJNE R3, #0, ThreeSecondsR; D1Changed == 0
	MOV A, R5								; A = counter
	CJNE A, #twoSeconds, ThreeSecondsR 		; counter == 2 * seconds
	
		; select bank 1
		SETB PSW.3					
		 
		
		MOV R0, #twoDot
		
		; select bank 0
							
		CLR PSW.3
		
		MOV R3, #1
		CALL Display
		JMP ZeroTenthSecondsR
	
	ThreeSecondsR:
	CJNE R3, #0, FourSecondsR; D1Changed == 0
	CJNE R5, #second, FourSecondsR ; counter == second
	
		; select bank 1
		SETB PSW.3					
		 
		
		MOV R0, #threeDot
		
		; select bank 0
							
		CLR PSW.3
		
		MOV R3, #1 ; D1changed = 1
		CALL Display
		JMP ZeroTenthSecondsR
		
	FourSecondsR:
	CJNE R3, #0, ZeroTenthSecondsR	; D1Changed == 0
	CJNE R1, #1, ZeroTenthSecondsR	; timeState == 1 
	CJNE R5, #0, ZeroTenthSecondsR	; counter == 0
	
		; select bank 1
		SETB PSW.3					
		 
		
		MOV R0, #fourDot
		
		; select bank 0
							
		CLR PSW.3
		
		MOV R3, #1
		CALL Display
		JMP ZeroTenthSecondsR
		
	CheckAnswerState1:
		JMP CheckAnswerState
		
	ZeroTenthSecondsR:
		CJNE R4, #0, OneTenthSecondsR; D2Changed == 0
		MOV A, R6
		CJNE A, #nineTenthSeconds, OneTenthSecondsR ; counter1 == 9 * tenthSeconds
		
		; select bank 1
		SETB PSW.3					
		 
		
		MOV R1, #zero
		
		; select bank 0
							
		CLR PSW.3
		
		MOV R4, #1
		CALL Display
		JMP CheckAnswerState
		
	OneTenthSecondsR:
		CJNE R4, #0, TwoTenthSecondsR; D2Changed == 0
		MOV A, R6
		CJNE A, #eightTenthSeconds, TwoTenthSecondsR ; counter1 == 8 * tenthSeconds
	
		; select bank 1
		SETB PSW.3					
		 
		
		MOV R1, #one
		
		; select bank 0
							
		CLR PSW.3
		
		MOV R4, #1
		CALL Display
		JMP CheckAnswerState
		
	TwoTenthSecondsR:
		CJNE R4, #0, ThreeTenthSecondsR; D2Changed == 0
		MOV A, R6
		CJNE A, #sevenTenthSeconds, ThreeTenthSecondsR ; counter1 == 7 * tenthSeconds
	
		; select bank 1
		SETB PSW.3					
		 
		
		MOV R1, #two
		
		; select bank 0
							
		CLR PSW.3
		
		MOV R4, #1
		CALL Display
		JMP CheckAnswerState
		
	ThreeTenthSecondsR:
		CJNE R4, #0, FourTenthSecondsR; D2Changed == 0
		MOV A, R6
		CJNE A, #sixTenthSeconds, FourTenthSecondsR ; counter1 == 6 * tenthSeconds
	
		; select bank 1
		SETB PSW.3					
		 
		
		MOV R1, #three
		
		; select bank 0
							
		CLR PSW.3
		
		MOV R4, #1
		CALL Display
		JMP CheckAnswerState
	
	FourTenthSecondsR:
		CJNE R4, #0, FiveTenthSecondsR; D2Changed == 0
		MOV A, R6
		CJNE A, #fiveTenthSeconds, FiveTenthSecondsR ; counter1 == 5 * tenthSeconds
	
		; select bank 1
		SETB PSW.3					
		 
		
		MOV R1, #four
		
		; select bank 0
							
		CLR PSW.3
		
		MOV R4, #1
		CALL Display
		JMP CheckAnswerState
		
	FiveTenthSecondsR:
		CJNE R4, #0, SixTenthSecondsR; D2Changed == 0
		MOV A, R6
		CJNE A, #fourTenthSeconds, SixTenthSecondsR ; counter1 == 4 * tenthSeconds
	
		; select bank 1
		SETB PSW.3					
		 
		
		MOV R1, #five
		
		; select bank 0
							
		CLR PSW.3
		
		MOV R4, #1
		CALL Display
		JMP CheckAnswerState
		
	SixTenthSecondsR:
		CJNE R4, #0, SevenTenthSecondsR; D2Changed == 0
		MOV A, R6
		CJNE A, #threeTenthSeconds, SevenTenthSecondsR ; counter1 == 3 * tenthSeconds
	
		; select bank 1
		SETB PSW.3					
		 
		
		MOV R1, #six
		
		; select bank 0
							
		CLR PSW.3
		
		MOV R4, #1
		CALL Display
		JMP CheckAnswerState
		
	SevenTenthSecondsR:
		CJNE R4, #0, EightTenthSecondsR; D2Changed == 0
		MOV A, R6
		CJNE A, #twoTenthSeconds, EightTenthSecondsR ; counter1 == 2 * tenthSeconds
	
		; select bank 1
		SETB PSW.3					
		 
		
		MOV R1, #seven
		
		; select bank 0
							
		CLR PSW.3
		
		MOV R4, #1
		CALL Display
		JMP CheckAnswerState
		
	EightTenthSecondsR:
		CJNE R4, #0, NineTenthSecondsR; D2Changed == 0
		CJNE R6, #tenthSecond, NineTenthSecondsR ; counter == tenthSeconds
	
		; select bank 1
		SETB PSW.3					
		 
		
		MOV R1, #eight
		
		; select bank 0
							
		CLR PSW.3
		
		MOV R4, #1
		CALL Display
		JMP CheckAnswerState
		
	NineTenthSecondsR:
	
		CJNE R4, #0, CheckAnswerState; D2Changed == 0
		CJNE R6, #0, CheckAnswerState ; counter == 0
		CJNE R1, #1, CheckAnswerState ; timeState == 1
	
		; select bank 1
		SETB PSW.3					
		 
		
		MOV R1, #nine
		
		; select bank 0
							
		CLR PSW.3
		
		MOV R4, #1  ; D2changed = 1
		CALL Display
		JMP CheckAnswerState
		
		
	CheckAnswerState:
		
		CJNE R2, #1, EndCycle1	; answerState == 1
		
		CJNE R3, #0, OneSecondAnswer; D1Changed == 0
		MOV A, R5
		CJNE A, #twoSeconds, OneSecondAnswer ; counter == 2 * seconds
	
		CALL Display
		MOV R5, #0				; reset counter
		
		JMP EndCycle
		
		EndCycle1:
			JMP EndCycle
		
		OneSecondAnswer:
		
			CJNE R3, #0, EndCycle; D1Changed == 0
			CJNE R5, #second, EndCycle ; counter == second

			; select bank 1
			SETB PSW.3
			
			CJNE R2, #1, CheckB 	; pressA == 1

			; select bank 1
			SETB PSW.3
			
			MOV A, R0
			MOV R0, #hifenDot
			MOV B, R1
			MOV R1, #aLetter
			
			CLR PSW.3
			CALL Display
			
			; select bank 1
			SETB PSW.3
			
			MOV R0, A
			MOV R1, B
			
			; select bank 0					
			CLR PSW.3
			JMP EndCycle
			
		CheckB:
			
			CJNE R3, #1, CheckC 	; pressB == 1

			; select bank 1
			SETB PSW.3
			
			MOV A, R0
			MOV R0, #hifenDot
			MOV B, R1
			MOV R1, #bLetter
			
			CLR PSW.3
			CALL Display
			
			; select bank 1
			SETB PSW.3
			
			MOV R0, A
			MOV R1, B
			
			; select bank 0					
			CLR PSW.3
			JMP EndCycle
			
		CheckC:
		
			CJNE R4, #1, CheckD 	; pressC == 1
		
			; select bank 1
			SETB PSW.3
			
			MOV A, R0
			MOV R0, #hifenDot
			MOV B, R1
			MOV R1, #cLetter
			
			CLR PSW.3
			CALL Display
			
			; select bank 1
			SETB PSW.3
			
			MOV R0, A
			MOV R1, B
			
			; select bank 0					
			CLR PSW.3
			JMP EndCycle
			
		CheckD:
		
			CJNE R5, #1, CheckNone 	; pressD == 1
			
			; select bank 1
			SETB PSW.3
			
			MOV A, R0
			MOV R0, #hifenDot
			MOV B, R1
			MOV R1, #dLetter
			
			CLR PSW.3
			CALL Display
			
			; select bank 1
			SETB PSW.3
			
			MOV R0, A
			MOV R1, B
			
			; select bank 0					
			CLR PSW.3
			JMP EndCycle
		
		CheckNone:
			
			; select bank 1
			SETB PSW.3
			
			MOV A, R0
			MOV R0, #hifenDot
			MOV B, R1
			MOV R1, #hifen
			
			CLR PSW.3
			CALL Display
			
			; select bank 1
			SETB PSW.3
			
			MOV R0, A
			MOV R1, B
			
			; select bank 0					
			CLR PSW.3
			JMP EndCycle

	EndCycle:
		CLR PSW.3
		JMP For				; begin again
			
Initializations:

	MOV R0, #waitingState
	MOV R1, #timeState
	MOV R2, #answerState
	MOV R3, #D1changed
	MOV R4, #D2changed
	MOV R5, #counter
	MOV R6, #counter1

	; select bank 1
	SETB PSW.3					
	  					
	
	MOV R0, #D1
	MOV R1, #D2
	MOV R2, #pressA
	MOV R3, #pressB
	MOV R4, #pressC
	MOV R5, #pressD
	
	; select bank 0
	CLR PSW.3					
	
	
	RET

EnableInterruptions:
	MOV IE, #8FH				; EA=1, ET1=1, EX1=1, ET0=1 e EX0=1 -> IE=10001111
	MOV IP, #00H				; Don't change priorities
	RET
	
ConfigureInterruptions:

	; TMOD Config
	MOV A, TMOD					; copy tmod to A
	ANL A,#0x00					; set all bits of tmod to 0
	ORL A,#0x11					; set timer mods to 1 (16 bits)
	MOV TMOD, A					; set tmod
	
	; timer 0 config
	MOV TH0, #THtimer			; set th0
	MOV TL0, #THtimer			; set tl0 
	
	; timer 1 config
	MOV TH1, #THtimer			; set th1
	MOV TL1, #THtimer			; set tl1

	; TCON config
	CLR TR0						; timer 0 is turned off
	CLR TR1						; timer 1 is turned off
	SETB IT0					; external interruption 0 is activated on falling edge
	SETB IT1					; external interruption 1 is activated on falling edge
	
	RET

Display:

	; select bank 1
	SETB PSW.3					
	

	MOV P1, R0
	MOV P2, R1
	
	; select bank 0
						
	CLR PSW.3
	
	RET


ExternalInterruption0:

	PUSH PSW
	
	; select bank 0				
	CLR PSW.3

	CJNE R0, #1, NotWaiting		; skip if the wainting state is 1
	
	CJNE R2, #0, ExternalInterruption0End; skip if answer state is 0
	
	; change states
	MOV R0, #0					; change to non waiting state
	MOV R1, #1					; change to start counting time state
	
	; enable timers
	SETB TR0
	SETB TR1
	
	; Reload timers
	MOV TH0, #THtimer			; set th0
	MOV TL0, #THtimer			; set tl0 
	MOV TH1, #THtimer			; set th1
	MOV TL1, #THtimer			; set tl1
	
	; reset counters
	MOV R5, #0
	MOV R6, #0
	
	JMP ExternalInterruption0End
	
	NotWaiting:
	
		CJNE R2, #1, ExternalInterruption0End ; answerState == 1
	
		; change states
		MOV R0, #1 				; change to waiting state
		MOV R2, #0				; change to non answer state
		
		; select bank 1
		SETB PSW.3					
		
		
		MOV R2, #0          ; Reset button press flags
		MOV R3, #0
		MOV R4, #0
		MOV R5, #0

		MOV R0, #fiveDot
		MOV R1, #zero
		
		; select bank 0
							
		CLR PSW.3
		
		CALL Display
		
		; reset counters
		MOV R5, #0
		MOV R6, #0
		
		// stop timers
		CLR TR0
		CLR TR1
		
	ExternalInterruption0End:
		POP PSW
		RETI
		
TimerInterruption0:

	PUSH PSW
	
	; select bank 0				
	CLR PSW.3
	
	; timer 0 config
	MOV TH0, #THtimer			; set th0
	MOV TL0, #THtimer			; set tl0 	
	
	INC R5						; Increment counter
	
	MOV R3, #0					; D1changed = 0
	
	CJNE R5, #fiveSeconds, TimerInterruption0End ; counter == 5*second
	
	CJNE R0, #0, TimerInterruption0End ; waitingState == 0
	
	CJNE R1, #1, TimerInterruption0End ; timeState == 1
	
	; reset counter
	MOV R5, #0					; counter = 0
	
	; change states
	MOV R1, #0 					; timeState = 0
	MOV R2, #1					; answerState = 1
	
	; display
	; select bank 1
	SETB PSW.3					
	
	
	MOV R0, #zeroDot
	MOV R1, #zero
	
	; select bank 0
						
	CLR PSW.3
	
	CALL Display
	
	TimerInterruption0End:
		POP PSW
		RETI
	
ExternalInterruption1:

	PUSH PSW
	
	; select bank 0				
	CLR PSW.3

	CJNE R0, #0, ExternalInterruption1End ; waitingState == 0
	
	CJNE R2, #0, ExternalInterruption1End ; answerState == 0
	
	JB BA, AnswerB			; BA == 0
	
	; select bank 1
	SETB PSW.3					
	
	
	MOV R2, #1
	
	; select bank 0
						
	CLR PSW.3
	
	; change states
	MOV R1, #0 					; timeState = 0
	MOV R2, #1					; answerState = 1
	
	JMP ExternalInterruption1End
	
	AnswerB: 
		JB BB, AnswerC			; BB == 0
		
		; select bank 1
		SETB PSW.3					
		
		
		MOV R3, #1
		
		; select bank 0
							
		CLR PSW.3
		
		; change states
		MOV R1, #0 					; timeState = 0
		MOV R2, #1					; answerState = 1
		
		JMP ExternalInterruption1End
	
	AnswerC:
		JB BC, AnswerD			; BC == 0
		
		; select bank 1
		SETB PSW.3					
		
		
		MOV R4, #1
		
		; select bank 0				
		CLR PSW.3
		
		; change states
		MOV R1, #0 					; timeState = 0
		MOV R2, #1					; answerState = 1
		
		JMP ExternalInterruption1End
	AnswerD:
		JB BD, ExternalInterruption1End; BD == 0

		; select bank 1
		SETB PSW.3					
		

		MOV R5, #1

		; select bank 0					
		CLR PSW.3

		; change states
		MOV R1, #0 					; timeState = 0
		MOV R2, #1					; answerState = 1
		
		JMP ExternalInterruption1End
		
	ExternalInterruption1End:
		POP PSW
		RETI

TimerInterruption1:

	PUSH PSW
	
	; select bank 0					
	CLR PSW.3 
	
	; timer 1 config
	MOV TH1, #THtimer			; set th1
	MOV TL1, #THtimer			; set tl1
	
	INC R6						; counter1++
		
	MOV R4, #0					; D2changed = 0
	
	CJNE R6, #second, TimerInterruption1End ; counter1 == second
	
	MOV R6, #0					; counter1 = 0
	
	TimerInterruption1End:
		POP PSW
		RETI
		
END