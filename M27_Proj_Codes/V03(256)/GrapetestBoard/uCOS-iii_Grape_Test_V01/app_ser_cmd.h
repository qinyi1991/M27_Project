#ifndef __APP_SER_CMD_H
#define __APP_SER_CMD_H

#include <includes.h>

void Ser_CMD_handling();
uint32_t Conversion_fraction(uint8_t* Target, uint8_t* Source,uint8_t ADDR, uint32_t end_value);

#endif