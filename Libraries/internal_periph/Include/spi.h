/*
 * spi.h
 *
 *  Created on: 5 сент. 2020 г.
 *      Author: kir
 */

#ifndef USERLIB_INC_SPI_H_
#define USERLIB_INC_SPI_H_
#include "stm32f4xx_spi.h"
#include "gpio.h"


void ConfigSPI2(void);
uint8_t SPI_ReceiveByte(void);
void SPI_SendByte(uint8_t data);

#endif /* USERLIB_INC_SPI_H_ */
