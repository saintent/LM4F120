#include "stdint.h"
#include "stdlib.h"
#include "string.h"

#include "i80_interface.h"

uint8_t writeData[5] = { 1, 2, 3, 4, 5 };
uint8_t writeBuffer[5];
uint8_t writeP = 0;
uint8_t readData[10] = { 11, 12 ,13 ,14 ,15 ,16 ,17 ,18, 19, 20 };
uint8_t readBuffer[10];
uint8_t readP = 0;
uint8_t rT[10];

void TaskWriteHandler(uint8_t* data, uint16_t len) {
	(void) data;
	(void) len;
	I80Request(I80_WRITE, I80_DAT, writeData, 5);
        writeP = 0;
}

void TaskReadHandler(uint8_t* data, uint16_t len) {
	memcpy(rT,data,len);
	I80Request(I80_READ, I80_DAT, readBuffer, 10);
        readP = 0;
}

void PortOp(PortAction_t ac, uint8_t* d) {
	if(ac == I80_WRITE) {
		writeBuffer[writeP] = d[0];
		writeData[writeP++]++;
	}
	else {
		d[0] = readData[readP];
		readData[readP++]++;
	}
}

int main()
{
	I80IOMap_t iomap = { 0 };
	iomap.DATA = PortOp;
	I80Init(&iomap);
	RegisterTaskWriteCb(TaskWriteHandler);
	RegisterTaskReadCb(TaskReadHandler);

	I80Request(I80_WRITE, I80_DAT, writeData, 5);
	I80Request(I80_READ, I80_DAT, readBuffer, 10);

	while(1) {
		I80Tick();
	}

 // return 0;
}
