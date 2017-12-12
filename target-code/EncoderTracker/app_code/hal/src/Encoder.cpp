//###########################################################################
// Encoder.cpp
// Hardware Layer class that manages a single Encoder module
//
// $Copyright: Copyright (C) LDO Systems
//###########################################################################
#include "Encoder.h"


Encoder::Encoder()
{
}


///Initializes the selected timer module as a quadrature encoder interface
void Encoder::Initialize(TimerChannel channel)
{
	base = (TIM_TypeDef*)channel;

	//enable clock
	switch (channel)
	{
	case _Tim1:
		RCC->APB2ENR = (RCC->APB2ENR & ~RCC_APB2ENR_TIM1EN_Msk) | RCC_APB2ENR_TIM1EN;
	case _Tim2:
		RCC->APB1ENR = (RCC->APB1ENR & ~RCC_APB1ENR_TIM2EN_Msk) | RCC_APB1ENR_TIM2EN;
		break;
	case _Tim3:
		RCC->APB1ENR = (RCC->APB1ENR & ~RCC_APB1ENR_TIM3EN_Msk) | RCC_APB1ENR_TIM3EN;
		break;
	case _Tim4:
		RCC->APB1ENR = (RCC->APB1ENR & ~RCC_APB1ENR_TIM4EN_Msk) | RCC_APB1ENR_TIM4EN;
		break;
	case _Tim15:
		RCC->APB2ENR = (RCC->APB2ENR & ~RCC_APB2ENR_TIM15EN_Msk) | RCC_APB2ENR_TIM15EN;
		break;
	case _Tim16:
		RCC->APB2ENR = (RCC->APB2ENR & ~RCC_APB2ENR_TIM16EN_Msk) | RCC_APB2ENR_TIM16EN;
		break;
	case _Tim17:
		RCC->APB2ENR = (RCC->APB2ENR & ~RCC_APB2ENR_TIM17EN_Msk) | RCC_APB2ENR_TIM17EN;
		break;
	default:
		break;
	}

	SetPrescaler(0);

	//Configure slave mode selection
	base->SMCR |= TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;

	//Configure compare registers to map inputs to each other
	base->CCMR1 = (base->CCMR1 & ~TIM_CCMR1_CC1S_Msk) | TIM_CCMR1_CC1S_1;
	base->CCMR1 = (base->CCMR1 & ~TIM_CCMR1_CC2S_Msk) | TIM_CCMR1_CC2S_1;
	base->CCER |= TIM_CCER_CC1P_Msk;
	base->CCER |= TIM_CCER_CC2P_Msk;

	//enable auto reloading
	base->CR1 |= TIM_CR1_ARPE_Msk;

	//configure counter for up counting
	base->CR1 &= ~TIM_CR1_DIR_Msk;

	ClearUpdateInterruptFlag();
	Enable();
	Reset();
}


///Configures Encoder Pins
void Encoder::EncoderPinInit(EncoderPinConfig pinConfig)
{
	Gpio chA;
	chA.Initialize(pinConfig.chAChannel, pinConfig.chAPin);
	chA.ConfigMode(Gpio::GpioMode::_Alt);
	chA.ConfigAltFunc(pinConfig.chAAltNum);
	chA.ConfigInputType(Gpio::GpioInputType::_NoPull);
	chA.ConfigSpeed(Gpio::GpioSpeed::_HighSpeed);

	Gpio chB;
	chB.Initialize(pinConfig.chBChannel, pinConfig.chBPin);
	chB.ConfigMode(Gpio::GpioMode::_Alt);
	chB.ConfigAltFunc(pinConfig.chBAltNum);
	chB.ConfigInputType(Gpio::GpioInputType::_NoPull);
	chB.ConfigSpeed(Gpio::GpioSpeed::_HighSpeed);
}
