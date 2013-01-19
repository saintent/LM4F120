
#include "hw_memmap.h"
#include "hw_types.h"
#include "debug.h"
#include "fpu.h"
#include "gpio.h"
#include "pin_map.h"
#include "rom.h"
#include "sysctl.h"
#include "uartstdio.h"

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, unsigned long ulLine)
{
}
#endif

int main(void) {
	volatile unsigned long ulLoop;

	//
	// Enable lazy stacking for interrupt handlers.  This allows floating-point
	// instructions to be used within interrupt handlers, but at the expense of
	// extra stack usage.
	//
	ROM_FPULazyStackingEnable();

	//
	// Set the clocking to run directly from the crystal.
	//
	ROM_SysCtlClockSet(
			SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ
					| SYSCTL_OSC_MAIN);

	//
	// Enable the GPIO port that is used for the on-board LED.
	//
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	//
	// Enable the GPIO pins for the LED (PF2 & PF3).
	//
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

	//
	// Initialize the UART.
	//
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	ROM_GPIOPinConfigure (GPIO_PA0_U0RX);
	ROM_GPIOPinConfigure (GPIO_PA1_U0TX);
	ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	UARTStdioInit(0);

	//
	// Hello!
	//
	UARTprintf("Hello, world!\n");

	//
	// We are finished.  Hang around doing nothing.
	//
	while (1) {
		//
		// Turn on the BLUE LED.
		//
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);

		//
		// Delay for a bit.
		//
		SysCtlDelay(SysCtlClockGet() / 10 / 3);

		//
		// Turn off the BLUE LED.
		//
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);

		//
		// Delay for a bit.
		//
		SysCtlDelay(SysCtlClockGet() / 10 / 3);
	}
}
