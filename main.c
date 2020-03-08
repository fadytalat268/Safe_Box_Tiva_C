/*
1. Reset_Password
2. Check_Password 
3. Wrong_Password 
4. Reset_Button 
5. Timer 
6. LCD_Control 
7. Lock_Control
*/
//==========================================
//Functions Prototypes:
void Reset_Password (uint8_t * Current_Password, uint8_t * New_Password);
uint8_t Check (uint8_t * Entered_Password);
void Wrong_Password (void);
void Reset_Button (void);
void Timer (void); 
LCD_Send_Msg 
Lock_Control

#define Lock_Pin PIN2
#define Lock_Port PORT_D

#include "stdint.h"
#include "myInc/tm4c123gh6pm.h"
//Many inclusions .h missing
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

// Function to receive the return of Reset Message
void Push_Button(char ret){
if (ret == 'A'){
Reset_Password();	
}
if(ret == 'B'){
// Write code here to go back to the previous state
}
// Function to reset password

void Reset_Password(void){
// Write code here to reset the password
}