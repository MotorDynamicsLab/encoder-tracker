//###########################################################################
// Encoder.h
// Hardware Layer class that manages a single Encoder module
//
// $Copyright: Copyright (C) LDO Systems
//###########################################################################
#ifndef __ENCODER_H__
#define __ENCODER_H__
#include "BasicTimer.h"
#include "Gpio.h"

class Encoder :public BaseTimer
{
public:
	//Structures
	struct EncoderPinConfig
	{
		Gpio::GpioChannel chAChannel;
		Gpio::GpioChannel chBChannel;
		uint16_t chAPin;
		uint16_t chBPin;
		uint8_t chAAltNum;
		uint8_t chBAltNum;
	};

	//Method
	bool flag = false;
	Encoder();
	void Initialize(TimerChannel channel);
	void EncoderPinInit(EncoderPinConfig pinConfig);

	///Configures resolution (max count) of encoder
	inline void ConfigResolution(uint16_t res) { SetReloadVal(res); }

	///Resets the count to zero
	inline void Reset() { SetCounter(0); }
};


#endif // !__ENCODER_H__

