//================ File Despciption =========================================//
//=== File name : source.c
//=== 
//===========================================================================//


//================ Index ====================================================//
//
//================ Include Standard =========================================//
//
//================ Include Peripherals ======================================//
#include "hw_memmap.h"
#include "hw_types.h"
#include "sysctl.h"
#include "rom.h"
//================ Include User App =========================================//
#include "system.h"
#include "systick.h"
//================ PUBLIC METHOD ============================================//
// extern function from another file
//
//================ PRIVATE METHOD ===========================================//
// non extern function
//
//================ PUBLIC DATA ==============================================//
// extern data
//
//================ PRIVATE DATA =============================================//
// non extern data
//
//================ PRIVATE DEFINE ===========================================//
//
//================ PRIVATE MACRO ============================================//
//
//================ SOURCE CODE ==============================================//
extern void TestSystick(void);
void SysTickHanlder(void) {
	TestSystick();
}

void SystemInit(void) {
	//
	// Set the clocking to run directly from the crystal.
	//
	ROM_SysCtlClockSet(
			SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_XTAL_16MHZ
					| SYSCTL_OSC_MAIN);
	//
	// Enable Peripherals
	//
	// GPIO
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA
			| SYSCTL_PERIPH_GPIOB
			| SYSCTL_PERIPH_GPIOD
			| SYSCTL_PERIPH_GPIOE
			| SYSCTL_PERIPH_GPIOF );
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
#ifdef LCD
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);
#endif
}

void SystemTickInit(void) {
	ROM_SysTickPeriodSet(SysCtlClockGet()/1000);
	ROM_SysTickIntEnable();
	ROM_SysTickEnable();
}

/*void RegisterCallBackFN(SystickCallBack_t* cbFn) {

}*/


//================ END OF FILE ==============================================//
