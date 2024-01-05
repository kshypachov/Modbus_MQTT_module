#include "gpio.h"

bool RelaySt = 0;

void (* SPI2MutexGet)		(void);
void (* SPI2MutexRelease)	(void);

void ConfigGPIO(void){

	GPIO_InitTypeDef    GPIO_InitStructure;

	/*!< Connect SPI pins */
	GPIO_PinAFConfig(w5500_SPI_SCK_GPIO_PORT, w5500_SPI_SCK_SOURCE, w5500_SPI_SCK_AF);
	GPIO_PinAFConfig(w5500_SPI_MISO_GPIO_PORT, w5500_SPI_MISO_SOURCE, w5500_SPI_MISO_AF);
	GPIO_PinAFConfig(w5500_SPI_MOSI_GPIO_PORT, w5500_SPI_MOSI_SOURCE, w5500_SPI_MOSI_AF);


	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;

	//!< SPI SCK pin configuration /
	GPIO_InitStructure.GPIO_Pin = w5500_SPI_SCK_PIN;
	GPIO_Init(w5500_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

	//!< SPI MOSI pin configuration /
	GPIO_InitStructure.GPIO_Pin =  w5500_SPI_MOSI_PIN;
	GPIO_Init(w5500_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

	//!< SPI MISO pin configuration /
	GPIO_InitStructure.GPIO_Pin =  w5500_SPI_MISO_PIN;
	GPIO_Init(w5500_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	//!< Configure w5500 Card CS pin in output pushpull mode /
	GPIO_InitStructure.GPIO_Pin = w5500_CS_PIN;
	GPIO_Init(w5500_CS_GPIO_PORT, &GPIO_InitStructure);

	//!< Configure w5500 Reset pin configuration /
	GPIO_InitStructure.GPIO_Pin =  w5500_RST_PIN;
	GPIO_Init(w5500_RST_GPIO_PORT, &GPIO_InitStructure);

	//!< Configure w25q CS pin in output pushpull mode /
	GPIO_InitStructure.GPIO_Pin = w25q_CS_PIN;
	GPIO_Init(w25q_CS_GPIO_PORT, &GPIO_InitStructure);

	//I2C GPIO
	GPIO_InitStructure.GPIO_Pin = oled_I2C_SDA_PIN | oled_I2C_SCL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, oled_I2C_SCL_SOURCE, GPIO_AF_I2C1);
	GPIO_PinAFConfig(GPIOB, oled_I2C_SDA_SOURCE, GPIO_AF_I2C1);

	/* Coils PINs*/
	GPIO_InitStructure.GPIO_Pin = coil1_PIN | coil2_PIN /*| coil3_PIN | coil4_PIN | coil5_PIN | coil6_PIN | coil7_PIN | coil8_PIN*/;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(coils_GPIO_PORT, &GPIO_InitStructure);

	//Inputs PINs
	GPIO_InitStructure.GPIO_Pin = input1_PIN | input2_PIN | input3_PIN /*| input4_PIN | input5_PIN | input6_PIN | input7_PIN | input8_PIN*/;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(inputs_GPIO_PORT, &GPIO_InitStructure);

	//Set chip w5500 and flash in inactive state
	GPIO_SetBits(w5500_CS_GPIO_PORT, w5500_CS_PIN);
	GPIO_SetBits(w25q_CS_GPIO_PORT, w25q_CS_PIN);
}

void EthernetChipResetOn(void){
	GPIO_ResetBits(w5500_RST_GPIO_PORT, w5500_RST_PIN);
}

void EthernetChipResetOff(void){
	GPIO_SetBits(w5500_RST_GPIO_PORT, w5500_RST_PIN);
}

void EthernetChipSelect(void){
	SPI2MutexGet();
	GPIO_ResetBits(w5500_CS_GPIO_PORT, w5500_CS_PIN);
}

void EthernetChipDeselect(void){
	SPI2MutexRelease();
	GPIO_SetBits(w5500_CS_GPIO_PORT, w5500_CS_PIN);
}

void FlashChipSelect(void){
	/* Select sFLASH: Chip Select pin low */
	SPI2MutexGet();
	GPIO_ResetBits(w25q_CS_GPIO_PORT, w25q_CS_PIN);
}

void FlashChipDeselect(void){
	/* Deselect sFLASH: Chip Select pin high */
	SPI2MutexRelease();
	GPIO_SetBits(w25q_CS_GPIO_PORT, w25q_CS_PIN);
}

#ifdef SPI_THREADSAFE
	void RegSPIMutexCallbackFunction(void (* FuncToTake), void (* FuncToRelease)){
		if (FuncToTake) 	SPI2MutexGet 		= FuncToTake;
		if (FuncToRelease)	SPI2MutexRelease 	= FuncToRelease;
	}
#endif
