#ifndef USART1_H_
#define USART1_H_

void Usart1_Init();
void Usart1_SendChar(char c);
void Usart1_SendString(char * str);
void Usart1_EnableTxInterrupt();

#endif /* USART2_H_ */
