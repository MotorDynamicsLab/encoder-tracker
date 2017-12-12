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


///Configure the timer channel
void Encoder::Initialize(TimerChannel channel)
{
	base = (TIM_TypeDef*)channel;
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
}


//Config Encoder Pin
void Encoder::EncoderPinInit(Gpio::GpioChannel ch1, uint8_t pin1, uint8_t afNum1, Gpio::GpioChannel ch2, uint8_t pin2, uint8_t afNum2)
{
	Gpio pinEncoder1;
	pinEncoder1.Initialize(ch1, pin1);
	pinEncoder1.ConfigMode(Gpio::GpioMode::_Alt);
	pinEncoder1.ConfigAltFunc(afNum1);
	pinEncoder1.ConfigInputType(Gpio::GpioInputType::_NoPull);
	pinEncoder1.ConfigSpeed(Gpio::GpioSpeed::_HighSpeed);

	Gpio pinEncoder2;
	pinEncoder2.Initialize(ch2, pin2);
	pinEncoder2.ConfigMode(Gpio::GpioMode::_Alt);
	pinEncoder2.ConfigAltFunc(afNum2);
	pinEncoder2.ConfigInputType(Gpio::GpioInputType::_NoPull);
	pinEncoder2.ConfigSpeed(Gpio::GpioSpeed::_HighSpeed);
}


///Config Slave mode selection
void Encoder::ConfigSlaveModeSelection(SlaveModeSelection slaveModeSelection)
{
	base->SMCR = (base->SMCR & ~TIM_SMCR_SMS_Msk) | (slaveModeSelection << TIM_SMCR_SMS_Pos);
}


///Capture/Compare 1 selection
void Encoder::ConfigCc1Selection(CcSelection cc1Selection)
{
	base->CCMR1 = (base->CCMR1 & ~TIM_CCMR1_CC1S_Msk) | (cc1Selection << TIM_CCMR1_CC1S_Pos);
}


///Capture/Compare 2 selection
void Encoder::ConfigCc2Selection(CcSelection cc2Selection)
{
	base->CCMR1 = (base->CCMR1 & ~TIM_CCMR1_CC2S_Msk) | (cc2Selection << TIM_CCMR1_CC2S_Pos);
}


///Capture/Compare 3 selection
void Encoder::ConfigCc3Selection(CcSelection cc3Selection)
{
	base->CCMR2 = (base->CCMR2 & ~TIM_CCMR2_CC3S_Msk) | (cc3Selection << TIM_CCMR2_CC3S_Pos);
}


///Capture/Compare 4 selection
void Encoder::ConfigCC4_Selection(CcSelection cc4Selection)
{
	base->CCMR2 = (base->CCMR2 & ~TIM_CCMR2_CC4S_Msk) | (cc4Selection << TIM_CCMR2_CC4S_Pos);
}


///CCx x can be 1 2 3 4 5 6,
///CCx channel configured as output :
///State::Clear : OC1 active high
///State::Set   : OC1 active low
///1: OC1 active low
///CC1 channel configured as input : CC1NP / CC1P bits select TI1FP1 and TI2FP1 polarity
///for trigger or capture operations.
///00 : noninverted / rising edge
///Circuit is sensitive to TIxFP1 rising edge(capture, trigger in reset, external clock or trigger
///	mode), TIxFP1 is not inverted(trigger in gated mode, encoder mode).
///	01 : inverted / falling edge
///	Circuit is sensitive to TIxFP1 falling edge(capture, trigger in reset, external clock or trigger
///		mode), TIxFP1 is inverted(trigger in gated mode, encoder mode).
///	10 : reserved, do not use this configuration.
///	11 : noninverted / both edges
///	Circuit is sensitive to both TIxFP1 rising and falling edges(capture, trigger in reset, external
///		clock or trigger mode), TIxFP1 is not inverted(trigger in gated mode).This configuration
///	must not be used for encoder mode.
void Encoder::ConfigCcxP(Ccx ccx, bool state)
{
	switch (ccx)
	{
	case _Cc1:
		base->CCER = (base->CCER & ~TIM_CCER_CC1P_Msk) | (state << TIM_CCER_CC1P_Pos);
		break;
	case _Cc2:
		base->CCER = (base->CCER & ~TIM_CCER_CC2P_Msk) | (state << TIM_CCER_CC2P_Pos);
		break;
	case _Cc3:
		base->CCER = (base->CCER & ~TIM_CCER_CC3P_Msk) | (state << TIM_CCER_CC3P_Pos);
		break;
	case _Cc4:
		base->CCER = (base->CCER & ~TIM_CCER_CC4P_Msk) | (state << TIM_CCER_CC4P_Pos);
		break;
	case _Cc5:
		base->CCER = (base->CCER & ~TIM_CCER_CC5P_Msk) | (state << TIM_CCER_CC5P_Pos);
		break;
	case _Cc6:
		base->CCER = (base->CCER & ~TIM_CCER_CC6P_Msk) | (state << TIM_CCER_CC6P_Pos);
		break;
	default:
		break;
	}
}


///enable or disable Auto-reload
void Encoder::ConfigAutoReload(bool isAutoReload)
{
	base->CR1 = (base->CR1 & ~TIM_CR1_ARPE_Msk) | isAutoReload << TIM_CR1_ARPE_Pos;
}


void Encoder::ConfigCounterDirection(CounterDirection dir)
{
	base->CR1 = (base->CR1 & ~TIM_CR1_DIR_Msk) | (dir << TIM_CR1_DIR_Pos);
}
