//###########################################################################
// App.h
// Top level application class
//
// $Copyright: Copyright (C) LDO Systems
//###########################################################################
#ifndef __APP_H__
#define __APP_H_
#include "AppHeaders.h"
#include "Encoder.h"
#include "Nvic.h"
#include "ExtInterrupt.h"
#include "SpiSlave.h"
#include "Gpi.h"
#include "Gpo.h"


class App
{
private:
	static const uint16_t num_encoders = 4;
	static const uint8_t cmd_mask = 0x0F;
	static const uint8_t enc_offset = 4;
	static const uint8_t read_cmd = 0x0F;
	static const uint8_t reset_cmd = 0x00;

	enum EncoderIndex
	{
		_X1 = 0,
		_X2 = 1,
		_Y = 2,
		_Z = 3,
	};

	ExtInterrupt exti;
	SpiSlave spi;
	Encoder encoderList[num_encoders];
	volatile int32_t encoderVals[num_encoders];
	volatile int32_t encoderValsSendBuf[num_encoders];
	volatile uint16_t oldEncoderCounts[num_encoders];
	volatile bool isClearEncoder[enc_offset];

	void ConfigEncoder();
	void ConfigSpi();
	void ConfigExtInt();
	void SendEncoderVals(uint8_t header);
	void ClearEncoderVals(uint8_t header);
	void ReverseEndian(int32_t* num);
	void ResetSpi();

public:
	App();
	void Initialize();
	void Execute();
	void ServeSpi();

	///Clears external interrupt flag and allow interrupt to trigger once again
	inline void ExtIntClear() { exti.Clear(); }
};


#endif //__APP_H__
