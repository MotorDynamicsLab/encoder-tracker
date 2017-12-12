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
}


///Interprets and executes command from the SPI master
void App::ServeSpi()
{
}


///Clears external interrupt flag and allow interrupt to trigger once again
void App::ExtIntClear()
{
}

