#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "typedefs.h"

typedef enum { buttonState_NOT_PRESSED, buttonState_PRESSED } buttonState_t;

void Inputs_Init(void);
buttonState_t Input_GetValue(u8 inputIndex);

#endif /* BUTTONS_H_ */
