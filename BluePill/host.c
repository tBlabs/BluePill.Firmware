#include "host.h"
#include <frame.h>
#include <usart1.h>
#include <typedefs.h>
#include <string.h>
#include "error.h"
#include "global.h"
#include "outputs.h"
#include "inputs.h"
#include "params.h"
#include "rtc.h"
#include <parser.h>
#include "pins.h"
#include "timeout.h"
#include "debug.h"
#include "pwms.h"
#include "adc.h"
#include "temperature_sensor.h"
#include "displays.h"
#include "buzzer.h"
#include "dac.h"


#define SENSORS_COUNT	11 // 6xInput, 4xADC, 1xRTC

void SendPong(void)
{
	FrameBufferBuilder_Start(&usart1TxCycleBuffer, responseFrameType_Pong);          // Start frame in Tx buffer
	FrameBufferBuilder_End(&usart1TxCycleBuffer);          // Recalculate frame size, crc, etc.
	Usart1_EnableTxInterrupt();          // Send data from usart1RxCycleBuffer
}

void SendError(error_t errorCode)
{
	FrameBufferBuilder_Start(&usart1TxCycleBuffer, responseFrameType_Error);          // Start frame in Tx buffer
	CycleBuffer_Add(&usart1TxCycleBuffer, (u8)errorCode);
	FrameBufferBuilder_End(&usart1TxCycleBuffer);          // Recalculate frame size, crc, etc.
	Usart1_EnableTxInterrupt();          // Send data from usart1RxCycleBuffer
}

void SendPushState(boolean state)
{
	FrameBufferBuilder_Start(&usart1TxCycleBuffer, responseFrameType_PushStateUpdate);          // Start frame in Tx buffer
	CycleBuffer_Add(&usart1TxCycleBuffer, (u8)state);
	FrameBufferBuilder_End(&usart1TxCycleBuffer);          // Recalculate frame size, crc, etc.
	Usart1_EnableTxInterrupt();          // Send data from usart1RxCycleBuffer
}

void SendValue(u8 addr, u32 value)
{
	FrameBufferBuilder_Start(&usart1TxCycleBuffer, responseFrameType_Update);          
	CycleBuffer_Add(&usart1TxCycleBuffer, addr);
	CycleBuffer_AddU32(&usart1TxCycleBuffer, value);
	FrameBufferBuilder_End(&usart1TxCycleBuffer);         
	Usart1_EnableTxInterrupt();        
}

void SendValues(u32 * values)
{
	FrameBufferBuilder_Start(&usart1TxCycleBuffer, responseFrameType_UpdateAll);       
	for(u8 i = 0 ; i < SENSORS_COUNT ; i++)
		CycleBuffer_AddU32(&usart1TxCycleBuffer, values[i]);
	FrameBufferBuilder_End(&usart1TxCycleBuffer);       
	Usart1_EnableTxInterrupt();         
}

typedef enum 
{
	ioAddr_Input1, 
	ioAddr_Input2, 
	ioAddr_Input3, 
	ioAddr_Input4, 
	ioAddr_Input5, 
	ioAddr_Input6, 
	ioAddr_Adc1, 
	ioAddr_Adc2, 
	ioAddr_Adc3, 
	ioAddr_Adc4, 
	ioAddr_Rtc,  
	ioAddr_Output1,
	ioAddr_Output2,
	ioAddr_Output3,
	ioAddr_Output4,
	ioAddr_Output5,
	ioAddr_Output6,
	ioAddr_Output7,
	ioAddr_Pwm1, 
	ioAddr_Pwm2,
	ioAddr_Pwm3,
	ioAddr_Pwm4, 
	ioAddr_Display1,
	ioAddr_Display1Dot,
	ioAddr_Buzzer1Enable,
	ioAddr_Buzzer1Volume,
	ioAddr_Buzzer1Frequency
}
ioAddr_t;


u32 PeripheralsManager_GetValueByAddr(ioAddr_t ioAddr)
{
	switch (ioAddr)
	{
		case ioAddr_Input1: return Input_GetValue(0);
		case ioAddr_Input2: return Input_GetValue(1);
		case ioAddr_Input3: return Input_GetValue(2);
		case ioAddr_Input4: return Input_GetValue(3);
		case ioAddr_Input5: return Input_GetValue(4);
		case ioAddr_Input6: return Input_GetValue(5);
		case ioAddr_Adc1: return Adc_GetValue(0);
		case ioAddr_Adc2: return Adc_GetValue(1);
		case ioAddr_Adc3: return Adc_GetValue(2);
		case ioAddr_Adc4: return Adc_GetValue(3);
		case ioAddr_Rtc: return Clock_Get();
		case ioAddr_Output1: return Output_GetValue(0);
		case ioAddr_Output2: return Output_GetValue(1);
		case ioAddr_Output3: return Output_GetValue(2);
		case ioAddr_Output4: return Output_GetValue(3);
		case ioAddr_Output5: return Output_GetValue(4);
		case ioAddr_Output6: return Output_GetValue(5);
		case ioAddr_Output7: return Output_GetValue(6);
		case ioAddr_Pwm1: return Pwm_GetValue(0);
		case ioAddr_Pwm2: return Pwm_GetValue(1);
		case ioAddr_Pwm3: return Pwm_GetValue(2);
		case ioAddr_Pwm4: return Pwm_GetValue(3);
		case ioAddr_Display1: return Display_GetValue(0);
		case ioAddr_Display1Dot: return DisplayDot_GetValue(0);
		case ioAddr_Buzzer1Enable: return Buzzer1_IsEnabled();
		case ioAddr_Buzzer1Volume: return Buzzer1_GetVolume();
		case ioAddr_Buzzer1Frequency: return Buzzer1_GetFrequency();
		default: return 0;
	}
	
	return 0;
}

