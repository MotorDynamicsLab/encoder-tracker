//###########################################################################
// ExtInterrupt.h
// Hardware Layer class that manages external interrupts
//
// $Copyright: Copyright (C) LDO Systems
//###########################################################################
#ifndef __EXTINTERRUP_H__
#define __EXTINTERRUP_H__
#include "HalHeaders.h"
#include "Gpio.h"

class ExtInterrupt
{
public:
	//Enumerations
	enum ExtIntMode
	{
		_Interrupt = 0,
		_Event = 1,
	};

	enum ExtIntEdge
	{
		_Rising = 0,
		_Falling = 1,
		_RisingAndFalling = 2,
	};

private:
	uint8_t extIntLine;
	uint32_t bitMask;
	volatile uint32_t* pendingReg;

public:
	//Methods
	ExtInterrupt();
	void Initialize(uint8_t line);
	void ConfigMode(ExtIntMode mode);
	void ConfigTriggerEdge(ExtIntEdge trigger);
	void ConfigExtPin(Gpio::GpioChannel ch, uint8_t pinNum);
	void SoftInt();

	///Checks whether an external interrupt event has occured
	inline bool Check() { return (bool)*pendingReg & bitMask; };

	///Clears the external interrupt event flag
	inline void Clear() { *pendingReg |= bitMask; };
};

#endif // !__EXTINTERRUP_H__
