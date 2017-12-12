//###########################################################################
// Spi.h
// Hardware Layer class that manages a single SPI module
//
// $Copyright: Copyright (C) LDO Systems
//###########################################################################
#include "Spi.h"
#include "Gpio.h"
#include "AppHeaders.h"

///Constructor
Spi::Spi()
	: base((SPI_TypeDef*)SPI1_BASE)
{
}


///Initializes the internal members for the SPI object.
///does not actually perform any configuration
void Spi::Initialize(SpiChannel channel)
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
void Spi::ConfigModeAndPins(SpiMasterSel MasterSelection, SpiMode cpolCphaMode, SpiPinConfig pinConfig)
{
	if (MasterSelection == SpiMasterSel::_Master)
		base->CR1 |= SPI_CR1_MSTR;
	else
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

	if (_Master == MasterSelection)
	{
		//misoPin.ConfigOutputType(Gpio::_PushPull);
		//mosiPin.ConfigOutputType(Gpio::_PushPull);
		//sckPin.ConfigOutputType(Gpio::_PushPull);

		misoPin.ConfigInputType(Gpio::_PullUp);
		mosiPin.ConfigInputType(Gpio::_PullUp);
		sckPin.ConfigInputType(Gpio::_PullUp);

		//Enable software slave management
		base->CR1 = ( base->CR1 & ~(SPI_CR1_SSM_Msk) ) | SPI_CR1_SSM;

		//SET SSI bit
		base->CR1 = ( base->CR1 & ~(SPI_CR1_SSI_Msk) ) |  SPI_CR1_SSI;
	}
	else
	{
		misoPin.ConfigOutputType(Gpio::_PushPull);
		mosiPin.ConfigInputType(Gpio::_PullUp);
		sckPin.ConfigInputType(Gpio::_NoPull);

		//Disable software slave management
		base->CR1 = base->CR1 & ~(SPI_CR1_SSM_Msk);
	}

}


///Configures the frame size and order (LSB or MSB first)
void Spi::ConfigFrame(SpiLsbFirst lsbfirst, SpiDataSize datasize)
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
void Spi::ConfigBaudRatePrescaler(SpiBaudRate baud_rate)
{
	base->CR1 = (base->CR1 & ~SPI_CR1_BR_Msk) | (baud_rate << SPI_CR1_BR_Pos);
}


///Config 1-line or 2-line unidirectional data mode selected
void Spi::ConfigBitMode(SpiBitMode bitmode)
{
	base->CR1 = (base->CR1 & ~(SPI_CR1_BIDIMODE_Msk)) | (bitmode << SPI_CR1_BIDIMODE_Pos);
}


///Enable or disable hardware CRC calculation
void Spi::ConfigCrc(bool isEnableCrc)
{
	base->CR1 = (base->CR1 & ~(SPI_CR1_CRCEN_Msk)) | (isEnableCrc << SPI_CR1_CRCEN_Pos);
}


///Configure the frame format to Motorola or TI mode
void Spi::ConfigFrameFormat(SpiFrameFormat frf)
{
	base->CR2 = (base->CR2 & ~(SPI_CR2_FRF_Msk)) | (frf << SPI_CR2_FRF_Pos);
}


///Configures the threshold of the RXFIFO that triggers an RXNE event
void Spi::ConfigFifoRecThreshold(SpiRxFifoThres SpiRxThresthreshold)
{
	base->CR2 = (base->CR2 & ~(SPI_CR2_FRXTH_Msk)) | (SpiRxThresthreshold << SPI_CR2_FRXTH_Pos);
}


///Enable or disable the DMA function for SPI
void Spi::ConfigDma(bool isEnableTxDma, bool isEnableRxDma)
{
	base->CR2 = (base->CR2 & (~SPI_CR2_TXDMAEN)) | (isEnableTxDma << SPI_CR2_TXDMAEN_Pos);
	base->CR2 = (base->CR2 & (~SPI_CR2_RXDMAEN)) | (isEnableRxDma << SPI_CR2_RXDMAEN_Pos);
}



uint8_t Spi::ReadWriteByte(uint8_t txData)
{
	while (!base->SR & SPI_SR_TXE);
	base->DR = txData;
	while (!base->SR & SPI_SR_RXNE);
	return uint8_t(base->DR);
}

///Receive data from SPI port
bool isRxReady = true;
void Spi::Read(uint8_t* rxData, uint16_t rxSize)
{
	if (!isRxReady)
	{
		return;
	}
	else
	{
		isRxReady = !isRxReady;
	}

	//Set the Rx FiFo threshold
	SET_BIT(base->CR2, SPI_CR2_FRXTH);

	if (!(base->CR1 & SPI_CR1_SPE_Msk))
	{
		Enable();
	}

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
	while (base->SR & SPI_SR_FTLVL_Msk)
	{
		//Disable TXE, RXNE and ERR interrupts for the interrupt process
		//base->CR2 &= ~(SPI_CR2_TXEIE | SPI_CR2_RXNEIE | SPI_CR2_ERRIE);
	}
	//Control the BSY flag
	while (base->SR & SPI_SR_BSY_Msk);


	// Control if the RX fifo is empty
	uint8_t tmpreg;
	while (base->SR & SPI_SR_RXNE_Msk)
	{
		tmpreg = *(volatile uint8_t*)&base->DR;
	}
	isRxReady = !isRxReady;
}


///Send data to SPI port
//bool isRxReady = true;
void Spi::Write(const uint8_t* txData, uint16_t txSize)
{
	if (!isRxReady)
	{
		return;
	}
	else
	{
		isRxReady = !isRxReady;
	}

	//Check if the SPI is already enabled
	if (!base->CR1 & SPI_CR1_SPE)
	{
		Enable();
	}

	if (!(base->CR1 & SPI_CR1_MSTR_Msk) || txSize == 1)
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
	uint32_t bug = 0;
	while ( base->SR & SPI_SR_FTLVL_Msk )
	{
		bug++;
		//Disable TXE, RXNE and ERR interrupts for the interrupt process
		//base->CR2 &= ~(SPI_CR2_TXEIE | SPI_CR2_RXNEIE | SPI_CR2_ERRIE);
	}


	//Control the BSY flag
	while (base->SR & SPI_SR_BSY_Msk)
	{
		base->CR2 &= ~(SPI_CR2_TXEIE | SPI_CR2_RXNEIE | SPI_CR2_ERRIE);
	}


	// Control if the RX fifo is empty
	uint8_t tmpreg;
	/*while (base->SR & SPI_SR_FRLVL_Msk)
	{
		tmpreg = *(volatile uint8_t*)&base->DR;
	}*/
	while (base->SR & SPI_SR_RXNE_Msk)
	{
		tmpreg = *(volatile uint8_t*)&base->DR;
	}

	//Clear overrun flag in 2 Lines communication mode because received is not read
	uint32_t tmpreg_ovr = 0x00U;
	if (!(base->CR1 & SPI_CR1_BIDIMODE_Msk))
	{
		tmpreg_ovr = base->DR;
	}

	//Clear overrun flag in 2 Lines communication mode because received is not read

	isRxReady = !isRxReady;
}


///Simultaneously send data and receive data 
void Spi::ReadAndWrite(uint8_t * rxData, const uint8_t * txData, uint16_t size)
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

void Spi::FlushRxFifo()
{
	uint32_t tmpreg;
	while (0 != (base->SR & SPI_SR_FRLVL))
	{
		if (base->SR & SPI_SR_RXNE)
		{
			tmpreg = base->DR;
		}
	}
}
