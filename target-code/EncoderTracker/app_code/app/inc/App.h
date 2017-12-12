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
#include "Spi.h"
#include "Gpi.h"
#include "Gpo.h"


class App
{
private:
	void ConfigEncoder();
	void ConfigSpi();
	void ConfigExit();
	void ConfigTick();
public:
	App();

	void Initialize();
	void UpdateEncoderVals();
	void ServeSpi();
	void ExtIntClear();
};


#endif //__APP_H__
