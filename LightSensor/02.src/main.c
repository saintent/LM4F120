//================ File Despciption =========================================//
//=== File name : main.c
//=== 
//===========================================================================//


//================ Index ====================================================//
//
//================ Include Header ===========================================//
#include "stdlib.h"
#include "stdint.h"
//================ Include Peripherals ======================================//
#include "lm4f120h5qr.h"
#include "hw_types.h"
#include "interrupt.h"
//================ Include User App =========================================//
#include "system.h"
#include "lightsensor.h"
#include "ledMatrix.h"
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

struct {
	uint16_t TB1Sec;
}Timebase;

uint8_t TestNum = 0;

void TestSystick(void) {
	uint8_t mcd;
	//HWREGBITW(0x400253FC, 2) ^= 1;
	if(++Timebase.TB1Sec >= 1000) {
		StartLightSensor();
		GetLightIntensity(&mcd);
		Write7SegRam(mcd);
/*		if(++TestNum < 99) {
			Write7SegRam(TestNum);
		}
		else {
			TestNum = 0;
		}*/
		Timebase.TB1Sec = 0;
	}

	LedProcess();
	//GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, HWREGBITW(GPIO_PORTF_BASE, 2) ^ 1);
}


int main (int argc, char* argv[]) {
	(void) argc;
	(void) argv;
	Timebase.TB1Sec = 0;
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
        
	// Light sensor initialized.
	LightSensorInit();

	// 7 Segment Init
	LedInit();

	SystemTickInit();

	// Enable interrupt
	IntMasterEnable();

	//Write7SegRam(TestNum);

	while(1);
}
//================ END OF FILE ==============================================//
