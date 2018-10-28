/*
    19200:8:1:n
    Tx: A.9
    Rx: A.10
    NVIC Priority: 0+0
    Interrupts: Rx
 */

#include "usart1.h"
#include <misc.h>
#include "stm32f10x.h"
#include "typedefs.h"
#include "general.h"
#include "cycleBuffer.h"
#include "global.h"
#include "pins.h"
#include <stm32f10x_usart.h>

#define RxBufferNotEmptyInterrupt(usart) 	((usart)->SR & (1<<5))
#define ReceivedByte(usart) 				((usart)->DR)
#define SendByte(usart,byte)				((usart)->DR=(byte))
#define TxInterruptDisable(usart)			((usart)->CR1 &= ~(1<<7))
#define TxInterruptEnable(usart)			((usart)->CR1 |= (1<<7))
#define TxInterrupt(usart) 					(usart->SR & (1<<7))

void USART1_IRQHandler(void)
{
	// TX
	if(TxInterrupt(USART1))
	{
		static u8 b;

		if (CycleBuffer_GetNewByte(&usart1TxCycleBuffer, &b))
		{
			SendByte(USART1, b);
		}
		else TxInterruptDisable(USART1);
	}


	// RX (CLEAR ERRORS)
	volatile u32 temp UNUSED = USART1->SR;  // This clears errors flags (overrun etc.)

	// RX
	if(RxBufferNotEmptyInterrupt(USART1))
	{
		CycleBuffer_Add(&usart1RxCycleBuffer, ReceivedByte(USART1));
	}
}

void Usart1_EnableTxInterrupt()
{
	TxInterruptEnable(USART1);
}

void Usart1_Init()
{
	USART_DeInit(USART1);

	// GPIO init
	Pin_Init(&(pin_t)PinA9, GPIO_Mode_AF_PP);  // Tx pin
	Pin_Init(&(pin_t)PinA10, GPIO_Mode_IN_FLOATING);  // Rx pin

	// USART INTERRUPT PRIORITY SET
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// USART INIT
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	USART_InitTypeDef UsartConfig;
	UsartConfig.USART_BaudRate = 19200;
	UsartConfig.USART_WordLength = USART_WordLength_8b;
	UsartConfig.USART_StopBits = USART_StopBits_1;
	UsartConfig.USART_Parity = USART_Parity_No;
	UsartConfig.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	UsartConfig.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART1, &UsartConfig);

	USART1->CR1 |= (1 << 7);  // bit #7: TXEIE: TXE interrupt enable
	USART1->CR1 |= (1 << 5);  // bit #5: RXNEIE: RXNE interrupt enable

	// ENABLE UART
	USART_Cmd(USART1, ENABLE);
}

void Usart1_SendChar(char c)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) ;

	USART_SendData(USART1, c);
}

void Usart1_SendString(char * str)
{
	while (*str)
		Usart1_SendChar(*(str++));
}
