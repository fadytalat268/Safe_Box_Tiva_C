/* T3
Functions (not complete):
1. Timer_Init (Done)
2. Timer (Done)
3. Reset_PW (Done)
4. Lock_Control (Done)
5. Receive_PW (Done)
6. Check_PW (Done)
7. Reset_Button (Done)
8. LCD_Send_Msg (Waiting Done)
9. Wrong_PW (//)
*/
//Includes:__________________________________________________________________________________________________________
#include "stdint.h"
#include "C:/Users/BoshBosh/Desktop/Tiva/myInc/tm4c123gh6pm.h"
#include "C:/Users/BoshBosh/Desktop/Tiva/myInc/BIT_MATH.h"
#include "C:/Users/BoshBosh/Desktop/Tiva/myInc/delay.h"
#include "C:/Users/BoshBosh/Desktop/Tiva/myInc/GPIO_int.h"
#include "C:/Users/BoshBosh/Desktop/Tiva/myInc/LCD_int.h"
#include "C:/Users/BoshBosh/Desktop/Tiva/myInc/KEYPAD_int.h"
#include "driverlib/eeprom.h"
void SystemInit () {}
//Functions Prototypes:______________________________________________________________________________________________
void Timer_Init (void);
void Timer (void); 
void eeprom_Init(void);
void Reset_PW (void);
void Lock_Control(uint8_t Order);
void Receive_PW (uint8_t * PW);
uint8_t Check_PW (uint8_t * Entered_PW);
void Reset_Button (void);
void LCD_Send_Msg(uint8_t* pchar);
void PORTS_Init(void);
void Wrong_PW (void);
void Open_Lock(void);
void Close_Lock(void);
void Open_Buzzer(void);
void Close_Buzzer(void);
uint8_t Is_Reset(void);
uint8_t Check_If_0000(uint8_t * Entered_PW);
uint8_t Keypad_PressedKey(void);
uint8_t Lock_Status(void);
void LCD_SendData(uint8_t Data);
//Macros:____________________________________________________________________________________________________________
#define Lock_Pin PIN2
#define Lock_Port PORT_D
#define Reset_Pin PIN4
#define Reset_Port PORT_F
#define Buzzer_Pin PIN3
#define Buzzer_Port PORT_D
#define freq 1000000 
#define OPEN 1
#define CLOSE 0	
#define PW_loc 0x00
//Global Variables:__________________________________________________________________________________________________
volatile uint8_t Current_PW [4] = "0000", counter=0;
//___________________________________________________________________________________________________________________

