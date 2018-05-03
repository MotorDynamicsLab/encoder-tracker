//###########################################################################
// Spi.h
// Hardware Layer class that manages a single SPI module
//
// $Copyright: Copyright (C) LDO Systems
//###########################################################################
#include "SpiSlave.h"
#include "Gpio.h"
#include "AppHeaders.h"

///Constructor
SpiSlave::SpiSlave()
	: base((SPI_TypeDef*)SPI1_BASE)
{
}


///Initializes the internal members for the SPI object.
///does not actually perform any configuration
void SpiSlave::Initialize(SpiChannel channel)
{
	base = (SPI_TypeDef*)channel;

	if (_Spi1 == channel)
	{
		RCC->APB2ENR |= 1 << 12;
	}
	else if(_Spi2 == channel)
	{
		RCC->APB1ENR |= 1 << 14;
	}
	else if (_Spi3 == channel)
	{
		RCC->APB1ENR |= 1 << 15;
	}
}


///Performs basic configuration, selects Master/Slave mode
///Selects CPOL and CPHA values.
///Configures GPIO pins for SPI use
void SpiSlave::ConfigModeAndPins(SpiMode cpolCphaMode, SpiPinConfig pinConfig)
{
	base->CR1 &= ~SPI_CR1_MSTR;

	const uint32_t cpol_cpha_msk = SPI_CR1_CPOL | SPI_CR1_CPHA;
	base->CR1 = (base->CR1 & ~cpol_cpha_msk) | cpolCphaMode;

	Gpio misoPin;
	Gpio mosiPin;
	Gpio sckPin;

	sckPin.Initialize(pinConfig.sckCh, pinConfig.sckPin);
	sckPin.ConfigMode(Gpio::_Alt);
	sckPin.ConfigAltFunc(pinConfig.sckAltNum);
	sckPin.ConfigSpeed(Gpio::_HighSpeed);

	mosiPin.Initialize(pinConfig.mosiCh, pinConfig.mosiPin);
	mosiPin.ConfigMode(Gpio::_Alt);
	mosiPin.ConfigAltFunc(pinConfig.mosiAltNum);
	mosiPin.ConfigSpeed(Gpio::_HighSpeed);

	misoPin.Initialize(pinConfig.misoCh, pinConfig.misoPin);
	misoPin.ConfigMode(Gpio::_Alt);
	misoPin.ConfigAltFunc(pinConfig.misoAltNum);
	misoPin.ConfigSpeed(Gpio::_HighSpeed);

	nssPin.Initialize(pinConfig.nssCh, pinConfig.nssPin);
	nssPin.ConfigMode(Gpio::_Alt);
	nssPin.ConfigAltFunc(pinConfig.nssAltNum);
	nssPin.ConfigSpeed(Gpio::_HighSpeed);

	misoPin.ConfigOutputType(Gpio::_PushPull);
	mosiPin.ConfigInputType(Gpio::_PullUp);
	sckPin.ConfigInputType(Gpio::_NoPull);
	nssPin.ConfigInputType(Gpio::_NoPull);
}


///Configures the frame size and order (LSB or MSB first)
void SpiSlave::ConfigFrame(SpiLsbFirst lsbfirst, SpiDataSize datasize)
{
	if (SpiLsbFirst::_LsbFirst == lsbfirst)
		base->CR1 |= SPI_CR1_LSBFIRST;
	else
		base->CR1 &= ~SPI_CR1_LSBFIRST;

	base->CR2 = (base->CR2 & ~SPI_CR2_DS_Msk) | (datasize << SPI_CR2_DS_Pos);
	SET_BIT(base->CR2, SPI_CR2_FRXTH);
	SPI_SR_FRLVL;
}


///Sets the baud rate of sclk during master mode
///The baud rate of SPI2 SPI3 is based on APP1
///The baud rate of SPI1 SPI4 is based on APP2
void SpiSlave::ConfigBaudRatePrescaler(SpiBaudRate baud_rate)
{
	base->CR1 = (base->CR1 & ~SPI_CR1_BR_Msk) | (baud_rate << SPI_CR1_BR_Pos);
}


///Configures the threshold of the RXFIFO that triggers an RXNE event
void SpiSlave::ConfigFifoRecThreshold(SpiRxFifoThres SpiRxThresthreshold)
{
	base->CR2 = (base->CR2 & ~(SPI_CR2_FRXTH_Msk)) | (SpiRxThresthreshold << SPI_CR2_FRXTH_Pos);
}


///Enable or disable the DMA function for SPI
void SpiSlave::ConfigDma(bool isEnableTxDma, bool isEnableRxDma)
{
	base->CR2 = (base->CR2 & (~SPI_CR2_TXDMAEN)) | (isEnableTxDma << SPI_CR2_TXDMAEN_Pos);
	base->CR2 = (base->CR2 & (~SPI_CR2_RXDMAEN)) | (isEnableRxDma << SPI_CR2_RXDMAEN_Pos);
}



