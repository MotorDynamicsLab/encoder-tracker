//###########################################################################
// App.cpp
// Top level application class
//
// $Copyright: Copyright (C) LDO Systems
//###########################################################################
#include "App.h"

App::App()
{
}


///Initializes all necessary hardware
void App::Initialize()
{
	ConfigSpi();
	ConfigEncoder();
	ConfigExtInt();
}


///Configures the Timer modules to be used as encoders
void App::ConfigEncoder()
{
	const uint16_t encoder_resolution = 0xFFFF;
	Encoder::EncoderPinConfig pinConfig;

	encoderList[_X1].Initialize(Encoder::_Tim4);
	encoderList[_X2].Initialize(Encoder::_Tim1);
	encoderList[_Y].Initialize(Encoder::_Tim3);
	encoderList[_Z].Initialize(Encoder::_Tim2);

	pinConfig.chAChannel = Gpio::_ChA;
	pinConfig.chBChannel = Gpio::_ChA;
	pinConfig.chAPin = 11;
	pinConfig.chBPin = 12;
	pinConfig.chAAltNum = 10;
	pinConfig.chBAltNum = 10;
	encoderList[_X1].EncoderPinInit(pinConfig);

	pinConfig.chAChannel = Gpio::_ChA;
	pinConfig.chBChannel = Gpio::_ChA;
	pinConfig.chAPin = 8;
	pinConfig.chBPin = 9;
	pinConfig.chAAltNum = 6;
	pinConfig.chBAltNum = 6;
	encoderList[_X2].EncoderPinInit(pinConfig);

	pinConfig.chAChannel = Gpio::_ChB;
	pinConfig.chBChannel = Gpio::_ChB;
	pinConfig.chAPin = 4;
	pinConfig.chBPin = 5;
	pinConfig.chAAltNum = 2;
	pinConfig.chBAltNum = 2;
	encoderList[_Y].EncoderPinInit(pinConfig);

	pinConfig.chAChannel = Gpio::_ChA;
	pinConfig.chBChannel = Gpio::_ChA;
	pinConfig.chAPin = 0;
	pinConfig.chBPin = 1;
	pinConfig.chAAltNum = 1;
	pinConfig.chBAltNum = 1;
	encoderList[_Z].EncoderPinInit(pinConfig);

	for (int i = 0; i < num_encoders; ++i)
	{
		encoderList[i].ConfigResolution(encoder_resolution);
		encoderList[i].Enable();
		encoderList[i].Reset();
	}
}


///Configures an SPI module as slave
void App::ConfigSpi()
{
}


///Configures the chip select pin as an external interrupt
void App::ConfigExtInt()
{
}


///Reads the encoder counts from the Timer modules and converts them into 32bit signed values
///This function should be called in the main loop
void App::Execute()
{
	uint16_t encCount;

	for (int i = 0; i < num_encoders; ++i)
	{
		encCount = encoderList[i].GetCounter();
		encoderVals[i] += (int16_t)(encCount - oldEncoderCounts[i]);
		oldEncoderCounts[i] = encCount;
	}
}


///Interprets and executes command from the SPI master
void App::ServeSpi()
{
}


///Clears external interrupt flag and allow interrupt to trigger once again
void App::ExtIntClear()
{
}

