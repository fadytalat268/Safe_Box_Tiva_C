/*
Functions:
1. Reset_PW
2. Check_PW 
3. Wrong_PW 
4. Reset_Button 
5. Timer 
6. LCD_Send_Msg
7. Lock_Ctrl
8. Input_PW
9. Timer_Init
*/
//==========================================
//Functions Prototypes:
uint8_t Check_PW(uint8_t *Entered_PW)
{
    int i;
    for (i=0; i<4, i++){
            if (Entered_PW[i]==Current_PW[i]){
                flag=1;
            }else {
                flag=0;
                break;
            }
    }
    return flag;
}
void Reset_PW (uint8_t *Entered_PW , uint8_t *New_PW)
{
    int i;
    LCD_Send_Msg("Enter curr PW");
    if (Check_PW(Entered_PW)){
        for (i=0 ;i<4 ; i++){
            Current_PW[i]=New_PW[i];
        }

    }else {
        LCD_Send_Msg("Wrong PW");

    }
    
}
void Wrong_PW (void);
void Reset_Button (void);
void Timer (void); 
void LCD_Send_Msg(uint8_t * Msg); 
void Lock_Control(uint8_t);
void Timer_Init (void);
void Input_PW (uint8_t * Entered_PW);

#define Lock_Pin PIN2
#define Lock_Port PORT_D

#include "stdint.h"
#include "myInc/tm4c123gh6pm.h"
//Many inclusions .h missing
#define freq 1000000 // can be changed
void SystemInit () {}

uint8_t Current_Password [4] = "0000";
uint8_t Entered_Password [4];
int main (){

}

void Timer_Init (void){
	NVIC_ST_CTRL_R = 0;  
	NVIC_ST_RELOAD_R  = freq - 1; // 1 second count
	NVIC_ST_CURRENT_R = 0;
	NVIC_ST_CTRL_R = 5;
}

void Input_PW(uint8_t * Entered_PW){
	
}
void Reset_PW (uint8_t * Current_PW, uint8_t * Entered_PW){

}
uint8_t Check_PW (uint8_t * Entered_PW){

}
void Wrong_PW (void){

}
void Reset_Button (void){

}
void Timer (void){

}
void LCD_Send_Msg(uint8_t * Msg){

}
void Lock_Control(uint8_t){

}
