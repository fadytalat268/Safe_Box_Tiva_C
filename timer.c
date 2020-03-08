#include "stdint.h"
#include "C:/Keil/Labware/inc/tm4c123gh6pm.h"
//LCD inclusions missing
#define freq 1000000 // can be changed
void SystemInit () {}

void SysTick_Init (void){

	NVIC_ST_CTRL_R = 0;  
	NVIC_ST_RELOAD_R  = freq - 1; // 1 second count
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R = 5;
}

int main (){
	char counter = 0;
	char Reset_Message[100]="To reset the password press (A), to go back press (B)"; //may need modification
  SysTick_Init();
	while (1){
	 if(NVIC_ST_CTRL_R & 0x00010000){
	   NVIC_ST_CURRENT_R = 0;
		 counter ++;
	 }
	 if(counter == 5){
		   //call the LCD
		   LCD_SendStr(Reset_Message);
			 break;
	 }
	 }
}
