#include "rcc.h"

void ConfigRcc(void){

//Config TIM RCC ************************************/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,	ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,	ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,	ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,	ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,	ENABLE);
//Config TIM RCC ------------------------------------

//Config GPIO RCC ************************************/
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
//Config GPIO RCC ------------------------------------

//Config SPI2 RCC ************************************/
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
//Config SPI2 RCC ************************************/

//Config I2C RCC *************************************/
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
//Config I2C RCC -------------------------------------

//Config CRC HASH RCC ------------------------------------
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
//Config CRC HASH RCC ------------------------------------
}

void ConfigCrystal(void){
	uint16_t i = 65535;
    // Resets the clock configuration to the default reset state
    RCC_DeInit();

//    goto M;

    // Enable external crystal (HSE)
    RCC_HSEConfig(RCC_HSE_ON);
    // Wait until HSE ready to use or not
    ErrorStatus errorStatus = RCC_WaitForHSEStartUp();
//    ErrorStatus errorStatus = SUCCESS;


    if (errorStatus == SUCCESS)
    {
    /*	// 20MHz   */
        // Configure the PLL for 84MHz SysClk and 48MHz for USB OTG, SDIO
    	RCC_PLLConfig(RCC_PLLSource_HSE, 8, 336, 4, 7);
        // Enable PLL
        RCC_PLLCmd(ENABLE);
        // Wait until main PLL clock ready
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){

        	i--;
        	if ( i==0){
        		goto M;
        	}
        }

        // Set flash latency
        FLASH_SetLatency(FLASH_Latency_6);

        // AHB 84MHz
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        // APB1 42MHz
        RCC_PCLK1Config(RCC_HCLK_Div2);
        // APB2 84 MHz
        RCC_PCLK2Config(RCC_HCLK_Div1);

        // Set SysClk using PLL
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    }
    else
    {
    	M:

    	RCC_DeInit();

    	RCC_HSEConfig(RCC_HSE_OFF);

    	RCC_PLLConfig(RCC_PLLSource_HSI, 16, 336, 4, 2);
        // Enable PLL
        RCC_PLLCmd(ENABLE);
        // Wait until main PLL clock ready
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

        // Set flash latency
        FLASH_SetLatency(FLASH_Latency_6);

        // AHB 84MHz
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        // APB1 42MHz
        RCC_PCLK1Config(RCC_HCLK_Div2);
        // APB2 84 MHz
        RCC_PCLK2Config(RCC_HCLK_Div1);

        // Set SysClk using PLL
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    }

    SystemCoreClockUpdate();
}
