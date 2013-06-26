//================ File Despciption =========================================//
//=== File name : i80 system interface
//=== 
//===========================================================================//

#ifndef I80_INTERFACE_H
#define I80_INTERFACE_H

//================ Include Standard =========================================//
#include "stdint.h"
//================ Include Peripherals ======================================//
//
//================ Include User App =========================================//
//
//================ PULBIC DEFINE ============================================//
#define MAX_I80_TASK			8
//================ PUBLIC MACRO =============================================//
//
//================ TYPEDEF DATA TYPE DEFINITION =============================//
//
//================ ENUMERATOR DEFINITION ====================================//
typedef enum OpState_tag {
	STATE_CLR = 0,
	STATE_SET
}OPState_t;

typedef enum PortAction_tag {
	I80_PORT_READ = 0,
	I80_PORT_WRITE
}PortAction_t;

typedef enum i80_access_tag {
	I80_INS = 0,
	I80_DAT
}i80AccessType_t;

typedef enum i80_request_tag {
	I80_READ = 0,
	I80_WRITE
}i80RequestType_t;
//================ TYPEDEF FUNCTION TYPE DEFFINITION ========================//
typedef void IOOperation_t(OPState_t);
typedef void I80TaskHandler_t(void*);
typedef void PortOperation_t(PortAction_t,uint8_t*);
typedef void TaskEndCallBack_t(uint8_t*, uint16_t);
//================ TYPEDEF STRUCT/UNION =====================================//
/*
 * Size of : 32
 */
typedef struct ioMap_tag {
	IOOperation_t*		RS;
	IOOperation_t*		RD;
	IOOperation_t*		WR;
	IOOperation_t*		CS;
	PortOperation_t*	DATA;
	uint32_t			rsv[3];
}I80IOMap_t;

/*
 * Size of : 16
 */
typedef struct i80TaskAttr_tag {
	uint8_t*			Data;
	uint16_t			Current;
	uint16_t			End;
	I80TaskHandler_t*	TaskHandler;
	i80RequestType_t	RequestType;
	i80AccessType_t		AccessType;
	uint8_t				rsv[2];
}I80TaskAttr_t;


/*
 * size of : 20
 */
/*typedef struct i80Task_tag {
	I80TaskAttr_t		Task;
	void*				Next;
}I80Task_t;*/
typedef struct i80Task_tag {
	I80TaskAttr_t		Task;
	struct i80Task_tag*		Next;
}I80Task_t;

/*
 * Size of : 8
 */
typedef struct i80TaskControl_tag {
	// 4 byte
	I80Task_t*			CurrentTask;
	// 1 byte
	struct {
		uint8_t			Busy:1;
		uint8_t			OverFlow:1;
		uint8_t			rsv:6;
	}Status;
	// 1 byte
	uint8_t				Task;
	// 2 byte
	uint16_t			CurrentMemoryUse;
	// Padding for 20 byte
	//uint8_t				rsv[20];
}I80TaskControl;
//================ EXTERN FUNCTION ==========================================//
void I80Init(I80IOMap_t* ioMapFn);
void RegisterTaskWriteCb(TaskEndCallBack_t* taskCallBack);
void UnRegisTaskWriteCb(void);
void RegisterTaskReadCb(TaskEndCallBack_t* taskCallBack);
void UnRegisTaskReadCb(void);

uint8_t I80Request(i80RequestType_t requestType, i80AccessType_t accessType,
		uint8_t* Data, uint16_t len);

void I80Tick(void);


//================ EXTERN FUNCTION POINTER ==================================//
//
//================ EXTERN VARIABLE ==========================================//
//
//================ EXTERN QUEUE =============================================//
//
//================ END OF FILE ==============================================//
#endif
//#ifndef I80_INTERFACE_H
