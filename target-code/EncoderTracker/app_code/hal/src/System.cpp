//###########################################################################
// System.h
// A mostly static class containing system wide functions.
//
// $Copyright: Copyright (C) LDO Systems
//###########################################################################
#include "System.h"

uint32_t System::cyclesInMicro = 72;


///Configure the SysTick timer
void System::Initialize(void)
{
	//Enable flash prefetch buffer
	FLASH->ACR |= FLASH_ACR_PRFTBE;

	//Enable Data Watchpoint and Trace unit and turn on its main counter
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

	//Select Systick clock source: SYSCLK_MHZ/8 = 72/8 = 16M
	SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk; 

	//Calculate the cyclesInMicro variable
	cyclesInMicro = SystemCoreClock / microsInSec;
}


///Configures the system to use an external crystal as clock source.
///The settings are fixed so it is advised to rewrite this function
///If a different crystal or frequency or multiplcation factor or
///prescaler is used.
void System::ConfigureForXtal()
{
	//First increase the flash wait states to 2 due to high system clock speed
	FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY) | FLASH_ACR_LATENCY_2;

	//The following modifies the SYSCLK to use PLL = HSEx9 as clock source
	//Given an external crystal of 8Mhz this will yield a SYSCLK = 72Mhz

	//Modify the PLL settings
	RCC->CR &= ~RCC_CR_PLLON_Msk; //Turn PLL off
	RCC->CR |= RCC_CR_HSEON;
	RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PLLMUL_Msk) | RCC_CFGR_PLLMUL9; //PLL = x9
	RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_PLLSRC_Msk) | RCC_CFGR_PLLSRC_HSE_PREDIV; //PLL source = HSE
	RCC->CR |= RCC_CR_PLLON_Msk; //Turn PLL on

	//Wait for PLL to lock
	while ( !(RCC->CR & RCC_CR_PLLRDY) ) {}

	//Switch SYSCLK source to the PLL
	RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW_Msk) | RCC_CFGR_SW_PLL;

	//Wait for the switch to complete
	while ( !(RCC->CFGR & RCC_CFGR_SWS_PLL) ) {}

	//Update the SystemCoreClock variable
	SystemCoreClockUpdate();

	//Update the cyclesInMicro variable
	cyclesInMicro = SystemCoreClock / microsInSec;
}


///Configures the system to use an hsi crystal as clock source.
void System::ConfigureForHsi()
{
	//Increase the flash wait states to 2 due to high system clock speed
	FLASH->ACR = (FLASH->ACR & ~FLASH_ACR_LATENCY) | FLASH_ACR_LATENCY_2;

	//If chips is STM32f303RE given an HSI crystal of 8Mhz this will yield a SYSCLK = 72Mhz
	//If chips is STM32f303CB given an HSI crystal of 4Mhz this will yield a SYSCLK = 64Mhz

	//Turn PLL off
	RCC->CR &= ~RCC_CR_PLLON_Msk;

	//Configures the AHB clock (HCLK)
	RCC->CFGR &= ~RCC_CFGR_HPRE_Msk;
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

	//Configures the High Speed APB2 clock (PCLK2)
	RCC->CFGR &= ~RCC_CFGR_PPRE2_Msk;
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

	//Configures the High Speed APB1 clock (PCLK1)
	RCC->CFGR &= ~RCC_CFGR_PPRE1_Msk;
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;

	//Turn on HSI 
	RCC->CR |= RCC_CR_HSION;
	while ((RCC->CR & RCC_CR_HSIRDY_Msk) != RCC_CR_HSIRDY);

	//Modify the PLL settings
	//PLL entry clock source 
	RCC->CFGR &= ~RCC_CFGR_PLLSRC;

#ifdef STM32F303xE
	//PLL input clock*9
	RCC->CFGR |= (uint32_t)RCC_CFGR_PLLMUL9;
#endif // STM32F303xE

#ifdef STM32F303xC
	//PLL input clock*16
	RCC->CFGR |= (uint32_t)RCC_CFGR_PLLMUL16;
#endif // STM32F303xC

	//Turn PLL on
	RCC->CR |= RCC_CR_PLLON_Msk;
	//Wait for PLL to lock
	while (!(RCC->CR & RCC_CR_PLLRDY));

	//Clear SW[1:0] bits
	RCC->CFGR &= ~RCC_CFGR_SW_Msk;
	//Switch SYSCLK source to the PLL
	RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW_Msk) | RCC_CFGR_SW_PLL;
	//Wait for the switch to complete
	while (!(RCC->CFGR & RCC_CFGR_SWS_PLL));

	//Update the SystemCoreClock variable
	SystemCoreClockUpdate();

	//Update the cyclesInMicro variable
	cyclesInMicro = SystemCoreClock / microsInSec;
}


///Delays for a specified number of microseconds.
///Delay range: 0-798915 us
void System::DelayUs(uint32_t micros)
{
	uint32_t delayStart = DWT->CYCCNT;
	uint32_t delayCycles = (micros * cyclesInMicro);
	while ((DWT->CYCCNT - delayStart) < delayCycles);
}


///Delays for a specified number of milliseconds.
///Delay range: 0-798 ms
void System::DelayMs(uint32_t millis)
{
	DelayUs(1000 * millis);
}