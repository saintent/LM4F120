//================ File Despciption =========================================//
//=== File name : source.c
//=== 
//===========================================================================//


//================ Index ====================================================//
//
//================ Include Standard =========================================//
//
//================ Include Peripherals ======================================//
#include "lm4f120h5qr.h"
//================ Include User App =========================================//
#include "ledmatrix.h"
////================ PUBLIC METHOD ============================================//
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
const uint8_t DIGI_FRONT[10] = { FRONT_0, FRONT_1, FRONT_2, FRONT_3, FRONT_4,
		FRONT_5, FRONT_6, FRONT_7, FRONT_8, FRONT_9 };
uint16_t displayRam[8] = { 0 };
uint16_t Seg7Ram[4] = { 0 };
LedCtr_t LedControl= { 0 };
#if defined (STM)
#elif defined (TI)
uint32_t LED_COM_PORT[] = { GPIO_PORTE_BASE, GPIO_PORTE_BASE };
uint32_t LED_COM_PIN[] = { GPIO_PIN_1, GPIO_PIN_2 };
#endif
//================ PRIVATE DEFINE ===========================================//
//
//================ PRIVATE MACRO ============================================//
//
//================ SOURCE CODE ==============================================//

/*void LedMatrixWrite(uint8_t Value) {
	(void) Value;
}*/

void LedSeg7Write(uint8_t Value) {
	GPIO_PORTD_DATA_R &= 0xF0;
	GPIO_PORTD_DATA_R |= Value & 0x0F;
	GPIO_PORTA_DATA_R &= 0x0F;
	GPIO_PORTA_DATA_R |= Value & 0xF0;
}

void LedInit(void) {
#if defined (STM)
#elif defined (TI)
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_4
			| GPIO_PIN_5
			| GPIO_PIN_6
			| GPIO_PIN_7);
	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0
			| GPIO_PIN_1
			| GPIO_PIN_2
			| GPIO_PIN_3);
	GPIOPinTypeGPIOOutputOD(LED_COM_PORT[0], LED_COM_PIN[0] | LED_COM_PIN[1]);
#endif
}

/*
void LedMatrix(void) {
	LedMatrixWrite(0);
		switch (LedControl.Matrix) {
			case COLUMN_0:
				GPIO_PORTE_DATA_R &= ~0x06;
				GPIO_PORTE_DATA_R |= GPIO_PIN_1;

				LedSegtWrite(displayRam[COLUMN_0]);

				LedControl.Digi = COLUMN_1;
				break;
			case COLUMN_1 :
				GPIO_PORTE_DATA_R &= ~0x06;
				GPIO_PORTE_DATA_R |= GPIO_PIN_2;

				LedSegtWrite(displayRam[COLUMN_1]);

				LedControl.Digi = COLUMN_0;

				break;
			default:
				break;
		}
}
*/

void Seg7(void) {
	LedSeg7Write(0);
	switch (LedControl.Digi) {
		case DIGI_0:
			GPIO_PORTE_DATA_R &= ~0x06;
			GPIO_PORTE_DATA_R |= GPIO_PIN_1;

			LedSeg7Write(Seg7Ram[DIGI_0]);

			LedControl.Digi = DIGI_1;
			break;
		case DIGI_1 :
			GPIO_PORTE_DATA_R &= ~0x06;
			GPIO_PORTE_DATA_R |= GPIO_PIN_2;

			LedSeg7Write(Seg7Ram[DIGI_1]);

			LedControl.Digi = DIGI_0;

			break;
		default:
			break;
	}
}

void LedProcess(void) {
	Seg7();
}

void Write7SegRam(uint8_t Value) {
	uint8_t temp = 0;
	uint8_t sBit = 0;
	uint8_t multiple = 10;

	memset(Seg7Ram, 0, 4);

	if(Value == 0) {
		Seg7Ram[0] = DIGI_FRONT[0];
		Seg7Ram[1] = DIGI_FRONT[0];
	}
	else {
		temp = Value;
		while(temp) {
			Seg7Ram[sBit] = DIGI_FRONT[temp%multiple];
			temp = temp/multiple;
			multiple = multiple * 10;
			sBit++;
		}

	}
}
//================ END OF FILE ==============================================//
