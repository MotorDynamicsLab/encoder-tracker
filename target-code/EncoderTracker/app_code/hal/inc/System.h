//###########################################################################
// System.h
// A mostly static class containing system wide functions.
//
// $Copyright: Copyright (C) LDO Systems
//###########################################################################
#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "HalHeaders.h"

//---------------------------------------------------------------------------
// Class Definition
//
class System
{
private:
	static const uint32_t microsInSec = 1000000;
	static const uint32_t microsInMilli = 1000;
	static uint32_t cyclesInMicro;

public:
	static void Initialize();
	static void ConfigureForXtal();
	static void ConfigureForHsi();
	static void DelayUs(uint32_t micros);
	static void DelayMs(uint32_t millis);
	static inline uint32_t GetSysClkCount() { return DWT->CYCCNT; }
};


#endif /* __SYSTEM_H__ */