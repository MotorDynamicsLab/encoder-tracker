//###########################################################################
// Nvic.cpp
// Manages the Nested vectored interrupt controller (NVIC)
//
// $Copyright: Copyright (C) LDO Systems
//###########################################################################
#include "Nvic.h"


///Default Constructor
Nvic::Nvic()
{
}


///Config IRQ Type
void Nvic::Initialize(IRQn_Type type)
{
	irqType = type;
}


///Returns the priority group setting, which indicates the number of bits used for the group priority and subpriority.
Nvic::PriorityGrouping Nvic::GetPriorityGroupSetting()
{
	return PriorityGrouping(NVIC_GetPriorityGrouping());
}


///Gets the priority value of the specified interrupt and
///decodes it into group priority and sub priority fields
void Nvic::DecodePriority(uint32_t* groupPriority, uint32_t* subPriority)
{
	NVIC_DecodePriority(irqType, GetPriorityGroupSetting(), groupPriority, subPriority);
}


///Configures the priority group setting, which determines the number of bits used for the group priority and subpriority.
void Nvic::ConfigPriorityGroupSetting(PriorityGrouping prioGroup)
{
	NVIC_SetPriorityGrouping(prioGroup);
}

///Sets the group priority and sub priority of the selected interrupt
void Nvic::SetPriority(uint8_t groupPriority, uint8_t subPriority)
{
	uint32_t prioritygroup = NVIC_GetPriorityGrouping();
	NVIC_SetPriority(irqType, NVIC_EncodePriority(prioritygroup, groupPriority, subPriority));
}
