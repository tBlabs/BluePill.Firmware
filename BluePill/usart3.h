#ifndef USART3_H_
#define USART3_H_

void Usart3_Init();
void Usart3_SendChar(char c);
void Usart3_SendString(char * str);
void Usart3_EnableTxInterrupt();

#endif /* USART3_H_ */
