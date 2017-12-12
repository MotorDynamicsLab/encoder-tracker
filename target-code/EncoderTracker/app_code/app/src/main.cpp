//###########################################################################
// main.cpp
// main entry point of code, also contains interrupt handlers
//
// $Copyright: Copyright (C) LDO Systems
//###########################################################################
#include "App.h"


int main(void)
{
	System::Initialize();
	System::ConfigureForHsi();
	
	while (1);
}

extern "C"
{
	void EXTI4_IRQHandler()
	{
	}

	void TIM7_IRQHandler()
	{
	}
}