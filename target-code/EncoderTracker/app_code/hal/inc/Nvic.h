//###########################################################################
// Nvic.h
// Manages the Nested vectored interrupt controller (NVIC)
//
// $Copyright: Copyright (C) LDO Systems
//###########################################################################
#ifndef __NVIC_H__
#define __NVIC_H__
#include "HalHeaders.h"


class Nvic
{
private:
	//Members
	IRQn_Type irqType;

public:

	//Enumerations
	enum PriorityGrouping
	{
		_PriorityGroup0 = (0x00000007U), // 0 bits for pre-emption priority 7 bits for subpriority 
		_PriorityGroup1 = (0x00000006U), // 1 bits for pre-emption priority 6 bits for subpriority 
		_PriorityGroup2 = (0x00000005U), // 2 bits for pre-emption priority 5 bits for subpriority 
		_PriorityGroup3 = (0x00000004U), // 3 bits for pre-emption priority 4 bits for subpriority 
		_PriorityGroup4 = (0x00000003U), // 4 bits for pre-emption priority 3 bits for subpriority 
		_PriorityGroup5 = (0x00000002U), // 5 bits for pre-emption priority 2 bits for subpriority 
		_PriorityGroup6 = (0x00000001U), // 6 bits for pre-emption priority 1 bits for subpriority 
		_PriorityGroup7 = (0x00000000U), // 7 bits for pre-emption priority 0 bits for subpriority 
	};

	//Methods
	Nvic();
	void Initialize(IRQn_Type type);
	static PriorityGrouping GetPriorityGroupSetting();
	void DecodePriority(uint32_t * groupPriority, uint32_t* subPriority);
	static void ConfigPriorityGroupSetting(PriorityGrouping prioGroup);
	void SetPriority(uint8_t groupPriority, uint8_t subPriority);

	///Clears the pending bit of an external interrupt.
	inline void ClearPending() { NVIC_ClearPendingIRQ(irqType); }

	///Disables a device specific interrupt in the NVIC interrupt controller.
	inline void DisableInterrupt() { NVIC_DisableIRQ(irqType); }

	///Enables a device specific interrupt in the NVIC interrupt controller.
	void EnableInterrupt() { NVIC_EnableIRQ(irqType); }

	///Checks if specified interrupt is active
	inline bool IsActive() { return NVIC_GetActive(irqType) > 0; }

	///Checks if specified interrupt is pending
	bool IsPending() { return NVIC_GetPendingIRQ(irqType) > 0; }

	///Forces a selected interrupt into pending state
	inline void ForcePending() { NVIC_SetPendingIRQ(irqType); }
};



#endif // !__NVIC_H__
