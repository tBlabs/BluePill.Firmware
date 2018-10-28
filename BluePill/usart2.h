#ifndef USART2_H_
#define USART2_H_

void Usart2_Init();
void Usart2_SendChar(char c);
void Usart2_SendString(char * str);
void Usart2_EnableTxInterrupt();

#endif /* USART2_H_ */
