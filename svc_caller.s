	EXTERN switch_pointers
	AREA	handle_pend,CODE,READONLY
	GLOBAL PendSV_Handler
	PRESERVE8
PendSV_Handler

		MRS r0, PSP ;loads PSP address into r0
		STMDB r0!, {r4-r11}
		MSR PSP, r0 ; After decrementing r0, save r0 into PSP

		BL switch_pointers
				
		;MRS r0, PSP ;MRS loads PSP’s address into r0
		LDMIA r0!, {r4-r11}
		
		MSR PSP, r0
				
		;write the magic value of 0xFFFFFFFD to LR (R14)
		MOV LR, #0xFFFFFFFD
		;return - Note that, since LR has the magic value, the microcontroller reloads it from the 
		;stack frame it pushed on at the start of this call. Finding that value and aligning your stack
		;will 100% cause you grief while you try to do a context switch, but how to do that depends
		;so much on your design that we can't tell you how!

		BX LR
		
		END