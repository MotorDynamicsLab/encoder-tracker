//###########################################################################
// ExtInterrupt.cpp
// Hardware Layer class that manages external interrupts
//
// $Copyright: Copyright (C) LDO Systems
//###########################################################################
#include "ExtInterrupt.h"

///Constructor
ExtInterrupt::ExtInterrupt()
	: pendingReg(__null),
	bitMask(0)
{
}


///Selects the interrupt line
void ExtInterrupt::Initialize(uint8_t line)
{
	extIntLine = line;
	if (31 >= line)
	{
		bitMask = 1 << extIntLine;
		pendingReg = &(EXTI->PR);
	}
	else
	{
		bitMask = extIntLine - 32;
		pendingReg = &(EXTI->PR2);
	}
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
}


///Select between event or interrupt mode
void ExtInterrupt::ConfigMode(ExtIntMode mode)
{
	if (31 >= extIntLine)
	{
		if (_Interrupt == mode)
		{
			EXTI->IMR |= bitMask;
			EXTI->EMR &= ~bitMask;
		}
		else
		{
			EXTI->IMR &= ~bitMask;
			EXTI->EMR |= bitMask;
		}
	}
	else
	{
		if (_Interrupt == mode)
		{
			EXTI->IMR2 |= bitMask;
			EXTI->EMR2 &= ~bitMask;
		}
		else
		{
			EXTI->IMR2 &= ~bitMask;
			EXTI->EMR2 |= bitMask;
		}
	}
}


///Selects external interrupt trigger edge
void ExtInterrupt::ConfigTriggerEdge(ExtIntEdge trigger)
{
	if (31 >= extIntLine)
	{
		if (_Rising == trigger)
		{
			EXTI->RTSR |= bitMask;
			EXTI->FTSR &= bitMask;
		}
		else if (_Falling == trigger)
		{
			EXTI->RTSR &= ~bitMask;
			EXTI->FTSR |= bitMask;
		}
		else
		{
			EXTI->RTSR |= bitMask;
			EXTI->FTSR |= bitMask;
		}
	}
	else
	{
		if (_Rising == trigger)
		{
			EXTI->RTSR2 |= bitMask;
			EXTI->FTSR &= bitMask;
		}
		else if (_Falling == trigger)
		{
			EXTI->RTSR2 &= ~bitMask;
			EXTI->FTSR2 |= bitMask;
		}
		else
		{
			EXTI->RTSR2 |= bitMask;
			EXTI->FTSR2 |= bitMask;
		}
	}
}


///Software Generated Interrupt
///This bit is cleared by clearing the corresponding bit in the EXTI_PR register (by
///writing a ¡®1¡¯ into the bit).
void ExtInterrupt::SoftInt()
{
	if (31 >= extIntLine)
	{
		EXTI->SWIER |= bitMask;
	}
	else
	{
		EXTI->SWIER2 |= bitMask;
	}
}


///Configures the Gpi pin that triggers the interrupt
void ExtInterrupt::ConfigExtPin(Gpio::GpioChannel ch, uint8_t pinNum)
{
	int nTemp = 0;
	int nGroup = 0;

	if (3 >= pinNum)
	{
		nTemp = pinNum;
		nGroup = 0;
	}
	else if (7 >= pinNum)
	{
		nTemp = pinNum - 4;
		nGroup = 1;
	}
	else if (11 >= pinNum)
	{
		nTemp = pinNum - 8;
		nGroup = 2;
	}
	else
	{
		nTemp = pinNum - 12;
		nGroup = 3;
	}

	SYSCFG->EXTICR[nGroup] = (SYSCFG->EXTICR[nGroup] & ~(0xf << (nTemp * 4))) | (ch << (nTemp * 4));
}
