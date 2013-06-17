//================ File Despciption =========================================//
//=== File name : source.c
//=== 
//===========================================================================//


//================ Index ====================================================//
//
//================ Include Standard =========================================//
#include "stdint.h"
//================ Include Peripherals ======================================//
#include "hw_types.h"
#include "adc.h"
#include "gpio.h"
//================ Include User App =========================================//
#include "system.h"
#include "lightsensor.h"
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
uint32_t LigthIntensityValue = 0;

//================ PRIVATE DEFINE ===========================================//
//
//================ PRIVATE MACRO ============================================//
//
//================ SOURCE CODE ==============================================//
void LightSensorIntHandler(void) {
	//
	// Get ADC Data
	//
	ADCSequenceDataGet(ADC0_BASE, 3, &(LigthIntensityValue));

	//
	// Clear the ADC interrupt flag.
	//
	ROM_ADCIntClear(ADC0_BASE, 3);
}

void LightSensorInit(void) {
	// Set up pin as ADC
	ROM_GPIOPinTypeADC(LIGHTSENSOR_PORT, LIGHTSENSOR_PIN);

	ROM_ADCSequenceConfigure(ADC0_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

	ROM_ADCSequenceStepConfigure(ADC0_BASE, 3, 0, ADC_CTL_CH0 | ADC_CTL_IE
			| ADC_CTL_END);

	ROM_ADCSequenceEnable(ADC0_BASE, 3);

	ROM_ADCIntEnable(ADC0_BASE, 3);

	ROM_ADCIntClear(ADC0_BASE, 3);

	ADCIntRegister(ADC0_BASE, 3, LightSensorIntHandler);
}

void StartLightSensor(void) {
	ROM_ADCProcessorTrigger(ADC0_BASE, 3);
}
//*****************************************************************************
//
//! Get value of light intensity (percentage).
//! \param Data Address of data to get value.
//!
//! \return none.
//
//*****************************************************************************
void GetLightIntensity(uint8_t* Data) {
	uint8_t temp;
	temp = (uint8_t)((LigthIntensityValue*100)/4096);
	Data[0] = temp;
	//return temp;
}

//================ END OF FILE ==============================================//
