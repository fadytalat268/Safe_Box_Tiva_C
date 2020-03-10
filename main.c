/* T1
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

//Buttons : A yes B no C open D close

//Includes:__________________________________________________________________________________________________________
#include "stdint.h"
#include "C:/Users/BoshBosh/Desktop/Tiva/myInc/tm4c123gh6pm.h"
#include "C:/Users/BoshBosh/Desktop/Tiva/myInc/BIT_MATH.h"
#include "C:/Users/BoshBosh/Desktop/Tiva/myInc/delay.h"
#include "C:/Users/BoshBosh/Desktop/Tiva/myInc/GPIO_int.h"
#include "C:/Users/BoshBosh/Desktop/Tiva/myInc/LCD_int.h"
#include "C:/Users/BoshBosh/Desktop/Tiva/myInc/KEYPAD_int.h"
void SystemInit () {}
//Functions Prototypes:______________________________________________________________________________________________
void Timer_Init (void);
void Timer (void); 
void Reset_PW (void);
void Lock_Control(uint8_t Order);
void Receive_PW (uint8_t * PW);
uint8_t Check_PW (uint8_t * Entered_PW);
void Reset_Button (void);
void LCD_Send_Msg(uint8_t * Msg); 
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
//Global Variables:__________________________________________________________________________________________________
volatile uint8_t Current_PW [4] = "0000", counter=0;
//Pins Configuration:________________________________________________________________________________________________

//___________________________________________________________________________________________________________________

int main (){
	PORTS_Init();
	Open_Lock();
	while(1){
	if(Lock_Status()==CLOSE) LCD_Send_Msg("Press C to Open");
	else if(Lock_Status()==OPEN) LCD_Send_Msg("Press D to Close");
	if(Is_Reset()) Timer();
	else if(Keypad_PressedKey()=='C') Lock_Control('C');
	else if(Keypad_PressedKey()=='D') Lock_Control('D');
	}
}
//Functions Definitions:_____________________________________________________________________________________________
void PORTS_Init(void){
GPIO_SetPinDirection(Lock_Port, Lock_Pin, OUTPUT);
GPIO_SetPinDirection(Reset_Port, Reset_Pin, INPUT);
GPIO_SetPinDirection(Buzzer_Port, Buzzer_Pin, OUTPUT);
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
	 //Open_Buzzer();
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
		   _delay_ms(1000);
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
    uint8_t i=0,Entered_PW[4], New_PW[4];
	  LCD_Send_Msg("Enter old PW");
	  Receive_PW (Entered_PW);
    if (Check_PW(Entered_PW) && Keypad_PressedKey()=='A'){
			LCD_Send_Msg("Enter new PW");
			counter=0;
			do{
			Receive_PW (New_PW);
		  if((!Check_If_0000(New_PW)) && Keypad_PressedKey()=='A'){
				LCD_Send_Msg("Try Again");
				_delay_ms(3000);
			}
			}while(!Check_If_0000(New_PW));
      if(Keypad_PressedKey()=='A'){
			for (i=0 ;i<4 ; i++) Current_PW[i]=New_PW[i];
			LCD_Send_Msg("Password Changed");
			}
    }
		else {
        LCD_Send_Msg("Wrong Password");
			  counter++;
			  if(counter == 5) Wrong_PW();
			  _delay_ms(3000);
			  Reset_PW();
    }
}
//___________________________________________________________________________________________________________________
void Lock_Control(uint8_t Order){
	uint8_t Entered_PW[4];
	if(Order == 'C' && Lock_Status() == OPEN){
		LCD_Send_Msg("Already Opened");
		_delay_ms(2000);
		return;
	}
	else if(Order == 'D' && Lock_Status == CLOSE){
		LCD_Send_Msg("Already Closed");
		return;
	}
	else{
	LCD_Send_Msg("Enter Password");
  Receive_PW(Entered_PW);
	if(Check_PW(Entered_PW) && Keypad_PressedKey()=='A'){
		counter=0;
		if(Order=='C'){			
		Open_Lock();
		LCD_Send_Msg("Unlocked");
		return;
		//while (Keypad_PressedKey()!=0xFF) Open_Buzzer();
	  }
		else if(Order=='D'){
		Close_Lock();
		LCD_Send_Msg("Locked");
		return;
		//while (Keypad_PressedKey()!=0xFF) Open_Buzzer();
		}
}	
	else{
		counter++;
		if(counter==5) Wrong_PW();
	LCD_Send_Msg("Wrong PW");
	_delay_ms(3000);
	}
}
}
//___________________________________________________________________________________________________________________
void Receive_PW (uint8_t * PW){
	    uint8_t i=0;
   	  uint8_t K_Old_Value = Keypad_PressedKey();
			while (i<4){
				if ((K_Old_Value!=Keypad_PressedKey()) && (Keypad_PressedKey()!=0xFF)){
						K_Old_Value=Keypad_PressedKey();
						PW[i]=Keypad_PressedKey();
					  //LCD_Send_Msg(Keypad_PressedKey());.................................................................HERE
						i++;
		    }
				//while (Keypad_PressedKey()!=0xFF) Open_Buzzer();
			}
}
//___________________________________________________________________________________________________________________
uint8_t Check_PW (uint8_t * Entered_PW){
    uint8_t i;
    for (i=0; i<4; i++){
            if (Entered_PW[i]!=Current_PW[i]){
                return 0;
						}
    }
    return 1;
}
//___________________________________________________________________________________________________________________
void Reset_Button (void){
	if(Keypad_PressedKey() == 'A'){
	 Reset_PW();
	return;
	}
	else if(Keypad_PressedKey() == 'B'){
	//go to previous state
		return;
	}
	else {
	LCD_Send_Msg("Press (A) or (B)");
	_delay_ms(3000);
	Reset_Button();
	}
}
//___________________________________________________________________________________________________________________
void LCD_Send_Msg(uint8_t* pchar)
{
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
 * kol makan testa5dem feh keypad_pressedkey() 7otaha l awel f variable ba3dan esta5demo fl if condition
 * l functions ely betestana feha input mn l user 3ala l keypad lazem lazem yekon feha while(1) le7ad ma l user yeda5al fa te3mel break
 * if ((K_Old_Value!=Keypad_PressedKey()) && (Keypad_PressedKey()!=0xFF))?? ehh lazmet dawel check dah ??
 * new passwords and old ones should be saved in eeprom using EEPROMProgram () to write and EEPROMRead to read after initializing eeprom 
 * lleeh l freq= 1000000 gebt l rakm dah menan 3ala asas eh ya3ny ??
 * leeeeeh bete3ml reset lama yeda5al l pw 5 marat 3'alaat ??????? just yeb3at sms 3ala l mobile 
 * 
 */

/*
 *return back to previous state ezay without saving it to dipslay a msg aw ay 7aga !
 *counter to break out of reset_button funct lw fdl msh byd5l 7aga s7 !
 *no need to keep if in received pass
 *f lock : if C , else if D else y2olo ydos 3la 7aga
 * check brdo lw das C hya mfto7a aw D w hya m2fola
 *else if pass s7 w mdassh A
 *counter incrementation in case of wrong password fein!!
 *
 */
 
