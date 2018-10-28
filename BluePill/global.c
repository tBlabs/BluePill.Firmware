#include "global.h"

u8 usart1TxData[2400];
u8 usart1RxData[2400];

u8 usart2TxData[2400];
u8 usart2RxData[2400];

cycleBuffer_t usart1TxCycleBuffer = CycleBuffer_InitStructure(usart1TxData);
cycleBuffer_t usart1RxCycleBuffer = CycleBuffer_InitStructure(usart1TxData);

cycleBuffer_t usart2TxCycleBuffer = CycleBuffer_InitStructure(usart2TxData);
cycleBuffer_t usart2RxCycleBuffer = CycleBuffer_InitStructure(usart2TxData);
