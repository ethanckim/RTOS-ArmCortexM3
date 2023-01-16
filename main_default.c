/*
 * Default main.c for rtos lab.
 * @author Andrew Morton, 2018
 */
#include <LPC17xx.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define THREAD_STACK_SIZE 0x200 //512 byte thread stack. This is a lot.
#define MAX_NUM_THREAD 16

/*	All tasks have their own...
		1. Task Control Block (TCB)
		2. Stack
*/
typedef struct {
	uint32_t* stack_ptr;
	uint32_t* sp_offset;
	// TODO Add more functionality if necessary, such as priority
} osTCB_t;

osTCB_t osTCB_list[MAX_NUM_THREAD];
uint8_t num_thread = 0, running_thread_index = 0;
osTCB_t* curr_thread, next_thread;
int first_contxt_switch = 1;

void osCreateThread(void (*fp)()) {
	// Find and set stack pointers
	uint32_t* MSR_Original = (uint32_t*)0; // Main Stack Pointer (MSP)
	uint32_t msrAddr = *MSR_Original;
	uint32_t* sp = (uint32_t*)(msrAddr - (num_thread+1)*THREAD_STACK_SIZE); // Process Stack Pointer (PSP)

	// 1. create new tcb
	osTCB_t newThread;
	newThread.stack_ptr = sp;
	
	//2. bootload stack
	*sp = 1<<24; // store xPSR (Program Status Registers)
	sp--;
	*sp = (uint32_t)fp; // store R15: PC (Program Counter)
	for(uint32_t i=31; i >= 26; i--) { 	// store 0x1F - 0x1A into R14 (LR), R12 (IP), R3, R2, R1, R0
		sp--;
		*sp = i;
	}
	for(uint32_t i=11; i >= 4; i--) { 	// store R11 - R4: Variable Registers
		sp--;
		*sp = i;
	}

	// 3. save offsetted stack pointer
	newThread.sp_offset = sp;
	osTCB_list[num_thread] = newThread;
	num_thread++;
	
	return;
}

static void trigger_pendsv(void)
{
	//printf("Triggering PendSV. Hopefully we'll see the other string\n");
	
	volatile uint32_t *icsr = (void*)0xE000ED04; //Interrupt Control/Status Vector
	*icsr = 0x1<<28; //tell the chip to do the pendsv by writing 1 to the PDNSVSET bit

	//flush things
	__ASM("isb"); //This just prevents other things in the pipeline from running before we return
	
	return;
	
}

extern void switch_pointers(void) {
	if (first_contxt_switch) {
		first_contxt_switch = 0;
	} else {
		curr_thread->sp_offset = (uint32_t*)__get_PSP();
	}
	__set_PSP((uint32_t)(next_thread->sp_offset));
	return;
}

void osYield() {
	if(num_thread <=1) return; // 1 task, no need to switch
	
	curr_thread = &osTCB_list[running_thread_index];
	running_thread_index = (running_thread_index+1)%num_thread;
	next_thread = &osTCB_list[running_thread_index];
		
	// Trigger PendSV Interrupt
	trigger_pendsv();
	
	return;
}

void osStart(){
	curr_thread = &osTCB_list[num_thread-1];
	next_thread = &osTCB_list[0];
	
	__set_CONTROL(1<<1);
	__set_PSP((uint32_t)(curr_thread->sp_offset - 100)); // do not store garbage in first context switch
	
	// Trigger PendSV Interrupt
	trigger_pendsv();

	// should jump to the first task after calling the Interrupt
}

void t1(void *args) {
	while(1) {
		printf("Thread 1\n");
		osYield();
	}
}

void t2(void *args) {
	while(1) {
		printf("Thread 2\n");
		osYield();
	}
}

int main(void) {
	SystemInit();
	
	printf("Starting RTOS\n"); // init printf
	
	/* - set the PendSV interrupt priority to the lowest level 0xFF, which makes it very negative.
	   - In the ARM Cortex, negative priorities are high.
		 - The constant 0xE000ED20 is the location of the priority table for PendSV
	*/
	*(uint32_t volatile *)0xE000ED20 |= (0xFFU << 16);

//	// Exercise 1: trigger PendSV and successfully return
//	uint32_t* MSR_Original = (uint32_t*)0;
//	uint32_t msrAddr = *MSR_Original;
//	uint32_t* threadPSP = (uint32_t*)(msrAddr - THREAD_STACK_SIZE);

//	__set_CONTROL(1<<1);
//	__set_PSP((uint32_t)threadPSP);
//	trigger_pendsv();
//	printf("If you see this you successfully got out of PendSV\n");
	
	osCreateThread(t1);
	osCreateThread(t2);
	
	osStart();
	
	while(1){}
}
