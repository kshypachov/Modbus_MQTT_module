#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_spi.h"
#include "stdbool.h"
#include "definition.h"

/*
 *  PB7 ---------------> I2C3_SDA (OLED)
 *  PB6 ---------------> I2C3_SCL (OLED)
 */

// OLED i2c defenitions
#define oled_I2C							I2C1
#define oled_I2C_CLK						RCC_APB1Periph_I2C1
#define oled_I2C_CLK_INIT					RCC_APB1PeriphClockCmd

#define oled_I2C_SDA_PIN					GPIO_Pin_7
#define oled_I2C_SDA_GPIO_PORT				GPIOB
#define oled_I2C_SDA_GPIO_CLK				RCC_AHB1Periph_GPIOB
#define oled_I2C_SDA_SOURCE					GPIO_PinSource7
#define oled_I2C_SDA_AF						GPIO_AF_I2C1

#define oled_I2C_SCL_PIN					GPIO_Pin_6
#define oled_I2C_SCL_GPIO_PORT				GPIOB
#define oled_I2C_SCL_GPIO_CLK				RCC_AHB1Periph_GPIOB
#define oled_I2C_SCL_SOURCE					GPIO_PinSource6
#define oled_I2C_SCL_AF						GPIO_AF_I2C1


/*
 * PD0 --------------> Coil1
 * PD1 --------------> Coil2
 * PD2 --------------> Coil3
 * PD3 --------------> Coil4
 * PD4 --------------> Coil5
 * PD5 --------------> Coil6
 * PD6 --------------> Coil7
 * PD7 --------------> Coil8
 *
 * PD8 --------------> Input1
 * PD9 --------------> Input2
 * PD10 -------------> Input3
 * PD11 -------------> Input4
 * PD12 -------------> Input5
 * PD13 -------------> Input6
 * PD14 -------------> Input7
 * PD15 -------------> Input8
 */
//Coils port definition
#define coils_GPIO_PORT						GPIOA
#define coils_GPIO_CLK						RCC_AHB1Periph_GPIOA

#define coil1_PIN							GPIO_Pin_5
#define coil2_PIN							GPIO_Pin_6
#define coil3_PIN							GPIO_Pin_7
/*
#define coil3_PIN							GPIO_Pin_2
#define coil4_PIN							GPIO_Pin_3
#define coil5_PIN							GPIO_Pin_4
#define coil6_PIN							GPIO_Pin_5
#define coil7_PIN							GPIO_Pin_6
#define coil8_PIN							GPIO_Pin_7
*/
#define inputs_GPIO_PORT					GPIOA
#define inputs_GPIO_CLK						RCC_AHB1Periph_GPIOA

#define input1_PIN							GPIO_Pin_0
#define input2_PIN							GPIO_Pin_1
#define input3_PIN							GPIO_Pin_2
/*
#define input4_PIN							GPIO_Pin_11
#define input5_PIN							GPIO_Pin_12
#define input6_PIN							GPIO_Pin_13
#define input7_PIN							GPIO_Pin_14
#define input8_PIN							GPIO_Pin_15
*/
//------------------------W5500 SPI Interface pins--------------------------------//
//SPI2,SPI3 - APB1 42 MHz (max)
#define w5500_SPI                           SPI2
#define w5500_SPI_CLK                       RCC_APB1Periph_SPI2
#define w5500_SPI_CLK_INIT                  RCC_APB1PeriphClockCmd

#define w5500_SPI_SCK_PIN                   GPIO_Pin_13
#define w5500_SPI_SCK_GPIO_PORT             GPIOB
#define w5500_SPI_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOB
#define w5500_SPI_SCK_SOURCE                GPIO_PinSource13
#define w5500_SPI_SCK_AF                    GPIO_AF_SPI2

#define w5500_SPI_MISO_PIN                  GPIO_Pin_14
#define w5500_SPI_MISO_GPIO_PORT            GPIOB
#define w5500_SPI_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define w5500_SPI_MISO_SOURCE               GPIO_PinSource14
#define w5500_SPI_MISO_AF                   GPIO_AF_SPI2

#define w5500_SPI_MOSI_PIN                  GPIO_Pin_15
#define w5500_SPI_MOSI_GPIO_PORT            GPIOB
#define w5500_SPI_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define w5500_SPI_MOSI_SOURCE               GPIO_PinSource15
#define w5500_SPI_MOSI_AF                   GPIO_AF_SPI2

#define w5500_CS_PIN                        GPIO_Pin_12
#define w5500_CS_GPIO_PORT                  GPIOB
#define w5500_CS_GPIO_CLK                   RCC_AHB1Periph_GPIOB

#define w5500_RST_PIN						GPIO_Pin_8
#define w5500_RST_GPIO_PORT                 GPIOA
#define w5500_RST_GPIO_CLK                  RCC_AHB1Periph_GPIOA
//------------------------W5500 SPI Interface pins--------------------------------//
//----------------------------W25Q FLASH INIT-------------------------------------//

#define w25q_SPI                           	w5500_SPI
#define w25q_SPI_CLK                       	w5500_SPI_CLK
#define w25q_SPI_CLK_INIT                  	w5500_SPI_CLK_INIT

#define w25q_SPI_SCK_PIN					w5500_SPI_SCK_PIN
#define w25q_SPI_SCK_GPIO_PORT				w5500_SPI_SCK_GPIO_PORT
#define w25q_SPI_SCK_GPIO_CLK				w5500_SPI_SCK_GPIO_CLK
#define w25q_SPI_SCK_SOURCE					w5500_SPI_SCK_SOURCE
#define w25q_SPI_SCK_AF						w5500_SPI_SCK_AF

#define w25q_SPI_MOSI_PIN					w5500_SPI_MOSI_PIN
#define w25q_SPI_MOSI_GPIO_PORT				w5500_SPI_MOSI_GPIO_PORT
#define w25q_SPI_MOSI_GPIO_CLK				w5500_SPI_MOSI_GPIO_CLK
#define w25q_SPI_MOSI_SOURCE				w5500_SPI_MOSI_SOURCE
#define w25q_SPI_MOSI_AF					w5500_SPI_MOSI_AF

#define w25q_SPI_MISO_PIN					w5500_SPI_MISO_PIN
#define w25q_SPI_MISO_GPIO_PORT				w5500_SPI_MISO_GPIO_PORT
#define w25q_SPI_MISO_GPIO_CLK				w5500_SPI_MISO_GPIO_CLK
#define w25q_SPI_MISO_SOURCE				w5500_SPI_MISO_SOURCE
#define w25q_SPI_MISO_AF					w5500_SPI_MISO_AF

#define w25q_CS_PIN							GPIO_Pin_9
#define w25q_CS_GPIO_PORT                  	GPIOA
#define w25q_CS_GPIO_CLK                   	RCC_AHB1Periph_GPIOA
//----------------------------W25Q FLASH INIT-------------------------------------//


void ConfigGPIO(void);
void EthernetChipResetOn(void);
void EthernetChipResetOff(void);
void EthernetChipSelect(void);
void EthernetChipDeselect(void);
void FlashChipSelect(void);
void FlashChipDeselect(void);
#ifdef SPI_THREADSAFE
	void RegSPIMutexCallbackFunction(void (* FuncToTake), void (* FuncToRelease));
#endif

