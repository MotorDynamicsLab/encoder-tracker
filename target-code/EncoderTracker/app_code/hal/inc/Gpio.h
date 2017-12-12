//###########################################################################
// Gpio.h
// Hardware Layer class that manages a single GPIO pin
//
// $Copyright: Copyright (C) LDO Systems
//###########################################################################
#ifndef __GPIO_H__
#define __GPIO_H__

#include "HalHeaders.h"

//---------------------------------------------------------------------------
// Class Definition
//
class Gpio
{
protected:
	//Static Constants
	static const uint32_t baseRegOffset = 0x400;
	static const uint16_t maxFourBitPin = 8;

	//Members
	GPIO_TypeDef* baseReg;
	uint32_t bitMask;
	uint16_t pinNum;

public:
	//Enumerations
	enum GpioChannel
	{
		_ChA = 0,
		_ChB = 1,
		_ChC = 2,
		_ChD = 3,
		_ChE = 4,
		_ChF = 5,
		_ChG = 6,
	};

	enum GpioMode
	{
		_Input  = 0b00,
		_Ouput  = 0b01,
		_Alt    = 0b10,
		_Analog = 0b11,
	};

	enum GpioState
	{
		_Low = 0,
		_High = 1,
	};

	enum GpioOutType
	{
		_PushPull = 0,
		_OpenDrain = 1,
	};

	enum GpioInputType
	{
		_NoPull   = 0b00,
		_PullUp   = 0b01,
		_PullDown = 0b10,
	};

	enum GpioSpeed
	{
		_LowSpeed  = 0b00,
		_MedSpeed  = 0b01,
		_HighSpeed = 0b11,
	};

	//Methods
	Gpio();
	void Initialize(GpioChannel channel, uint16_t pin);
	void ConfigMode(GpioMode mode);
	void ConfigInputType(GpioInputType inputType);
	void ConfigOutputType(GpioOutType outputType);
	void ConfigAltFunc(uint8_t altMode);
	void ConfigSpeed(GpioSpeed speed);
	void FailSafe();

	///Returns the mask used for register manipulation
	inline uint32_t BitMask() { return bitMask; }

	///Returns the mask used for double bit register manipulation
	inline uint32_t DoubleBitMask() { return 0b11 << (pinNum * 2); }
};

#endif /* __GPIO_H__ */
