//###########################################################################
// Dma.h
// Hardware Layer class that manages a single DMA module
//
// $Copyright: Copyright (C) LDO Systems
//###########################################################################
#ifndef __DMA_H__
#define __DMA_H__
#include "HalHeaders.h"

//---------------------------------------------------------------------------
// Class Definition
//

class Dma
{
protected:
	//Menbers
	volatile DMA_Channel_TypeDef* baseReg;
	volatile DMA_TypeDef* statusReg;
	uint8_t channel;
	uint8_t offset;

public:
	enum DmaChPriority
	{
		_Low = 0b00,
		_Medium = 0b01,
		_High = 0b10,
		_VeryHigh = 0b11,
	};

	enum DmaDataSize
	{
		_8Bits = 00,
		_16Bits = 01,
		_32Bits = 10,
		_Reserved = 11,
	};

	enum DmaDatDir
	{
		_PeriphToMemory = 0,
		_MemoryToPeriph = DMA_CCR_DIR,
		_MemoryToMemory = DMA_CCR_MEM2MEM,
	};

	//Methods
	Dma();
	void Initialize(uint8_t dmaGroup,uint8_t dmaChannel);
	void ConfigPriority(DmaChPriority dmaChPriority);
	void ConfigDataSize(DmaDataSize dmaDataSize);
	void ConfigMemIncMode(bool isEnableIncMode);
	void ConfigPerphIncMode(bool isEnableIncMode);
	void ConfigCircularMode(bool isEnableCircularMode);
	void ConfigDataDir(DmaDatDir dmaDataDir);
	void ConfigTransErrInt(bool isEnable);
	void ConfigHalfTransInt(bool isEnable);
	void ConfigFullTransInt(bool isEnable);
	
	///Configure memory address
	inline void SetMemAddr(void* memoryAddr) { baseReg->CMAR = (uint32_t)memoryAddr; };

	///Configure the peripheral address
	inline void SetPeriphAddr(void* periphAddr) { baseReg->CPAR = (uint32_t)periphAddr; };

	///Configure the length of the data to be transmitted
	inline void SetDataLen(uint16_t len) { baseReg->CNDTR = len; };

	///Enable DMA
	inline void Enable() { baseReg->CCR |= DMA_CCR_EN; };

	///Disable DMA
	inline void Disable() { baseReg->CCR &= ~DMA_CCR_EN; };

	///Get transfer error flag
	inline bool GetTransferErrorFlag() { return (statusReg->ISR & (DMA_ISR_TEIF1 << offset)) > 0; };

	///Clear transfer error flag
	inline void  ClearTransferErrorFlag() { statusReg->IFCR |= (DMA_IFCR_CTEIF1 << offset); };

	///Get half transfer  flag
	inline bool GetHalfTransferFlag() { return (statusReg->ISR & (DMA_ISR_HTIF1 << offset) > 0); };

	///Clear half transfer flag
	inline void  ClearHalfTransferFlag() { statusReg->IFCR |= (DMA_IFCR_CHTIF1 << offset); };

	///Get transfer complete  flag
	inline bool GetTransferCompleteFlag() { return (statusReg->ISR & (DMA_ISR_TCIF1 << offset)) > 0; };

	///Clear transfer complete flag
	inline void  ClearTransferCompleteFlag() { statusReg->IFCR |= (DMA_IFCR_CTCIF1 << offset); };

	///Get global interrupt  flag
	inline bool GetAnyFlag() { return (statusReg->ISR & (DMA_ISR_GIF1 << offset)) > 0; };

	///Clear global interrupt flag
	inline void  ClearAllFlags() { statusReg->IFCR |= (DMA_IFCR_CGIF1 << offset); };
};



#endif /*__DMA_H__*/