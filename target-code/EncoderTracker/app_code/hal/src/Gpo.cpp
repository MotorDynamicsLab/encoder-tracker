//###########################################################################
// Gpo.cpp
// Hardware layer that manages a single GPIO output pin
//
// $Copyright: Copyright (C) LDO Systems
//###########################################################################
#include "Gpo.h"


///Constructor
Gpo::Gpo()
{
}


///Initializes the GPIO pin, does not actually perform any configuration
void Gpo::Initialize(GpioChannel channel, uint16_t pin, GpioState initState = _Low)
{
	//Call the parent initialize first
	Gpio::Initialize(channel, pin);

	setReg = &(baseReg->BSRR);
	clearReg = &(baseReg->BRR);

	//Set the initial value
	//Set the correct mode
	Write(initState);
	ConfigMode(_Ouput);
}


///Configures the output type (push-pull or open drain)
void Gpo::ConfigType(GpioOutType outType)
{
	baseReg->OTYPER = (baseReg->OTYPER & ~bitMask) | (outType << pinNum);
}


///Writes a specified state into the pin
void Gpo::Write(GpioState state)
{
	if (_Low == state) Clear();
	else Set();
}