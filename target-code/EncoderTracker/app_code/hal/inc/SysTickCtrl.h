//###########################################################################
// SysTickCtrl.h
// Controls SysTick related settings and functions.
// SysTick is a 24 bit countdown timer used by the system for various 
// timing purposes. Documentation can be found at the following address:
// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0497a/Bhccjgga.html
//
// $Copyright: Copyright (C) LDO Systems
//###########################################################################
#ifndef __SYSTICK_CTRL_H__
#define __SYSTICK_CTRL_H__

#include "AppHeaders.h"

//---------------------------------------------------------------------------
// Class Definition
//
class SysTickCtrl
{
public:
	enum ClkSrc
	{
		ClkSrc_Ext = 0,
		ClkSrc_Proc = 1 //SYSCLK_MHZ/8
	};

	///Enables or disables the SysTick timer interrupt
	static void ConfigSysTickInt(bool enable)
	{
		if (enable) SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk ;
		else SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
	}

	///Sets the clock source of the SysTick timer to external source
	static void SetClkSource(ClkSrc src)
	{
		if (ClkSrc_Ext == src) SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk;
		else SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	}

	///Enables the SysTick timer
	static inline void EnableSysTick() { SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; }

	///Disables the SysTick timer
	static inline void DisableSysTick() { SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; }

	///Sets the reload value of the SysTick timer (value to load when count reaches zero)
	static inline void SetReloadVal(uint32_t reloadVal) { SysTick->LOAD = reloadVal; }

	///Gets the current count of the SysTick timer
	static inline uint32_t GetCount() { return SysTick->VAL; }

	///Checks if the timer counted down to zero
	static inline bool CheckZeroFlag() { return SysTick->CTRL >> SysTick_CTRL_COUNTFLAG_Pos; }
};

#endif //__SYSTICK_CTRL_H__