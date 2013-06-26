//================ File Despciption =========================================//
//=== File name : i80_interface.c
//=== 
//===========================================================================//


//================ Index ====================================================//
//  i80 system interface 8/16 bit
//	Stack memory use
//		--	IOMap			Use for read write io port of variant
//							microcontroller
//		--	i80TaskControl 	Use for control task
//
//		||===============================||
//		||		IOMap		32 byte		 ||
//		||===============================||
//		||		TaskControl	8 byte		 ||
//		||===============================||
//		||		TaskEndCallBack	4 byte	 ||
//		||===============================||
//										Total 44 byte
//
//================ Include Standard =========================================//
#include "stdint.h"
#include "stdlib.h"
#include "string.h"
//================ Include Peripherals ======================================//
//
//================ Include User App =========================================//
#include "i80_interface.h"
//================ PUBLIC METHOD ============================================//
// extern function from another file
//
//================ PRIVATE METHOD ===========================================//
// non extern function
void IOPortDump(OPState_t state);
void PortDump(PortAction_t action, uint8_t* data);
void TaskEndCallBackDefault(uint8_t* data, uint16_t len);
void WriteIns(void* Task);
void WriteData(void* Task);
void ReadIns(void* Task);
void ReadData(void* Task);
//================ PUBLIC DATA ==============================================//
// extern data
//
//================ PRIVATE DATA =============================================//
// non extern data
I80IOMap_t iOMap = { .RS = IOPortDump,
		.RD = IOPortDump,
		.WR = IOPortDump,
		.CS = IOPortDump,
		.DATA = PortDump };
I80TaskControl 	i80TaskControl;
TaskEndCallBack_t* taskReadCallBack = TaskEndCallBackDefault;
TaskEndCallBack_t* taskWriteCallBack = TaskEndCallBackDefault;
//================ PRIVATE DEFINE ===========================================//
//
//================ PRIVATE MACRO ============================================//
//
//================ SOURCE CODE ==============================================//
void I80Init(I80IOMap_t* ioMapFn) {
	memset((void*)&i80TaskControl, 0 ,sizeof(i80TaskControl));
	if(ioMapFn->CS != 0) {
		iOMap.CS = ioMapFn->CS;
	}
	if(ioMapFn->DATA != 0) {
		iOMap.DATA = ioMapFn->DATA;
	}
 	if(ioMapFn->RD != 0) {
 		iOMap.RD = ioMapFn->RD;
 	}
 	if(ioMapFn->RS != 0) {
 		iOMap.RS = ioMapFn->RS;
 	}
 	if(ioMapFn->WR != 0) {
 		iOMap.WR = ioMapFn->WR;
 	}
}

void RegisterTaskWriteCb(TaskEndCallBack_t* taskCallBack) {
	taskWriteCallBack = taskCallBack;
}

void UnRegisTaskWriteCb(void) {
	taskWriteCallBack = TaskEndCallBackDefault;
}

void RegisterTaskReadCb(TaskEndCallBack_t* taskCallBack) {
	taskReadCallBack = taskCallBack;
}

void UnRegisTaskReadCb(void) {
	taskReadCallBack = TaskEndCallBackDefault;
}

uint8_t I80Request(i80RequestType_t requestType, i80AccessType_t accessType,
		uint8_t* Data, uint16_t len) {
	uint8_t result = 0;
	uint8_t getLast = 0;
	I80Task_t* newTask,*current;
	uint8_t loop;
	if(i80TaskControl.Task < MAX_I80_TASK) {
		// Access locate for new task
		newTask = (I80Task_t*)malloc(sizeof(I80Task_t));
		if(newTask != 0) {
			// Register new task
			newTask->Task.Data = Data;
			newTask->Task.Current = 0;
			newTask->Task.End = len;
			newTask->Task.AccessType = accessType;
			newTask->Task.RequestType = requestType;
			//newTask->Next = 0;
			// instruction access and read request
			if((accessType == I80_INS) && (requestType == I80_READ)) {
				newTask->Task.TaskHandler = ReadIns;
			}
			// instruction access and write request
			else if ((accessType == I80_INS) && (requestType == I80_WRITE)){
				newTask->Task.TaskHandler = WriteIns;
			}
			// Data access and read request
			else if ((accessType == I80_DAT) && (requestType == I80_READ)){
				newTask->Task.TaskHandler = ReadData;
			}
			// Data access and write request
			else if ((accessType == I80_DAT) && (requestType == I80_WRITE)) {
				newTask->Task.TaskHandler = WriteData;
			}
			else {
				// Error cannot get request type or access type
				result = 0;
				// free allocate memory
				free((void*)newTask);
				goto NEW_TASK_END;
			}
			newTask->Next = 0;
			// Find last point
			loop = i80TaskControl.Task;
			current = i80TaskControl.CurrentTask;
			if(loop  == 0) {
				//current = newTask;
				i80TaskControl.CurrentTask = newTask;
				i80TaskControl.CurrentMemoryUse += 20;
				i80TaskControl.Task++;
				result = 1;
			}
			else {
				while (loop) {
					if (current->Next == 0) {
						// last point
						current->Next = newTask;
						i80TaskControl.CurrentMemoryUse += 20;
						i80TaskControl.Task++;
						getLast = 1;
						result = 1;
						break;
					}
					else {
						// Get next point
						current = current->Next;
						loop--;
					}
				}
				if (!getLast) {
					// fail to get last point
					free((void*) newTask);
				}
			}

		}
	}
	NEW_TASK_END :
	return result;
}

