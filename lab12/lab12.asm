.ORIG x3000
	; clear all registers
	AND R1, R1, #0;
	AND R2, R2, #0;
	AND R3, R3, #0;
	AND R4, R4, #0;
	AND R5, R5, #0;
	AND R6, R6, #0;
	AND R7, R7, #0;

	GETC
	ADD R1, R0, #0
	ST R1, CHAR0

	GETC
	ADD R2, R0, #0
	ST R2, CHAR1

	GETC
	ADD R3, R0, R0
	ADD R3, R3, R3
	ADD R3, R3, R3
	ADD R3, R3, R3
	LD R2, FOUR	; gets starting address
	ADD R3, R3, R2
	; R3 is starting address for character

	; loop through all 16 lines
	; loop through each row, each 0 = R1, each 1 = R2
	; out the character
	
	; R3 CANNOT USE
	; R6 is Inner Loop Counter
	; R5 is Current Word
	; R4 is Current Bit
	; R0 is Char Output
	; R2 is placecholder
	; R1 is Line count
	; Loop through 1 line:
	
	LD R4, FIRSTBIT ; x8000
	LD R2, ZERO
	LD R6, ZERO
	LD R1, SIXTEEN	; 17 run 16 times
	BR PARSELINE

PARSELINE	LDR R5, R3, #0		; Get word
		; need to go 16 lines
		BR PROCESSBIT

; if current bit is 0, then print char0, else print char1
PROCESSBIT	
		; i'm updating the actual line, i need to save that line
		
		AND R2, R5, R4	; find 0 or 1
		BRz OUTZ	; if 0 print char0
		; if 1
		LD R0, CHAR1	; print 1
		OUT
		BR RIGHTSHIFT


OUTZ		LD R0, CHAR0	; print 0
		OUT
		BR RIGHTSHIFT

RIGHTSHIFT	LD R2, ZERO	
		BR LOOP

LOOP:		ADD R4, R4, #-2
		BRn NEXTBIT
		ADD R2, R2, #1
		BR LOOP

NEXTBIT:	
		ADD R4, R2, #0	; sets R4 to R2
		ADD R6, R6, #1	; ++ iter count
		ADD R2, R6, #-8 ; if zero, then ive gone through all 8 bits
		BRz NEWLINE	;
		BR PROCESSBIT

NEWLINE:	
		; if ran 16 times then done
		ADD R1, R1, #-1	; decrements R1
		BRz STOP	;	
		ADD R3, R3, #1	; inc mem addr
		AND R6, R6, #0	; resets R6
		LD R4, FIRSTBIT	; resets R4 for iterator
		LD R0, LINE	; print new line
		OUT
		BR PARSELINE

STOP:
		HALT

FOUR	.FILL x4000
CHAR0	.FILL x2000
CHAR1	.FILL x2001
LINE 	.FILL x000A
ZERO	.FILL x0000
FIRSTBIT	.FILL x8000
SIXTEEN	.FILL x0010

.END
