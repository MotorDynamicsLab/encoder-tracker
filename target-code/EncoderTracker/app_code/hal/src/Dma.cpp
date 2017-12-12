//###########################################################################
// Dma.cpp
// Hardware Layer class that manages a single DMA module
//
// $Copyright: Copyright (C) LDO Systems
//###########################################################################
#include "Dma.h"

///Default constructor
Dma::Dma()
{
}


///Selects the DMA channel number
void Dma::Initialize(uint8_t dmaGroup, uint8_t dmaChannel)
{
	channel = dmaChannel;
	offset = (channel - 1) * 4;
	if (1 == dmaGroup)
	{
		RCC->AHBENR |= RCC_AHBENR_DMA1EN;
		statusReg = DMA1;
		switch (dmaChannel)
		{
		case 1:
			baseReg = (DMA_Channel_TypeDef*)DMA1_Channel1_BASE;
			break;
		case 2:
			baseReg = (DMA_Channel_TypeDef*)DMA1_Channel2_BASE;
			break;
		case 3:
			baseReg = (DMA_Channel_TypeDef*)DMA1_Channel3_BASE;
			break;
		case 4:
			baseReg = (DMA_Channel_TypeDef*)DMA1_Channel4_BASE;
			break;
		case 5:
			baseReg = (DMA_Channel_TypeDef*)DMA1_Channel5_BASE;
			break;
		case 6:
			baseReg = (DMA_Channel_TypeDef*)DMA1_Channel6_BASE;
			break;
		case 7:
			baseReg = (DMA_Channel_TypeDef*)DMA1_Channel7_BASE;
			break;
		default:
			break;
		}
	}
	else
	{
		RCC->AHBENR = RCC_AHBENR_DMA2EN;
		statusReg = DMA2;
		switch (dmaChannel)
		{
		case 1:
			baseReg = (DMA_Channel_TypeDef*)DMA2_Channel1_BASE;
			break;
		case 2:
			baseReg = (DMA_Channel_TypeDef*)DMA2_Channel2_BASE;
			break;
		case 3:
			baseReg = (DMA_Channel_TypeDef*)DMA2_Channel3_BASE;
			break;
		case 4:
			baseReg = (DMA_Channel_TypeDef*)DMA2_Channel4_BASE;
			break;
		case 5:
			baseReg = (DMA_Channel_TypeDef*)DMA2_Channel5_BASE;
			break;
		default:
			break;
		}
	}
}


///Sets the DMA channel priority
void Dma::ConfigPriority(DmaChPriority dmaChPriority)
{
	baseReg->CCR = (baseReg->CCR & ~DMA_CCR_PL_Msk) | (dmaChPriority << DMA_CCR_PL_Pos);
}


///Sets the data width of the transmission
void Dma::ConfigDataSize(DmaDataSize dmaDataSize)
{
	baseReg->CCR = (baseReg->CCR & ~DMA_CCR_MSIZE_Msk) | (dmaDataSize << DMA_CCR_MSIZE_Pos);
	baseReg->CCR = (baseReg->CCR & ~DMA_CCR_PSIZE_Msk) | (dmaDataSize << DMA_CCR_PSIZE_Pos);
}


///Enables or disables memory increment mode
void Dma::ConfigMemIncMode(bool isEnableIncMode)
{
	baseReg->CCR = (baseReg->CCR & ~DMA_CCR_MINC_Msk) | (isEnableIncMode << DMA_CCR_MINC_Pos);
}


///Enables or disables periphery increment mode
void Dma::ConfigPerphIncMode(bool isEnableIncMode)
{
	baseReg->CCR = (baseReg->CCR & ~DMA_CCR_PINC_Msk) | (isEnableIncMode << DMA_CCR_PINC_Pos);
}


///Enables or disables circular mode
void Dma::ConfigCircularMode(bool isEnableCircularMode)
{
	baseReg->CCR = (baseReg->CCR & ~DMA_CCR_CIRC_Msk) | (isEnableCircularMode << DMA_CCR_CIRC_Pos);
}


///Set data transmission direction
void Dma::ConfigDataDir(DmaDatDir dmaDataDir)
{
	if (_MemoryToMemory == dmaDataDir)
	{
		baseReg->CCR |= DMA_CCR_MEM2MEM;
		baseReg->CCR &= ~DMA_CCR_DIR;
	}
	else if (_MemoryToPeriph == dmaDataDir)
	{
		baseReg->CCR |= DMA_CCR_DIR;
		baseReg->CCR &= ~DMA_CCR_MEM2MEM;
	}
	else
	{
		baseReg->CCR &= ~DMA_CCR_MEM2MEM;
		baseReg->CCR &= ~DMA_CCR_DIR;
	}
}


///Enable or disable interrupts when a transmission error occurs
void Dma::ConfigTransErrInt(bool isEnable)
{
	baseReg->CCR = (baseReg->CCR & (~DMA_CCR_TEIE_Msk)) | (isEnable << DMA_CCR_TCIE_Pos);
}


///Enable or disable interrupts when transmission is half complete
void Dma::ConfigHalfTransInt(bool isEnable)
{
	baseReg->CCR = (baseReg->CCR & ~DMA_CCR_HTIE_Msk) | (isEnable << DMA_CCR_HTIE_Pos);
}


///Enable or disable interrupts when transmission is complete
void Dma::ConfigFullTransInt(bool isEnable)
{
	baseReg->CCR = (baseReg->CCR & ~DMA_CCR_TCIE_Msk) | (isEnable << DMA_CCR_TCIE_Pos);
}