//###########################################################################
// Gpio.cpp
// Hardware Layer class that manages a single GPIO pin
//
// $Copyright: Copyright (C) LDO Systems
//###########################################################################
#include "Gpio.h"


///Constructor
Gpio::Gpio()
	: baseReg((GPIO_TypeDef *)GPIOA_BASE),
	bitMask(0),
	pinNum(0)
{
}


///Initializes the internal members for the GPIO pin object.
///does not actually perform any configuration
void Gpio::Initialize(GpioChannel channel, uint16_t pin)
{
	baseReg = (GPIO_TypeDef *)( GPIOA_BASE + (channel * baseRegOffset) );
	pinNum = pin;
	bitMask = 0x1 << pinNum;

	//Enable the peripheral clock for the corresponding port
	uint32_t clkEnaMask = RCC_AHBENR_GPIOAEN_Msk << channel;
	RCC->AHBENR |= clkEnaMask;
}


///Sets the mode of the GPIO pin.
void Gpio::ConfigMode(GpioMode mode)
{
	baseReg->MODER = (baseReg->MODER & ~( DoubleBitMask() ) ) | (mode << (pinNum * 2) );
	int i = 0;
}


///Configures internal pull up/down resistors
void Gpio::ConfigInputType(GpioInputType inputType)
{
	baseReg->PUPDR = (baseReg->PUPDR & ~(DoubleBitMask())) | (inputType << (pinNum * 2));
}


///Configures push-pull or open-drain for output pins
void Gpio::ConfigOutputType(GpioOutType outputType)
{
	baseReg->OTYPER = (baseReg->OTYPER & ~(BitMask())) | (outputType << pinNum);
}


///Configures the output speed
void Gpio::ConfigSpeed(GpioSpeed speed)
{
	baseReg->OSPEEDR = (baseReg->OSPEEDR & ~(DoubleBitMask())) | (speed << (pinNum * 2));
}


///Sets the alternate function of the pin when pin mode is configured 
///as alternate mode. For AF0 pass in 0, for AF1 pass in 1 etc.
void Gpio::ConfigAltFunc(uint8_t altMode)
{
	if (pinNum < maxFourBitPin)
	{
		uint32_t shift = pinNum * 4;
		uint32_t mask = ~(0xF << shift);
		baseReg->AFR[0] = (baseReg->AFR[0] & mask) | altMode << shift;
	}
	else
	{
		uint32_t shift = (pinNum - maxFourBitPin) * 4;
		uint32_t mask = ~(0xF << shift);
		baseReg->AFR[1] = (baseReg->AFR[1] & mask) | altMode << shift;
	}
}


///Puts the GPIO pin in a fail safe mode (input mode).
void Gpio::FailSafe()
{
	baseReg->MODER &= ~( DoubleBitMask() );
}