void WriteIns(void* Task) {
	uint8_t portValue;
	// Operate
	portValue = 0;
	//	-- Set Port output active low
	//iOMap.DATA(I80_PORT_WRITE, &portValue);
	// Clear RS of instruction command
	iOMap.RS(STATE_CLR);
	// Set RD, Clear WR
	iOMap.RD(STATE_SET);
	iOMap.WR(STATE_CLR);
	// Write Data
	iOMap.DATA(I80_PORT_WRITE,
			&((I80TaskAttr_t*)Task)->Data[((I80TaskAttr_t*)Task)->Current]);
	// Clear CS
	iOMap.CS(STATE_CLR);
	asm("nop");
	asm("nop");
	asm("nop");
	iOMap.CS(STATE_SET);
	// Set WR
	iOMap.WR(STATE_SET);

}

void WriteData(void* Task) {
	uint8_t portValue;
	// Operate
	portValue = 0;
	//	-- Set Port output active low
	//iOMap.DATA(I80_PORT_WRITE, &portValue);
	// Set RS
	iOMap.RS(STATE_SET);
	//  RD, Clear WR
	iOMap.RD(STATE_SET);
	iOMap.WR(STATE_CLR);
	// Write Data
	iOMap.DATA(I80_PORT_WRITE,
			&((I80TaskAttr_t*)Task)->Data[((I80TaskAttr_t*)Task)->Current]);
	// Clear CS
	iOMap.CS(STATE_CLR);
	asm("nop");
	asm("nop");
	asm("nop");
	iOMap.CS(STATE_SET);
	// Set WR
	iOMap.WR(STATE_SET);
}

void ReadIns(void* Task) {
	uint8_t portValue;
	// Operate
	portValue = 0;
	//	-- Set Port output active low
	//iOMap.DATA(I80_PORT_WRITE, &portValue);
	// Clear RS of instruction command
	iOMap.RS(STATE_CLR);
	// Clear RD, Set WR
	iOMap.RD(STATE_CLR);
	iOMap.WR(STATE_SET);
	// Clear CS
	iOMap.CS(STATE_CLR);
	asm("nop");
	asm("nop");
	asm("nop");
	// Read Data
	iOMap.DATA(I80_PORT_READ,
			&((I80TaskAttr_t*)Task)->Data[((I80TaskAttr_t*)Task)->Current]);
	iOMap.CS(STATE_SET);
	// Set WR
	iOMap.RD(STATE_SET);
}

void ReadData(void* Task) {
	uint8_t portValue;
	// Operate
	portValue = 0;
	//	-- Set Port output active low
	//paiOMap.DATA(I80_PORT_WRITE, &portValue);
	// Set RS
	iOMap.RS(STATE_SET);
	// Clear RD, Set WR
	iOMap.RD(STATE_CLR);
	iOMap.WR(STATE_SET);
	// Clear CS
	iOMap.CS(STATE_CLR);
	asm("nop");
	asm("nop");
	asm("nop");
	// Read Data
	iOMap.DATA(I80_PORT_READ,
			&((I80TaskAttr_t*)Task)->Data[((I80TaskAttr_t*)Task)->Current]);
	iOMap.CS(STATE_SET);
	// Set WR
	iOMap.RD(STATE_SET);
}

void I80Tick(void) {
	I80Task_t **currentTask, *prvTask;
	//I80Task_t **ppCurrentTask;
	I80TaskAttr_t* task;

	if(i80TaskControl.Task != 0) {
		currentTask = &i80TaskControl.CurrentTask;
		task = &currentTask[0]->Task;
		//ppCurrentTask = &currentTask;
		task->TaskHandler((void*)task);
		// if end of process of this task
		if(++task->Current >= task->End) {
			// Call back
			if(task->RequestType == I80_READ) {
				taskReadCallBack(task->Data, task->End);
			}
			else {
				taskWriteCallBack(task->Data, task->End);
			}

			prvTask = *currentTask;
			// Get next Task
			if(currentTask[0]->Next != 0) {
				// Pointed to next task
				currentTask[0] = currentTask[0]->Next;
			}
			i80TaskControl.Task--;
			// free memory of previous task
			free((void*)prvTask);
		}
	}
}

void IOPortDump(OPState_t state) {
	(void) state;
}
void PortDump(PortAction_t action, uint8_t* data) {
	(void) action;
	(void) data;
}
void TaskEndCallBackDefault(uint8_t* data, uint16_t len) {
	(void) data;
	(void) len;
}
//================ END OF FILE ==============================================//
