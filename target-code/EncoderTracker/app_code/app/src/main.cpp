//###########################################################################
// main.cpp
// main entry point of code, also contains interrupt handlers
//
// $Copyright: Copyright (C) LDO Systems
//###########################################################################
#include "App.h"

App app;
Gpo intPin;

int main(void)
{
	System::Initialize();
	System::ConfigureForHsi();
	intPin.Initialize(Gpio::_ChB, 12, Gpio::_High);
	intPin.Set();
	app.Initialize();

	while (1)
	{
		app.Execute();
	}
}

extern "C"
{
	void EXTI4_IRQHandler()
	{
		intPin.Clear();
		app.ServeSpi();
		app.ExtIntClear();
		intPin.Set();
	}
}