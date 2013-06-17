#include "hw_ints.h"
#include "hw_nvic.h"
#include "hw_memmap.h"
#include "hw_types.h"
#include "debug.h"
#include "fpu.h"
#include "gpio.h"
#include "pin_map.h"
#include "rom.h"
#include "timer.h"
#include "sysctl.h"
#include "uart.h"
#include "uartstdio.h"
#include "cmdline.h"
#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include "interrupt.h"

int ProcessPWM(int argc, char *argv[]);
int ProcessHelp(int argc, char *argv[]);
typedef struct PWM_Period_tag {
	uint16_t	CH1;
	uint16_t	CH2;
	uint16_t	CH3;
	uint16_t	rsv;
}PWM_Period_t;

PWM_Period_t PWM_CTR = { 0, 0, 0 };
//
// The table of commands supported by this application.
//
tCmdLineEntry g_sCmdTable[] =
{
{ "PWM", ProcessPWM, "PWM CONTROL." },
{ "pwm", ProcessPWM, "PWM CONTROL." },
{ "help", ProcessHelp, "Application help." }
};

void SetPWMMatch(void) {
    TimerMatchSet(TIMER0_BASE, TIMER_B, PWM_CTR.CH1);
    TimerMatchSet(TIMER1_BASE, TIMER_A, PWM_CTR.CH2);
    TimerMatchSet(TIMER1_BASE, TIMER_B, PWM_CTR.CH3);
}

int ProcessHelp(int argc, char *argv[])
{
	//
	// Do something, using argc and argv if the command takes arguments.
	//
	char text[1024];
	//uint16_t pos;
	/*pos =*/ sprintf(text, "\r\n\t%s : %s Color R,G,B", g_sCmdTable[0].pcCmd, g_sCmdTable[0].pcHelp);
	UARTprintf(text);
        UARTprintf("\r\nPWM Demo : ");
	return 0;
}

int ProcessPWM(int argc, char *argv[])
{
	//
	// Do something, using argc and argv if the command takes arguments.
	//
	uint32_t temp;
	uint32_t value;
	char * pEnd;
	if(argc == 4 ) {
		// Red
		value = (uint32_t) strtol(argv[1], &pEnd, 10);
		if (value <= 0) {
			PWM_CTR.CH1 = 15999;
		}
		else if ((0 < value) && (value < 100)) {
			temp = (value * (uint32_t)16000) / 100;
			PWM_CTR.CH1 = 16000 - temp;
		}
		else if (value == 100) {
			PWM_CTR.CH1 = 1;
		}
		else
			goto CMDPWM_ERROR;

		// Blue
		value = (uint32_t) strtol(argv[3], &pEnd, 10);
		if (value <= 0) {
			PWM_CTR.CH2 = 15999;
		}
		else if ((0 < value) && (value < 100)) {
			temp = (value * (uint32_t)16000) / 100;
			PWM_CTR.CH2 = 16000 - temp;
		}
		else if (value == 100) {
			PWM_CTR.CH2 = 1;
		}
		else
			goto CMDPWM_ERROR;

		// Green
		value = (uint32_t) strtol(argv[2], &pEnd, 10);
		if (value <= 0) {
			PWM_CTR.CH3 = 15999;
		}
		else if ((0 < value) && (value < 100)) {
			temp = (value * (uint32_t)16000) / 100;
			PWM_CTR.CH3 = 16000 - temp;
		}
		else if (value == 100) {
			PWM_CTR.CH3 = 1;
		}
		else goto CMDPWM_ERROR;

		SetPWMMatch();

	}
	else {
CMDPWM_ERROR :
		UARTprintf("CMD Error \r\n");
                UARTprintf("PWM Demo : ");
	}
	return 0;
}


int main()
{
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
				SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_XTAL_16MHZ
						| SYSCTL_OSC_MAIN);
		//
		// Enable the GPIO port that is used for the on-board LED.
		//
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	        ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1 | SYSCTL_PERIPH_TIMER0);
	        ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
		//
		// Enable the GPIO pins for the LED (PF2 & PF3).
		//
		//ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2 | GPIO_PIN_1 | GPIO_PIN_3);
		ROM_IntMasterEnable();
		//
		// Initialize the UART.
		//

		ROM_GPIOPinConfigure (GPIO_PA0_U0RX);
		ROM_GPIOPinConfigure (GPIO_PA1_U0TX);
		ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
		UARTStdioInit(0);
		UARTEchoSet(true);


		//
		// Enable the UART interrupt.
		//

		//
		/// Initialize PWM
		//

		GPIOPinConfigure(GPIO_PF1_T0CCP1);
		GPIOPinConfigure(GPIO_PF2_T1CCP0);
		GPIOPinConfigure(GPIO_PF3_T1CCP1);
		GPIOPinTypeTimer(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

		//
		// Configure Timer as a 16-bit periodic timer.
		//
		TimerConfigure(TIMER0_BASE, TIMER_CFG_16_BIT_PAIR |
			                   TIMER_CFG_B_PWM);
		TimerConfigure(TIMER1_BASE,
				TIMER_CFG_16_BIT_PAIR |
				TIMER_CFG_A_PWM |
				TIMER_CFG_16_BIT_PAIR |
				TIMER_CFG_B_PWM);
		// Configure period is 10 KHz
	    TimerLoadSet(TIMER0_BASE, TIMER_B, 16000);
	    TimerLoadSet(TIMER1_BASE, TIMER_A, 16000);
	    TimerLoadSet(TIMER1_BASE, TIMER_B, 16000);

	    //
	    // Set the Timer match value to load value (100% ON) .
	    //
	    TimerMatchSet(TIMER0_BASE, TIMER_B, PWM_CTR.CH1);
	    TimerMatchSet(TIMER1_BASE, TIMER_A, PWM_CTR.CH2);
	    TimerMatchSet(TIMER1_BASE, TIMER_B, PWM_CTR.CH3);

	    // Red
	    TimerEnable(TIMER0_BASE, TIMER_B);
	    // Blue
	    TimerEnable(TIMER1_BASE, TIMER_BOTH);
	    // Green
	    //TimerEnable(TIMER1_BASE, TIMER_B);

		// // Hello!
		//
		UARTprintf("LED PWM Control Demo\n");
                UARTprintf("PWM Demo : ");

		//
		// We are finished.  Hang around doing nothing.
		//
		uint8_t ch[256];
		int bIN;
		while (1) {
			bIN = UARTgets(ch, 256);
                        UARTprintf("PWM Demo : ");
			if(bIN) {
				CmdLineProcess(ch);
                                
			}
		}
}