int main (){
	PORTS_Init();
	Open_Lock();
	eeprom_Init();
	uint8_t Key;
	while(1){
	Key=Keypad_PressedKey();
	if(Lock_Status()==CLOSE) LCD_Send_Msg("Press C to Open");
	else if(Lock_Status()==OPEN) LCD_Send_Msg("Press D to Close");
	if(Is_Reset()) Timer();
	else if(Key=='C') Lock_Control('C');
	else if(Key=='D') Lock_Control('D');
	}
}
//Functions Definitions:_____________________________________________________________________________________________
void PORTS_Init(void){
GPIO_SetPinDirection(Lock_Port, Lock_Pin, OUTPUT);
GPIO_SetPinDirection(Reset_Port, Reset_Pin, INPUT);
GPIO_SetPinDirection(Buzzer_Port, Buzzer_Pin, OUTPUT);
}
//___________________________________________________________________________________________________________________
void eeprom_Init(void){
	
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCT_USE_PLL|SYSTCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);
	SysCtlDelay(20000000);
	SystCtlPeriperalEnable(SYSTCTL_PERIPH_EEPROM0);
	EEPROMInit();
	EEPROMRead(Current_PW,PW_loc,sizeof(Current_PW));
	
}
//___________________________________________________________________________________________________________________
void Open_Lock(void){
   GPIO_SetPinValue(Lock_Port, Lock_Pin, OPEN);
}
//___________________________________________________________________________________________________________________
void Close_Lock(void){
   GPIO_SetPinValue(Lock_Port, Lock_Pin, CLOSE);
}
//___________________________________________________________________________________________________________________
void Open_Buzzer(void){
   GPIO_SetPinValue(Buzzer_Port, Buzzer_Pin, OPEN);
}
//___________________________________________________________________________________________________________________
void Close_Buzzer(void){
   GPIO_SetPinValue(Buzzer_Port, Buzzer_Pin, CLOSE);
}
//___________________________________________________________________________________________________________________
uint8_t Is_Reset(void){
  return (!GPIO_GetPinValue(Reset_Port, Reset_Pin));
}
//___________________________________________________________________________________________________________________
uint8_t Lock_Status(void){
  return (GPIO_GetPinValue(Lock_Port, Lock_Pin));
}
//___________________________________________________________________________________________________________________
void Timer_Init (void){
	NVIC_ST_CTRL_R = 0;  
	NVIC_ST_RELOAD_R  = freq - 1; // 1 second count
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R = 5;
}
//___________________________________________________________________________________________________________________
void Timer (void){
	char t_count = 0;
  Timer_Init();
	while (Is_Reset()){
	 Open_Buzzer();
	 if(NVIC_ST_CTRL_R & 0x00010000){
	   NVIC_ST_CURRENT_R = 0;
		 t_count ++;
	 }
	 if(t_count == 5){
		   Close_Buzzer();
		   _delay_ms(1000);
		   Open_Buzzer();
		   _delay_ms(1000);
			 Close_Buzzer();
		   LCD_Send_Msg("Reset->A,Back->B");
		   _delay_ms(3000);
		   Reset_Button();
			 break;
	 }
	 }
}
//___________________________________________________________________________________________________________________
uint8_t Check_If_0000(uint8_t * Entered_PW){
		if(Entered_PW[0]=='0' && Entered_PW[1]=='0' && Entered_PW[2]=='0' && Entered_PW[3]=='0') return 0;
  	else return 1;
}
//___________________________________________________________________________________________________________________
void Reset_PW (void){
    uint8_t i=0,Entered_PW[4], New_PW[4],Key;
	  LCD_Send_Msg("Enter old PW");
	  Receive_PW (Entered_PW);
    while (Check_PW(Entered_PW)){
			Key=Keypad_PressedKey();
			if(Key=='A'){
			LCD_Send_Msg("Enter new PW");
			counter=0;
			do{
			Receive_PW (New_PW);
			Key=Keypad_PressedKey();
		  if((!Check_If_0000(New_PW)) && Key=='A'){
				LCD_Send_Msg("PW cant be 0000");
				_delay_ms(3000);
			}
			}while(!Check_If_0000(New_PW));
			while(Check_If_0000(New_PW)){
      Key=Keypad_PressedKey();
			if(Key=='A'){
				for (i=0 ;i<4 ; i++) Current_PW[i]=New_PW[i];
				EEPROMProgram(Current_PW,PW_loc,sizeof(Current_PW));
				LCD_Send_Msg("Password Changed");
				return;
			}
			}
		}
    }
			  counter++;
			  if(counter == 5) Wrong_PW();
		    LCD_Send_Msg("Wrong Password");
			  _delay_ms(3000);
			  return;
   // }
}
//___________________________________________________________________________________________________________________
void Lock_Control(uint8_t Order){
	uint8_t Entered_PW[4], Key;
	if(Order == 'C' && Lock_Status() == OPEN){
		LCD_Send_Msg("Already Opened");
		_delay_ms(3000);
		return;
	}
	else if(Order == 'D' && Lock_Status == CLOSE){
		LCD_Send_Msg("Already Closed");
		_delay_ms(3000);
		return;
	}
	else{
	LCD_Send_Msg("Enter Password");
  Receive_PW(Entered_PW);
	while(Check_PW(Entered_PW)){
		Key=Keypad_PressedKey();
			if(Key == 'A'){
			counter=0;
			if(Order=='C'){			
			Open_Lock();
			LCD_Send_Msg("Unlocked");
			return;
			}
			else if(Order=='D'){
			Close_Lock();
			LCD_Send_Msg("Locked");
			return;
			}
			}
  }	
		counter++;
		if(counter==5) Wrong_PW();
	LCD_Send_Msg("Wrong Password");
	_delay_ms(3000);
	return;
}
}
//___________________________________________________________________________________________________________________
void Receive_PW (uint8_t * PW){
	    uint8_t i=0, Key;
			while (i<4){
				Key=Keypad_PressedKey();
				if (Key!=0xFF){
						PW[i]=Key;
					  LCD_SendData(Key);
						i++;
		    }
			}
}
//___________________________________________________________________________________________________________________
uint8_t Check_PW (uint8_t * Entered_PW){
    uint8_t i;
   	EEPROMRead(Current_PW,PW_loc,sizeof(Current_PW));
    for (i=0; i<4; i++){
            if (Entered_PW[i]!=Current_PW[i]){
                return 0;
						}
    }
    return 1;
}
//___________________________________________________________________________________________________________________
void Reset_Button (void){
	uint8_t Key;
	while(1){
		Key=Keypad_PressedKey();
	if(Key == 'A'){
	 Reset_PW();
	return;
	}
	else if(Key == 'B'){
	//go to previous state
		return;
	}
	}
}
//___________________________________________________________________________________________________________________
void LCD_Send_Msg(uint8_t* pchar){
	LCD_SendCommand(CLR_DISPLAY);
	LCD_SendCommand(RETURN_HOME);
	LCD_SendStr(pchar);
        LCD_GoToXY(Row1,Col0);
}
//___________________________________________________________________________________________________________________
void Wrong_PW (void){
	uint8_t i;
//...................................................................................................................HERE
/*
	Send Configuration Message on Mobile
*/
//Alarm 	
	for(i=0;i<5;i++){
	Open_Buzzer();
	_delay_ms(1500);
	Close_Buzzer();
  _delay_ms(300);		
	}
}
/*
Update gamed bofteeka, improved Keypad_PressedKey() function into variable Key and while loops, needs some revisions.
Missing and Wrong_PW() and Some Linking issues.
*/
