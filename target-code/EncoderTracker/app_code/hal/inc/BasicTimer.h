//###########################################################################
// BasicTimer.h
// Hardware Layer class that manages a single BasicTimer module
//
// $Copyright: Copyright (C) LDO Systems
//###########################################################################
#ifndef __BASIC_TIMER__
#define __BASIC_TIMER__
#include "HalHeaders.h"

//---------------------------------------------------------------------------
// Class Definition
//

class BaseTimer
{
protected:
	//Members
	//volatile TIM_TypeDef *base;

public:
	//Enumerations
	volatile TIM_TypeDef *base;
	enum TimerChannel
	{
		_Tim1  = TIM1_BASE,
		_Tim2  = TIM2_BASE,
		_Tim3  = TIM3_BASE,
		_Tim4  = TIM4_BASE,
		_Tim6  = TIM6_BASE,
		_Tim7  = TIM7_BASE,
		_Tim8  = TIM8_BASE,
		_Tim15 = TIM15_BASE,
		_Tim16 = TIM16_BASE,
		_Tim17 = TIM17_BASE,
	};

	enum UpdateRequestSource
	{
		_FurtherRequestSource = 0,
		_CounterFlowOnly = 1,
	};

	enum MasterModeSelection
	{
		_Reset = 0b000,
		_Enable = 0b001,
		_Update = 0b010,
	};

	//Methods
	BaseTimer();
	void Initialize(TimerChannel timchannel);
	void RemapUif(bool isRemap);
	void ToggleAutoReloadPreload(bool enablePreload);
	void ToggleOnePulseMode(bool enableOnePulseMode);
	void SelectUpdateRequestSource(UpdateRequestSource updaterequestsource);
	void ToggleAutoUpdateEvent(bool isDisableAutoUpdateEvent);
	void ConfigMasterModeSelection(MasterModeSelection mastermodeselection);
	void ToggleUpdateDmaRequest(bool enable);
	void ToggleUpdateInterrupt(bool enable);

	///Clears the update interrupt flag
	inline void ClearUpdateInterruptFlag() { base->SR &= ~TIM_SR_UIF_Msk; };

	///Forces an update event, which re-initializes timer counter and prescaler-counter.
	inline void ForceUpdate() { base->EGR |= TIM_EGR_UG; };

	///Forces the counter value
	inline void SetCounter(uint32_t val) { base->CNT = val; };

	///Gets the the counter value
	inline uint32_t GetCounter() { return uint32_t(base->CNT); };

	//Sets the auto-reload value
	inline void SetReloadVal(uint32_t value) { base->ARR = value; };

	///Gets the auto-reload value
	uint32_t GetReloadVal() { return uint32_t(base->ARR); };

	///The counter clock frequency CK_CNT is equal to fCK_PSC / (PSC[15:0] + 1).
	inline void SetPrescaler(uint32_t pscvalue) { base->PSC = pscvalue; };

	///Gets the counter clock prescaler value
	inline uint32_t GetPrescaler() { return uint32_t(base->PSC); };

	///Enables timer - the counter starts
	inline void Enable() { base->CR1 |= TIM_CR1_CEN; };

	///Disables timer = the counter stops
	inline void Disable() { base->CR1 &= ~TIM_CR1_CEN; };
};



#endif  //__BASIC_TIMER__ 