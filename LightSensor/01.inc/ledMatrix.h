//================ File Despciption =========================================//
//=== File name : LEDMATRIX_H
//=== 
//===========================================================================//

#ifndef LEDMATRIX_H
#define LEDMATRIX_H

//================ Config Lib ===============================================//
#define TI
//================ Include Standard =========================================//
#include "stdint.h"
//================ Include Peripherals ======================================//
#if defined (STM)
#elif defined (TI)
#include "system.h"
#include "gpio.h"
#else

#endif
//================ Include User App =========================================//
//
//================ PULBIC DEFINE ============================================//
// 7 Segment
//		  A
//		======
//	 F ||  G ||B
//	   ||====||
//	 E ||	 ||C
//		======  . DP
//		  D
#define SEG_A		1
#define SEG_B		2
#define SEG_C		4
#define SEG_D		8
#define SEG_E		16
#define SEG_F		32
#define SEG_G		64
#define SEG_DP		128

#define FRONT_0		SEG_A | SEG_B | SEG_C | SEG_D| SEG_E | SEG_F
#define FRONT_1		SEG_B | SEG_C
#define FRONT_2		SEG_A | SEG_B | SEG_G | SEG_E | SEG_D
#define FRONT_3		SEG_A | SEG_B | SEG_C | SEG_D | SEG_G
#define FRONT_4		SEG_B | SEG_C | SEG_G | SEG_F
#define FRONT_5		SEG_A | SEG_C | SEG_D | SEG_F | SEG_G
#define FRONT_6		SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G
#define FRONT_7		SEG_A | SEG_B | SEG_C
#define FRONT_8		SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G
#define FRONT_9		SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G
//================ PUBLIC MACRO =============================================//
//
//================ TYPEDEF DATA TYPE DEFINITION =============================//
//
//================ ENUMERATOR DEFINITION ====================================//
typedef enum LedType_tag {
	COLUMN_0 = 0,
	COLUMN_1,
	COLUMN_2,
	COLUMN_3,
	COLUMN_4,
	COLUMN_5,
	COLUMN_6,
	COLUMN_7
}LedMatrix_t;

typedef enum Seg7_tag {
	DIGI_0 = 0,
	DIGI_1,
	DIGI_2,
	DIGI_3
}Seg7Type_t;


//================ TYPEDEF FUNCTION TYPE DEFFINITION ========================//
//
//================ TYPEDEF STRUCT/UNION =====================================//
typedef struct LedCtr_tag {
	LedMatrix_t	Matrix;
	Seg7Type_t	Digi;
	uint16_t	rsv;
}LedCtr_t;
//================ EXTERN FUNCTION ==========================================//
void LedInit(void);
void LedProcess(void);
void Write7SegRam(uint8_t Value);
//================ EXTERN FUNCTION POINTER ==================================//
//
//================ EXTERN VARIABLE ==========================================//
//
//================ EXTERN QUEUE =============================================//
//
//================ END OF FILE ==============================================//
#endif
//#ifndef LEDMATRIX_H
