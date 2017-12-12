//###########################################################################
// main.cpp
// main entry point of code, also contains interrupt handlers
//
// $Copyright: Copyright (C) LDO Systems
//###########################################################################
#include "App.h"

App app;

int main(void)
{
	System::Initialize();
	System::ConfigureForHsi();
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
		app.ServeSpi();
		app.ExtIntClear();
	}
}