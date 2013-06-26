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
#include "flash.h"
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
uint16_t IntensityMin;
uint16_t IntensityMax;
uint16_t IntenDiff;
uint32_t LightIntensityValue = 0;
//uint8_t  LightIntensity = 0;

//================ PRIVATE DEFINE ===========================================//
//
//================ PRIVATE MACRO ============================================//
//
//================ SOURCE CODE ==============================================//
void LightSensorIntHandler(void) {
	//
	// Get ADC Data
	//
	ADCSequenceDataGet(ADC0_BASE, 3, &(LightIntensityValue));
	if(LightIntensityValue < IntensityMin ) {
		LightIntensityValue = IntensityMin;
	}
	else if(LightIntensityValue > IntensityMax) {
		LightIntensityValue = IntensityMax;
	}

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

	IntensityMax = 4096;
	IntenDiff = 4096;
	IntensityMin = 0;
	


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
	uint16_t rawData;

	if (IntenDiff != 0) {
		rawData = LightIntensityValue - IntensityMin;
		temp = (uint8_t) ((rawData * 100) / IntenDiff);
		if(temp >= 100) {
			temp = 99;
		}
		else {
			Data[0] = temp;
		}

	}
	else {
		Data[0] = 0;
	}
	//return temp;
}

void CalibrateIntensity(uint8_t type, uint8_t rst) {
	if(!rst) {
		if (type == LIGHT_LO_CAL) {
			IntensityMin = LightIntensityValue;
		}
		else {
			IntensityMax = LightIntensityValue;
		}
		IntenDiff = IntensityMax - IntensityMin;
	}
	else {
		IntensityMax = 4096;
		IntenDiff = 4096;
		IntensityMin = 0;
	}
}

//================ END OF FILE ==============================================//
