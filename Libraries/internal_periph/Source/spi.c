#include "spi.h"
/*
 * spi.c
 *
 *  Created on: 5 сент. 2020 г.
 *      Author: kir
 */


void ConfigSPI2(void){
	SPI_InitTypeDef spi;
	spi.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	spi.SPI_Mode = SPI_Mode_Master;
	spi.SPI_DataSize = SPI_DataSize_8b;
	spi.SPI_CPOL = SPI_CPOL_High;
	spi.SPI_CPHA = SPI_CPHA_2Edge;
	spi.SPI_NSS = SPI_NSS_Soft;
	spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	spi.SPI_FirstBit = SPI_FirstBit_MSB;
	spi.SPI_CRCPolynomial = 0x7;
	SPI_Init(w5500_SPI, &spi);
	SPI_Cmd(w5500_SPI, ENABLE);
	SPI_NSSInternalSoftwareConfig(w5500_SPI, SPI_NSSInternalSoft_Set);
}

uint8_t SPI_ReceiveByte(void)
{
  while (SPI_I2S_GetFlagStatus(w5500_SPI, SPI_I2S_FLAG_TXE) == RESET);
  SPI_I2S_SendData(w5500_SPI, 0xFF);
  while (SPI_I2S_GetFlagStatus(w5500_SPI, SPI_I2S_FLAG_BSY) == SET) ;
  return SPI_I2S_ReceiveData(w5500_SPI);
}

void SPI_SendByte(uint8_t data){
	while(SPI_I2S_GetFlagStatus(w5500_SPI, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(w5500_SPI, data);
	while (SPI_I2S_GetFlagStatus(w5500_SPI, SPI_I2S_FLAG_BSY) == SET) ;
	SPI_I2S_ReceiveData(w5500_SPI);
}



