#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <stdint.h>

void init_keypad(void);
uint8_t KepadUpdate(void);
uint8_t KEYPAD_Scan(uint8_t *pkey);

#endif /* KEYPAD_H_ */
