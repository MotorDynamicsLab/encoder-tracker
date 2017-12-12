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
	//Enumerations
	enum SlaveModeSelection
	{
		_Sms0 = 0x0,        //Slave mode disabled - if CEN = бо1 then the prescaler is clocked directly by the internal
					        //clock.
		_Sms1 = 0x1,        //Encoder mode 1 - Counter counts up / down on TI1FP1 edge depending on TI2FP2
					        //level.
		_Sms2 = 0x2,        //Encoder mode 2 - Counter counts up / down on TI2FP2 edge depending on TI1FP1
					        //level.
		_Sms3 = 0x3,        //Encoder mode 3 - Counter counts up / down on both TI1FP1 and TI2FP2 edges
					        //depending on the level of the other input.
		_Sms4 = 0x4,        //Reset Mode - Rising edge of the selected trigger input(TRGI) reinitializes the counter
					        //and generates an update of the registers.
		_Sms5 = 0x5,        //Gated Mode - The counter clock is enabled when the trigger input(TRGI) is high.The
					        //counter stops(but is not reset) as soon as the trigger becomes low.Both start and stop of
					        //the counter are controlled.
		_Sms6 = 0x6,        //Trigger Mode - The counter starts at a rising edge of the trigger TRGI(but it is not
					        //reset).Only the start of the counter is controlled.
		_Sms7 = 0x7,        //External Clock Mode 1 - Rising edges of the selected trigger(TRGI) clock the counter.
		_Sms8 = 0x10000,    // Combined reset + trigger mode - Rising edge of the selected trigger input(TRGI)
						    //reinitializes the counter, generates an update of the registers and starts the counter.
	};

	//_CCxS_00 : CC4 channel is configured as output
	//_CCxS_01 : CC4 channel is configured as input, IC4 is mapped on TI4
	//_CCxS_10 : CC4 channel is configured as input, IC4 is mapped on TI3
	//_CCxS_11 : CC4 channel is configured as input, IC4 is mapped on TRC.This mode is working only if
	//			 an internal trigger input is selected through TS bit(TIMx_SMCR register)
	//For CC3S
	//_CCxS_00 : CC3 channel is configured as output
	//_CCxS_01 : CC3 channel is configured as input, IC3 is mapped on TI3
	//_CCxS_10 : CC3 channel is configured as input, IC3 is mapped on TI4
	//_CCxS_11 : CC3 channel is configured as input, IC3 is mapped on TRC.This mode is working only if
	//			 an internal trigger input is selected through TS bit(TIMx_SMCR register)
	//For CC2S
	//_CCxS_00 : CC2 channel is configured as output.
	//_CCxS_01 : CC2 channel is configured as input, IC2 is mapped on TI2.
	//_CCxS_10 : CC2 channel is configured as input, IC2 is mapped on TI1.
	//_CCxS_11 : CC2 channel is configured as input, IC2 is mapped on TRC.This mode is working only if
	//			 an internal trigger input is selected through the TS bit(TIMx_SMCR register)
	//For CC1S
	//_CCxS_00 : CC1 channel is configured as output.
	//_CCxS_01 : CC1 channel is configured as input, IC1 is mapped on TI1.
	//_CCxS_10 : CC1 channel is configured as input, IC1 is mapped on TI2.
	//_CCxS_11 : CC1 channel is configured as input, IC1 is mapped on TRC.This mode is working only if
	//			 an internal trigger input is selected through TS bit(TIMx_SMCR register)
	enum CcSelection
	{
		_Ccxs00 = 0b00,
		_Ccxs01 = 0b01,
		_Ccxs10 = 0b10,
		_Ccxs11 = 0b11,
	};

	enum Ccx
	{
		_Cc1 = 1,
		_Cc2 = 2,
		_Cc3 = 3,
		_Cc4 = 4,
		_Cc5 = 5,
		_Cc6 = 6,
	};

	enum CounterDirection
	{
		_UpCounter = 0,
		_DownCounter = 1,
	};

	//Method
	bool flag = false;
	Encoder();
	void Initialize(TimerChannel channel);
	void EncoderPinInit(Gpio::GpioChannel ch1,uint8_t pin1,uint8_t afNum1, Gpio::GpioChannel ch2, uint8_t pin2, uint8_t afNum2);
	void ConfigSlaveModeSelection(SlaveModeSelection slaveModeSelection);
	void ConfigCc1Selection(CcSelection cc1Selection);
	void ConfigCc2Selection(CcSelection cc2Selection);
	void ConfigCc3Selection(CcSelection cc3Selection);
	void ConfigCC4_Selection(CcSelection cc4Selection);
	void ConfigCcxP(Ccx ccx, bool state);
	void ConfigAutoReload(bool isAutoReload);
	void ConfigCounterDirection(CounterDirection dir);
};


#endif // !__ENCODER_H__

