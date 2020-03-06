/***************************************************************************************/
/******************************           LCD_DRIVER         *********************************/
/***************************************************************************************/
#include <stdint.h>
#include "myInc/BIT_MATH.h"
#include "myInc/GPIO_int.h"
#include "myInc/LCD_int.h"
#include "myInc/delay.h"

void LCD_Init_8BIT(uint8_t Disp_mode)
{
    
    GPIO_Init(Data_Port, ALL_PORT, DIGITAL_MODE);
    GPIO_SetPortDirection(Data_Port, 0xff);
    GPIO_Init(Ctrl_Port, ALL_PORT, DIGITAL_MODE);
    GPIO_SetPinDirection(Ctrl_Port, PIN1, OUTPUT);
    GPIO_SetPinDirection(Ctrl_Port, PIN2, OUTPUT);
    GPIO_SetPinDirection(Ctrl_Port, PIN3, OUTPUT);
    _delay_ms(30);
    LCD_SendCommand(I8BIT_2LINE);
    _delay_ms(1);
    LCD_SendCommand(Disp_mode);
    _delay_ms(1);
    LCD_SendCommand(CLR_DISPLAY);
    _delay_ms(2);
    LCD_SendCommand(ENTRY_MODE);
}

void LCD_SendData(uint8_t Data)
{
    GPIO_SetPinValue(Ctrl_Port, RS, HIGH);
    GPIO_SetPinValue(Ctrl_Port, RW, LOW);
    GPIO_SetPinValue(Ctrl_Port, E, HIGH);
    GPIO_SetPinValue(Data_Port, ALL_PORT, Data);
    GPIO_SetPinValue(Ctrl_Port, E, LOW);
    _delay_ms(5);
    GPIO_SetPinValue(Ctrl_Port, E, HIGH);
}

void LCD_SendCommand(uint8_t Inst)
{
    GPIO_SetPinValue(Ctrl_Port, RS, LOW);
    GPIO_SetPinValue(Ctrl_Port, RW, LOW);
    GPIO_SetPinValue(Ctrl_Port, E, HIGH);
    GPIO_SetPinValue(Data_Port, ALL_PORT,Inst);
    GPIO_SetPinValue(Ctrl_Port, E, LOW);
    _delay_ms(5);
    GPIO_SetPinValue(Ctrl_Port, E, HIGH);
}

void LCD_GoToXY(uint8_t Row, uint8_t Col)
{
    uint8_t inst = 0x80 | Col | Row;
    LCD_SendCommand(inst);
}

void LCD_SendStr(uint8_t* ptrStr)
{// edit with gotoxy so that it can write a long string on the 2 lines
    uint8_t i;
    for(i=0; ptrStr[i] != '\0'; i++)
    {
        LCD_SendData(ptrStr[i]);
    }
}

void LCD_WriteNum(uint16_t num)
{
    if(num >= 10000)
    {
    	LCD_SendData((num/10000)+'0');
    	num -= (num/10000)*10000;
    	LCD_SendData((num/1000)+'0');
    	num -= (num/1000)*1000;
    	LCD_SendData((num/100)+'0');
    	num -= (num/100)*100;
    	LCD_SendData((num/10)+'0');
    	num -= (num/10)*10;
    	LCD_SendData((num)+'0');
    	return ;
    }
    if(num >= 1000)
	{
		LCD_SendData((num/1000)+'0');
		num -= (num/1000)*1000;
		LCD_SendData((num/100)+'0');
		num -= (num/100)*100;
		LCD_SendData((num/10)+'0');
		num -= (num/10)*10;
		LCD_SendData((num)+'0');
		return ;
	}
    if(num >= 100)
	{
		LCD_SendData((num/100)+'0');
		num -= (num/100)*100;
		LCD_SendData((num/10)+'0');
		num -= (num/10)*10;
		LCD_SendData((num)+'0');
		return ;
	}
    if(num >= 10)
	{
		LCD_SendData((num/10)+'0');
		num -= (num/10)*10;
		LCD_SendData((num)+'0');
		return ;
	}
    LCD_SendData((num)+'0');
    return ;

}

