boolean PeripheralsManager_SetValueByAddr(ioAddr_t ioAddr, u32 value)
{
	switch (ioAddr)
	{ 
		case ioAddr_Rtc: Clock_Set(value); break;
		case ioAddr_Output1: Output_Set(0, value); break;
		case ioAddr_Output2: Output_Set(1, value); break;
		case ioAddr_Output3: Output_Set(2, value); break;
		case ioAddr_Output4: Output_Set(3, value); break;
		case ioAddr_Output5: Output_Set(4, value); break;
		case ioAddr_Output6: Output_Set(5, value); break;
		case ioAddr_Output7: Output_Set(6, value); break;
		case ioAddr_Pwm1: Pwm_Set(0, value); break;
		case ioAddr_Pwm2: Pwm_Set(1, value); break;
		case ioAddr_Pwm3: Pwm_Set(2, value); break;
		case ioAddr_Pwm4: Pwm_Set(3, value); break;
		case ioAddr_Display1: Display_Set(0, value); break;
		case ioAddr_Display1Dot: DisplayDot_Set(0, value); break;
		case ioAddr_Buzzer1Enable: Buzzer1_Enable(value); break;
		case ioAddr_Buzzer1Volume: Buzzer1_SetVolume(value); break;
		case ioAddr_Buzzer1Frequency: Buzzer1_SetFrequency(value); break;
		default: return false;
	}
	
	return true;
}

void ReadAllSensors(u32 * values)
{
	for (u8 i = 0; i < SENSORS_COUNT; i++)
	{
		values[i] = PeripheralsManager_GetValueByAddr(i);
	}
}

boolean push = false;
u8 scanInterval = 20; // ms

void ReadAndSendAllSensorsValues()
{
	u32 values[SENSORS_COUNT];
	ReadAllSensors(values);
	SendValues(values);
}

void FrameParserSwitch(requestFrameType_t frameType, u8 * data, u16 dataSize)
{
	switch (frameType)
	{
		case requestFrameType_Ping:
			SendPong();
			break;
		
		case requestFrameType_PushStateSet:
			if (dataSize != 2)
			{
				SendError(error_INVALID_FRAME_SIZE);
				break;
			}
			push = data[0];
			scanInterval = data[1];
			SendPushState(push);		
			break;
		
		case requestFrameType_GetAll:
			ReadAndSendAllSensorsValues();
			break;

		case requestFrameType_Get:
			if (dataSize == 1)
			{
				ioAddr_t addr = (ioAddr_t)data[0];          // TODO: validate addr range
				
				u32 value = PeripheralsManager_GetValueByAddr(addr);
				
				SendValue(addr, value);
			}	
			else SendError(error_INVALID_FRAME_SIZE);
			break;
	
		case requestFrameType_Set: 
			if (dataSize == 5)
			{
				ioAddr_t addr = (ioAddr_t)data[0];          // TODO: validate addr range
				u32 value = GetU32(data, 1);
				
				boolean success = PeripheralsManager_SetValueByAddr(addr, value);
				if (success)
				{
					SendValue(addr, value);
				}
				else SendError(error_ARG_OUT_OF_RANGE);
			}	
			else SendError(error_INVALID_FRAME_SIZE);
			break;
		
		default: 
			SendError(error_INVALID_FRAME_TYPE);
			break;
	}
}

timeout_t pushTimekeeper;
u32 oldValues[SENSORS_COUNT];


void HostTask(void)
{
	static u8 b;
	static frame_t frame;
	
	if (push)
	{
		if (Tick(&pushTimekeeper, scanInterval))
		{
			u32 values[SENSORS_COUNT];
			ReadAllSensors(values);
			int areEqual = memcmp(values, oldValues, sizeof(values));
			if (areEqual != 0)
			{
				memcpy(oldValues, values, sizeof(values));
				SendValues(values);
			}
		}
	}

	while (CycleBuffer_GetNewByte(&usart1RxCycleBuffer, &b))
	{
		if (Frame_Parse(&frame, b))
		{
			FrameParserSwitch(frame.type, frame.data, frame.dataSize);
		}
	}
}
