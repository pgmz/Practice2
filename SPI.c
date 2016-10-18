/*
 * SPI.c
 *
 *  Created on: 19/09/2016
 *      Author: Patricio Gomez
 */

#include "SPI.h"

/*SPI_enable, enables the SPI channel indicated*/
void SPI_enable(SPI_ChannelType channel){

	switch(channel){
	case SPI_0:
		SPI0_MCR &= ~(SPI_MCR_MDIS_MASK);
		break;

	case SPI_1:
		SPI1_MCR &= ~(SPI_MCR_MDIS_MASK);
		break;

	case SPI_2:
		SPI2_MCR &= ~(SPI_MCR_MDIS_MASK);
		break;
	}

}

/*SPI_clk, enables the module, according to its position in the clock gating*/
void SPI_clk(SPI_ChannelType channel){

	switch(channel){
	case SPI_0:
		SIM->SCGC6 |= SPI0_CLOCK_GATING;
		break;

	case SPI_1:
		SIM->SCGC6 |= SPI1_CLOCK_GATING;
		break;

	case SPI_2:
		SIM->SCGC3 |= SPI2_CLOCK_GATING;
		break;
	}
}

/*configures as master or slave, the SPI channel received*/
void SPI_setMaster(SPI_ChannelType channel, SPI_MasterType masterOrSlave){
	switch(channel){
	case SPI_0:
		SPI0_MCR &= ~SPI_MCR_MSTR_MASK;
		SPI0_MCR |= SPI_MCR_MSTR(masterOrSlave);
		break;

	case SPI_1:
		SPI1_MCR &= ~SPI_MCR_MSTR_MASK;
		SPI1_MCR |= SPI_MCR_MSTR(masterOrSlave);
		break;

	case SPI_2:
		SPI2_MCR &= ~SPI_MCR_MSTR_MASK;
		SPI2_MCR |= SPI_MCR_MSTR(masterOrSlave);
		break;
	}
}

/*enables or disables the FIFO, in the SPI channel received*/
void SPI_FIFO(SPI_ChannelType channel, SPI_EnableFIFOType enableOrDisable){

	switch(channel){
	case SPI_0:
		SPI0_MCR &= ~(SPI_MCR_DIS_TXF_MASK | SPI_MCR_DIS_RXF_MASK);
		SPI0_MCR |= (SPI_MCR_DIS_RXF(enableOrDisable)|SPI_MCR_DIS_TXF(enableOrDisable));
		break;

	case SPI_1:
		SPI1_MCR &= ~(SPI_MCR_DIS_TXF_MASK | SPI_MCR_DIS_RXF_MASK);
		SPI1_MCR |= (SPI_MCR_DIS_RXF(enableOrDisable)|SPI_MCR_DIS_TXF(enableOrDisable));
		break;

	case SPI_2:
		SPI2_MCR &= ~(SPI_MCR_DIS_TXF_MASK | SPI_MCR_DIS_RXF_MASK);
		SPI2_MCR |= (SPI_MCR_DIS_RXF(enableOrDisable)|SPI_MCR_DIS_TXF(enableOrDisable));
		break;
	}

}

/*configures the clock polarity, according to the SPI channel received*/
void SPI_clockPolarity(SPI_ChannelType channel, SPI_PolarityType cpol){

	switch(channel){
	case SPI_0:
		SPI0_CTAR0 &= ~(SPI_CTAR_CPOL_MASK);
		SPI0_CTAR0 |= SPI_CTAR_CPOL(cpol);
		break;

	case SPI_1:
		SPI1_CTAR0 &= ~(SPI_CTAR_CPOL_MASK);
		SPI1_CTAR0 |= SPI_CTAR_CPOL(cpol);
		break;

	case SPI_2:
		SPI2_CTAR0 &= ~(SPI_CTAR_CPOL_MASK);
		SPI2_CTAR0 |= SPI_CTAR_CPOL(cpol);
		break;
	}

}
/*Sets the frameSize to the SPI channel received*/
void SPI_frameSize(SPI_ChannelType channel, uint32 frameSize){

	switch(channel){
	case SPI_0:
		SPI0_CTAR0 &= ~(SPI_CTAR_FMSZ_MASK);
		SPI0_CTAR0 |= frameSize;
		break;

	case SPI_1:
		SPI1_CTAR0 &= ~(SPI_CTAR_FMSZ_MASK);
		SPI1_CTAR0 |= frameSize;
		break;

	case SPI_2:
		SPI2_CTAR0 &= ~(SPI_CTAR_FMSZ_MASK);
		SPI2_CTAR0 |= frameSize;
		break;
	}
}

/*Sets the clock phase to the SPI channel received*/
void SPI_clockPhase(SPI_ChannelType channel, SPI_PhaseType cpha){

	switch(channel){
	case SPI_0:
		SPI0_CTAR0 &= ~(SPI_CTAR_CPHA_MASK);
		SPI0_CTAR0 |= SPI_CTAR_CPHA(cpha);
		break;

	case SPI_1:
		SPI1_CTAR0 &= ~(SPI_CTAR_CPHA_MASK);
		SPI1_CTAR0 |= SPI_CTAR_CPHA(cpha);
		break;

	case SPI_2:
		SPI2_CTAR0 &= ~(SPI_CTAR_CPHA_MASK);
		SPI2_CTAR0 |= SPI_CTAR_CPHA(cpha);
		break;
	}
}

