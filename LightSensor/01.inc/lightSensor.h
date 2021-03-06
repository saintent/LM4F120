//================ File Despciption =========================================//
//=== File name : LIGHTSENSOR_H
//=== 
//===========================================================================//

#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

//================ Include Standard =========================================//
//
//================ Include Peripherals ======================================//
#include "gpio.h"
//================ Include User App =========================================//
#include "system.h"
//================ PULBIC DEFINE ============================================//
#define LIGHTSENSOR_PORT		GPIO_PORTE_BASE
#define LIGHTSENSOR_PIN			GPIO_PIN_3
//================ PUBLIC MACRO =============================================//
//
//================ TYPEDEF DATA TYPE DEFINITION =============================//
//
//================ ENUMERATOR DEFINITION ====================================//
//
//================ TYPEDEF FUNCTION TYPE DEFFINITION ========================//
//
//================ TYPEDEF STRUCT/UNION =====================================//
//
//================ EXTERN FUNCTION ==========================================//
void LightSensorInit(void);
void StartLightSensor(void);
void GetLightIntensity(uint8_t* Data);
//================ EXTERN FUNCTION POINTER ==================================//
//
//================ EXTERN VARIABLE ==========================================//
//
//================ EXTERN QUEUE =============================================//
//
//================ END OF FILE ==============================================//
#endif
//#ifndef LIGHTSENSOR_H
