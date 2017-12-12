//###########################################################################
// Gpi.h
// Hardware layer that manages a single GPIO input pin
//
// $Copyright: Copyright (C) LDO Systems
//###########################################################################
#include "Gpi.h"


///Constructor
Gpi::Gpi()
{
}


///Initializes the GPIO pin, does not actually perform any configuration
void Gpi::Initialize(GpioChannel channel, uint16_t pin)
{
	//Call the parent initialize first
	Gpio::Initialize(channel, pin);

	dataReg = &(baseReg->IDR);

	//Set the correct mode
	ConfigMode(_Input);
}


///Configures the internal pull resistor (pull-up, pull-down, none)
void Gpi::ConfigPullUpDown(GpioInputType pullMode)
{
	baseReg->PUPDR = (baseReg->PUPDR & ~(DoubleBitMask())) | (pullMode << (pinNum * 2));
}