#include "frame.h"
#include "debug.h"
#include "general.h"
#include "crcCalc.h"
#include "string.h"
#include "cycleBuffer.h"

#define GoTo(step)		{ explodeStep = (step); }
#define NextStep()		explodeStep++

enum
{
	Synchro_Byte1, 
	Synchro_Byte2, 
	FrameType, 
	DataSize,
	Data,
	Xor 
}
explodeStep = Synchro_Byte1;

void FrameReset(frame_t * frame)
{
	frame->valid = false;
	frame->xor = 0;
	frame->dataSize = 0;
}

boolean Frame_Parse(frame_t * frame, u8 b)
{
	static u8 dataCounter;
	static u8 xor;
	
	if (explodeStep != Xor)
	{
		xor ^= b;
	}

	switch (explodeStep)
	{
		case Synchro_Byte1:	
			if (b == 0xBB) GoTo(Synchro_Byte2);
			break;

		case Synchro_Byte2:	
			if (b == 0xAA)
			{
				NextStep();
				frame->valid = false;
				xor = 0;
				xor ^= 0xBB;
				xor ^= 0xAA;
			}
			else GoTo(Synchro_Byte1);

			break;

		case FrameType:		
			frame->type = b;
			GoTo(DataSize);
			break;

		case DataSize:		
			frame->dataSize = b;
								
			if (frame->dataSize > 0)
			{
				dataCounter = 0;
				GoTo(Data);
			}
			else
			{
				GoTo(Xor);
			}						

			break;

		case Data:		
			frame->data[dataCounter++] = b;

			if (dataCounter >= frame->dataSize)
			{
				GoTo(Xor);
			}

			break;
		
		case Xor:
			GoTo(Synchro_Byte1);
			
			if (xor == b)
			{
				Debug("xor ok");
				return true;
			}
			else
			{
				Debug("xor err", xor, b);
				int dummy = xor;
			}
		
			break;

		default: 
			GoTo(Synchro_Byte1); 
			break;
	}

	return false;
}

static u16 startPoint;
static u16 dataStartPoint;

void FrameBufferBuilder_Start(cycleBuffer_t * buffer, u8 frameType)
{
	startPoint = CycleBuffer_GetCurrentWritePosition(buffer);            

	CycleBuffer_Add(buffer, 0xAB);
	CycleBuffer_Add(buffer, frameType);

	dataStartPoint = CycleBuffer_GetCurrentWritePosition(buffer);
}

void FrameBufferBuilder_End(cycleBuffer_t * buffer)
{
	u16 thisPoint = CycleBuffer_GetCurrentWritePosition(buffer);
	u8 xor = CycleBuffer_Xor(buffer, startPoint, thisPoint);
	CycleBuffer_Add(buffer, xor);
}
