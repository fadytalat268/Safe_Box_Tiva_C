#include <stdint.h>
#include <stdbool.h>
#include "myInc/BIT_MATH.h"
#include "driverlib/sysctl.h"
#include "myInc/tm4c123gh6pm.h"
#include "myInc/GPIO_int.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
# include "driverlib/uart.h"
#include "UART_INT.H"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

void UART2_init(void)
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

    GPIO_Init(PORT_D, PIN7, ALTERNATIVE_MODE); // does nothing just to unlock pin7
    GPIOPinConfigure(GPIO_PD6_U2RX);
    GPIOPinConfigure(GPIO_PD7_U2TX);

    GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_INT_PIN_6|GPIO_INT_PIN_7);
    UARTConfigSetExpClk(UART2_BASE, SysCtlClockGet(), 9600, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE));
}
void UART_sendchar (uint8_t data)
{
	UARTCharPut(UART2_BASE, 'E');
   
}