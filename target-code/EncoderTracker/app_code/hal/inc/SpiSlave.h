//###########################################################################
// Spi.h
// Hardware Layer class that manages a single SPI module
//
// $Copyright: Copyright (C) LDO Systems
//###########################################################################
#ifndef __SPI_SLAVE_H__
#define __SPI_SLAVE_H__

#include "HalHeaders.h"
#include "Gpio.h"
#include "Gpi.h"

//---------------------------------------------------------------------------
// Class Definition
//
class SpiSlave
{
protected:
	//Members
	volatile SPI_TypeDef *base;
	Gpi nssPin;

	bool isSelected() { return 0 == nssPin.ReadRaw(); }

public:
	//Enumerations
	enum SpiChannel
	{
		_Spi1 = SPI1_BASE,
		_Spi2 = SPI2_BASE,
		_Spi3 = SPI3_BASE,
	};

	enum SpiMode
	{
		_Cpol0Cpha0 = 0b00,
		_Cpol0Cpha1 = 0b01,
		_Cpol1Cpha0 = 0b10,
		_Cpol1Cpha1 = 0b11,
	};

	enum SpiBaudRate
	{
		_Fpclk2   = 0b000,  //SPI clock = Fpclk / 2
		_Fpclk4   = 0b001,  //SPI clock = Fpclk / 4
		_Fpclk8   = 0b010,  //SPI clock = Fpclk / 8
		_Fpclk16  = 0b011,  //SPI clock = Fpclk / 16
		_Fpclk32  = 0b100,  //SPI clock = Fpclk / 32
		_Fpclk64  = 0b101,  //SPI clock = Fpclk / 64
		_Fpclk128 = 0b110,  //SPI clock = Fpclk / 128
		_Fpclk256 = 0b111,  //SPI clock = Fpclk / 256
	};

	enum SpiBitMode
	{
		_TwoLine = 0,
		_OneLine = 1,
	};

	enum SpiDataSize
	{
		_4Bit  = 0b0011,
		_5Bit  = 0b0100,
		_6Bit  = 0b0101,
		_7Bit  = 0b0110,
		_8Bit  = 0b0111,
		_9Bit  = 0b1000,
		_10Bit = 0b1001,
		_11Bit = 0b1010,
		_12Bit = 0b1011,
		_13Bit = 0b1100,
		_14Bit = 0b1101,
		_15Bit = 0b1110,
		_16Bit = 0b1111,
	};

	enum SpiRxFifoThres
	{
		_2Byte = 0,
		_1Byte = 1
	};

	enum SpiLsbFirst
	{
		_MsbFirst = 0,
		_LsbFirst = 1,
	};

	//Structures
	struct SpiPinConfig
	{
		Gpio::GpioChannel nssCh;
		Gpio::GpioChannel sckCh;
		Gpio::GpioChannel misoCh;
		Gpio::GpioChannel mosiCh;
		uint16_t nssPin;
		uint16_t sckPin;
		uint16_t mosiPin;
		uint16_t misoPin;
		uint8_t nssAltNum;
		uint8_t sckAltNum;
		uint8_t mosiAltNum;
		uint8_t misoAltNum;
	};

	//Methods
	SpiSlave();
	void Initialize(SpiChannel channel);
	void ConfigMode(SpiMode cpolCphaMode);
	void ConfigPins(SpiPinConfig pinConfig);
	void ConfigFrame(SpiLsbFirst lsbfirst, SpiDataSize datasize);
	void ConfigBaudRatePrescaler(SpiBaudRate baud_rate);
	void ConfigFifoRecThreshold(SpiRxFifoThres status);
	void ConfigDma(bool isEnableTxDma, bool isEnableRxDma);
	inline void Enable() { base->CR1 |= SPI_CR1_SPE; }
	inline void Disable() { base->CR1 &= ~SPI_CR1_SPE; }
	uint8_t ReadWriteByte(uint8_t txData);
	void Read(uint8_t* rxData, uint16_t rxSize);
	void Write(const uint8_t* txData, uint16_t txSize);
	void ReadAndWrite(uint8_t* rxData, const uint8_t* txData, uint16_t size);
	void FlushRxFifo();
};

#endif /* __SPI_H__ */