/*Sets the baud rate to the SPI channel received*/
static void SPI_baudRate(SPI_ChannelType channel, uint32 baudRate){

	switch(channel){
	case SPI_0:
		SPI0_CTAR0 &= ~(SPI_CTAR_BR_MASK);
		SPI0_CTAR0 |= SPI_CTAR_BR(baudRate);
		break;

	case SPI_1:
		SPI1_CTAR0 &= ~(SPI_CTAR_BR_MASK);
		SPI1_CTAR0 |= SPI_CTAR_BR(baudRate);
		break;

	case SPI_2:
		SPI2_CTAR0 &= ~(SPI_CTAR_BR_MASK);
		SPI2_CTAR0 |= SPI_CTAR_BR(baudRate);
		break;
	}
}

/*Sets if Most Significant Bit or Less Significant Bit is first*/
static void SPI_mSBFirst(SPI_ChannelType channel, SPI_LSMorMSBType msb){

	switch(channel){
	case SPI_0:
		SPI0_CTAR0 &= ~(SPI_CTAR_LSBFE_MASK);
		SPI0_CTAR0 |= SPI_CTAR_LSBFE(msb);
		break;

	case SPI_1:
		SPI1_CTAR0 &= ~(SPI_CTAR_LSBFE_MASK);
		SPI1_CTAR0 |= SPI_CTAR_LSBFE(msb);
		break;

	case SPI_2:
		SPI2_CTAR0 &= ~(SPI_CTAR_LSBFE_MASK);
		SPI2_CTAR0 |= SPI_CTAR_LSBFE(msb);
		break;
	}
}

/*Starts transference in the SPI Channel received*/
void SPI_startTranference(SPI_ChannelType channel){

	switch(channel){
	case SPI_0:
		SPI0_MCR &= ~SPI_MCR_HALT_MASK;
		break;

	case SPI_1:
		SPI1_MCR &= ~SPI_MCR_HALT_MASK;
		break;

	case SPI_2:
		SPI2_MCR &= ~SPI_MCR_HALT_MASK;
		break;
	}
}

/*Stops transference in the SPI channel received*/
void SPI_stopTranference(SPI_ChannelType channel){

	switch(channel){
	case SPI_0:
		SPI0_MCR |= SPI_MCR_HALT_MASK;
		break;

	case SPI_1:
		SPI1_MCR |= SPI_MCR_HALT_MASK;
		break;

	case SPI_2:
		SPI2_MCR |= SPI_MCR_HALT_MASK;
		break;
	}
}

/*Sends data (1 byte) through the SPI channel 0*/
void SPI_sendOneByte(SPI_ChannelType channel, uint8 Data){
	switch(channel){
	case SPI_0:
		/*Pushes the data in the TX FIFO*/
		SPI0_PUSHR = (Data);
		/*While transference is completed, wait*/
		while(0 == (SPI0_SR & SPI_SR_TCF_MASK));
		/*Clear the Transference complete flag*/
		SPI0_SR |= SPI_SR_TCF_MASK;
		break;

	case SPI_1:
		/*Pushes the data in the TX FIFO*/
		SPI1_PUSHR = (Data);
		/*While transference is completed, wait*/
		while(0 == (SPI1_SR & SPI_SR_TCF_MASK));
		/*Clear the Transference complete flag*/
		SPI1_SR |= SPI_SR_TCF_MASK;
		break;

	case SPI_2:
		/*Pushes the data in the TX FIFO*/
		SPI2_PUSHR = (Data);
		/*While transference is completed, wait*/
		while(0 == (SPI2_SR & SPI_SR_TCF_MASK));
		/*Clear the Transference complete flag*/
		SPI2_SR |= SPI_SR_TCF_MASK;
		break;
	}

}

/*Initialize a SPI channel, according to a struct that has the configuration
 * for a SPI channel, and invoke the corresponding functions to configure each
 * parameter*/
void SPI_init(const SPI_ConfigType* SPI_Config){
	SPI_clk(SPI_Config->SPI_Channel);
	GPIO_clockGating(SPI_Config->GPIOForSPI.GPIO_portName);
	GPIO_pinControlRegister(SPI_Config->GPIOForSPI.GPIO_portName,SPI_Config->GPIOForSPI.SPI_clk,&(SPI_Config->pinConttrolRegisterPORTD));
	GPIO_pinControlRegister(SPI_Config->GPIOForSPI.GPIO_portName,SPI_Config->GPIOForSPI.SPI_Sout,&(SPI_Config->pinConttrolRegisterPORTD));
	SPI_setMaster(SPI_Config->SPI_Channel, SPI_Config->SPI_Master);
	SPI_FIFO(SPI_Config->SPI_Channel, SPI_Config->SPI_EnableFIFO);
	SPI_enable(SPI_Config->SPI_Channel);
	SPI_clockPolarity(SPI_Config->SPI_Channel, SPI_Config->SPI_Polarity);
	SPI_frameSize(SPI_Config->SPI_Channel, SPI_Config->frameSize);
	SPI_clockPhase(SPI_Config->SPI_Channel, SPI_Config->SPI_Phase);
	SPI_baudRate(SPI_Config->SPI_Channel, SPI_Config->baudrate);
	SPI_mSBFirst(SPI_Config->SPI_Channel, SPI_MSB);
}