uint8_t SpiSlave::ReadWriteByte(uint8_t txData)
{
	while (!base->SR & SPI_SR_TXE);
	base->DR = txData;
	while (!base->SR & SPI_SR_RXNE);
	return uint8_t(base->DR);
}

///Receive data from SPI port
void SpiSlave::Read(uint8_t* rxData, uint16_t rxSize)
{
	while (0 < rxSize)
	{
		if (base->SR & SPI_SR_RXNE_Msk)
		{
			*rxData = *(volatile uint8_t*)&base->DR;
			rxData += sizeof(uint8_t);
			rxSize--;
		}
	}

	//Check the end of the transaction
	//Control if the TX fifo is empty
	while (base->SR & SPI_SR_FTLVL_Msk);

	//Control the BSY flag
	while (base->SR & SPI_SR_BSY_Msk);

	// Control if the RX fifo is empty
	uint8_t tmpreg;
	while (base->SR & SPI_SR_RXNE_Msk)
	{
		tmpreg = *(volatile uint8_t*)&base->DR;
	}
}


///Send data to SPI port
void SpiSlave::Write(const uint8_t* txData, uint16_t txSize)
{
	while (0 < txSize)
	{
		if (base->SR & SPI_SR_TXE_Msk)
		{
			if (1 < txSize)
			{
				base->DR = *((uint16_t*)txData);
				txData += sizeof(uint16_t);
				txSize -= sizeof(uint16_t);
			}
			else
			{
				*(volatile uint8_t*)&base->DR = *txData++;
				txSize--;
			}
		}
	}

	//Check the end of the transaction
	//Control if the TX fifo is empty
	while (base->SR & SPI_SR_FTLVL_Msk);
	while (base->SR & SPI_SR_BSY_Msk);

	//Flush any data received while transmitting
	FlushRxFifo();
}


///Simultaneously send data and receive data 
void SpiSlave::ReadAndWrite(uint8_t * rxData, const uint8_t * txData, uint16_t size)
{
	if (__null == rxData || __null == txData) return;

	uint16_t TxCount = size;
	uint16_t RxCount = size;
	uint8_t txallowed = 1;

	//Set the Rx Fido threshold
	//set fiforxthreshold according the reception data length: 8bit
	SET_BIT(base->CR2, SPI_CR2_FRXTH);

	//Check if the SPI is already enabled
	if (0 == (base->CR1 & SPI_CR1_SPE)) Enable();

	//Receive and Send data in 8 Bit mode
	if (((base->CR1 & SPI_CR1_MSTR_Msk) != SPI_CR1_MSTR) || 1 == TxCount)
	{
		if (size > 1)
		{
			base->DR = *((uint16_t *)txData);
			txData += sizeof(uint16_t);
			TxCount -= 2;
		}
		else
		{
			*(volatile uint8_t*)&base->DR = (*txData++);
			TxCount--;
		}
	}

	while (TxCount > 0 || RxCount > 0)
	{
		//check TXE flag
		if (txallowed && (TxCount > 0) && (SPI_SR_TXE == (base->SR & SPI_SR_TXE)))
		{
			if (TxCount > 1)
			{
				base->DR = *((uint16_t *)txData);
				txData += sizeof(uint16_t);
				TxCount -= 2;
			}
			else
			{
				*(volatile uint8_t *)&base->DR = (*txData++);
				TxCount--;
			}
			//Next Data is a reception (Rx). Tx not allowed
			txallowed = 0;
		}

		//Wait until RXNE flag is reset
		if ((RxCount > 0) && (SPI_SR_RXNE == (base->SR & SPI_SR_RXNE)))
		{
			if (RxCount > 1)
			{
				*((uint16_t *)rxData) = base->DR;
				rxData += sizeof(uint16_t);
				RxCount -= 2;
				//set fiforxthresold before to switch on 8 bit data size
				if (RxCount <= 1)
				{
					SET_BIT(base->CR2, SPI_CR2_FRXTH);
				}
			}
			else
			{
				*(uint8_t *)rxData = *(volatile uint8_t *)&base->DR;
				rxData++;
				RxCount--;
			}

			//Next Data is a Transmission (Tx). Tx is allowed
			txallowed = 1;
		}

	}

	//Check the end of the transaction
	if (SPI_CR1_MSTR == (base->CR1 & SPI_CR1_MSTR))
	{
		while ((base->SR & SPI_SR_FRLVL) != 0);
		while (0 != (SPI_SR_FTLVL_Msk & base->SR));
	}
	while (0 != (SPI_SR_BSY_Msk & base->SR));
}

///Flush the RX FIFO buffer
void SpiSlave::FlushRxFifo()
{
	uint32_t rxData;
	while (0 != (base->SR & SPI_SR_FRLVL))
	{
		if (base->SR & SPI_SR_RXNE)
		{
			rxData = base->DR;
		}
	}
}