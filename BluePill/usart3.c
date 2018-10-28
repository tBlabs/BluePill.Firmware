/*
    19200:8:1:n
    Tx: B10
    Rx: B11
    NVIC Priority: 0+0
    Interrupts: Rx
 */

#include "usart3.h"
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

void USART3_IRQHandler(void)
{
	// TX
	if(TxInterrupt(USART3))
	{
		static u8 b;

		if (CycleBuffer_GetNewByte(&usart1TxCycleBuffer, &b))
		{
			SendByte(USART3, b);
		}
		else TxInterruptDisable(USART3);
	}


	// RX (CLEAR ERRORS)
	volatile u32 temp UNUSED = USART3->SR;  // This clears errors flags (overrun etc.)

	// RX
	if(RxBufferNotEmptyInterrupt(USART3))
	{
		CycleBuffer_Add(&usart1RxCycleBuffer, ReceivedByte(USART3));
	}
}

void Usart3_EnableTxInterrupt()
{
	TxInterruptEnable(USART3);
}

void Usart3_Init()
{
	USART_DeInit(USART3);

	// GPIO init
	Pin_Init(&(pin_t)PinB10, GPIO_Mode_AF_PP);  // Tx pin
	Pin_Init(&(pin_t)PinB11, GPIO_Mode_IN_FLOATING);  // Rx pin

	// USART INTERRUPT PRIORITY SET

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// USART INIT

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	USART_InitTypeDef UsartConfig;
	UsartConfig.USART_BaudRate = 19200;
	UsartConfig.USART_WordLength = USART_WordLength_8b;
	UsartConfig.USART_StopBits = USART_StopBits_1;
	UsartConfig.USART_Parity = USART_Parity_No;
	UsartConfig.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	UsartConfig.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART3, &UsartConfig);

	USART3->CR1 |= (1 << 7);  // bit #7: TXEIE: TXE interrupt enable
	USART3->CR1 |= (1 << 5);  // bit #5: RXNEIE: RXNE interrupt enab

	// ENABLE UART
	USART_Cmd(USART3, ENABLE);
}

void Usart3_SendChar(char c)
{
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET) ;

	USART_SendData(USART3, c);
}

void Usart3_SendString(char * str)
{
	while (*str)
		Usart3_SendChar(*(str++));
}
