#ifndef FRAME_H_
#define FRAME_H_

/*************************************************************************************************/

#include "typedefs.h"
#include "crc.h"
#include "cycleBuffer.h"
#include "general.h"

/*************************************************************************************************/

typedef enum
{
	requestFrameType_Ping = 1,
	requestFrameType_Get = 2,
	requestFrameType_GetAllSensors = 3,
	requestFrameType_Set = 4,
	requestFrameType_ConfigRead = 5,
	requestFrameType_ConfigWrite = 6,
	requestFrameType_GetAll = 7,
}
requestFrameType_t;

typedef enum
{
	responseFrameType_Pong = 1,
	responseFrameType_Error = 2,
	responseFrameType_Update = 3,
	responseFrameType_UpdateAllSensors = 4,
	responseFrameType_ConfigUpdate = 5,
	responseFrameType_UpdateAll = 6
}
responseFrameType_t;


/*************************************************************************************************/

#define FRAME_SYNCHRO		((u16)0xAABB) // transmitted as little-endian!

#define FRAME_DATA_CAPACITY 1024

/*************************************************************************************************/


typedef struct //PACKED
{
	u8 type;   // could be type of frameType_t but only for transmitted frames
	u8 dataSize;   // real size of data[]
	u8 data[FRAME_DATA_CAPACITY];   // data buffer
	u8 xor;
	boolean valid;
}
frame_t;

/*************************************************************************************************/

boolean Frame_Parse(frame_t * frame, u8 b);
void Frame_Print(frame_t * frame);
void FrameBufferBuilder_Start(cycleBuffer_t * buffer, u8 frameType);
void FrameBufferBuilder_End(cycleBuffer_t * buffer);

/*************************************************************************************************/

#endif /* FRAME_H_ */
