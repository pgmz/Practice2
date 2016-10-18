/*
 * ADC.h
 *
 *  Created on: 06/10/2016
 *      Author: Patricio Gomez
 */

#ifndef SOURCES_ADC_H_
#define SOURCES_ADC_H_

#include "DataTypeDefinitions.h"

/**
 * Struct ADC_MailBoxType contains the flag and Data that are updated when an
 * interrupt occurs
 * **/
typedef struct{
	/*flag is set to TRUE when an interruption occurs, and FALSE when MailBoxData
	 * is read*/
	uint8 flag :1;
	/*MailBoxData has the ADC convertion result*/
	float MailBoxData;
}ADC_MailBoxType;

/**
 * Enumeration ADC_ChannelType that indicates which ADC channel will be used
 * **/
typedef enum{	ADC_0,	ADC_1}ADC_ChannelType;

/**
 * Enumeration AB_ChannelType that indicates which channel (A or B) within the ADC
 * will be used
 * **/
typedef enum{	A,	B}AB_ChannelType;

/**
 * Enumeration singleOrDifferential that indicates how the ADC will be used.
 * Single Ended refers to (Vmax = 3.3V, Vmin = 0), and Differential refers to
 * (Vmax = 3.3V, Vmin = -3.3V)
 * **/
typedef enum{	SINGLE_ENDED,	DIFFERENTIAL}singleOrDifferential;

/**
 * Enumeration inputChannelSelect that indicates which input will be be used.
 * **/
typedef enum{	DAD0 = 0, DAD1 = 1, DAD2 = 2, DAD3 = 3,
	AD4 = 4, AD5 = 5, AD6 = 6, AD7 = 7, AD8 = 8, AD9 = 9, AD10 = 10, AD11 = 11, AD12 = 12, AD13 = 13, AD14 = 14,
	AD15 = 15, AD16 = 16, AD17 = 17, AD18 = 18, AD19 = 19, AD20 = 20, AD21 = 21, AD22 = 22, AD23 = 23,
	TEMP_SENSOR = 26, BAND_GAP = 27, VREFSH_DIFF = 28, VREFSH_SINGLE = 29
}inputChannelSelect;

/**
 * Enumeration normalOrLowPower that indicates how the ADC will work,
 * low power is useful to reduce noise in the input
 * **/
typedef enum{	NORMAL_POWER,	LOW_POWER}normalOrLowPower;

/**
 * Enumeration clockDivider that indicates if the clock source will have a pre-scaler
 * **/
typedef enum{	ADIV_1, 	ADIV_2,	ADIV_4,	ADIV_8 }clockDivider;

/**
 * Enumeration sampleTimeSize that indicates how the ADC will work.
 * **/
typedef enum{	SHORT_SAMPLE,	LONG_SAMPLE}sampleTimeSize;

/**
 * Enumeration conversionMode that indicates how much bits will the ADC give, in the
 * conversion result.
 * When differential, the number of bits is increased by 1, expect for 16 bits.
 * (9 bits, 13 bits, 11 bits, 16 bits) -> Differential options
 * (8 bits, 12 bits, 10 bits, 16 bits) -> Single Ended options
 * **/
typedef enum{	BITS_8,	BITS_12,	BITS_10,	BITS_16}conversionMode;

/**
 * Enumeration inputClockSelect that indicates with which clock the ADC will
 * work
 * **/
typedef enum{	BUS_CLOCK,	ALTCLK2,	ALTCLK1,	ADACK}inputClockSelect;

/**
 * Enumeration conversionTrigger that indicates how the conversion will start.
 * By hardware, a external signal is needed.
 * By software, starts when we write in ADCx_SCF1
 * **/
typedef enum{	SOFTWARE_TRIGGER,	HARDWARE_TRIGGER}conversionTrigger;

/**
 * Enumeration hardwareAverage, enables or disables the hardware average
 * Having hardware average, reduces noise inputs
 * **/
typedef enum{	HW_AVRG_DISABLED,	HW_AVRG_ENABLED}hardwareAverage;

/**
 * Enumeration hardwareAverageSamples, specifies how many samples will be done,
 * in order to have an average and give a conversion result
 * **/
typedef enum{	SAMPLES_4,	SAMPLES_8,	SAMPLES_16,	SAMPLES_32}hardwareAverageSamples;

/**
 * Struct ADC_ConfigType has all the data needed to configure the ADC
 *  * **/
typedef struct{
	/*Specifies which ADC to use: ADC0 or ADC1*/
	ADC_ChannelType xchannel :1;
	/*Specifies channel A or channel B*/
	AB_ChannelType nchannel :1;
	/*Specifies single ended or differential*/
	singleOrDifferential singleOrDifferentialMode :1;
	/*Specifies inputChannel*/
	inputChannelSelect inputChannel;
	/*Specifies normal power or low power*/
	normalOrLowPower powerMode :1;
	/*Specifies clock divider*/
	clockDivider clockDiv :2;
	/*Specifies long or short sample*/
	sampleTimeSize sampleSize :1;
	/*Specifies size of conversion result: 8, 10, 12, or 16 bits*/
	conversionMode conversionSize :2;
	/*specifies clock source*/
	inputClockSelect clockSource :2;
	/*specifies conversion trigger: software or hardware*/
	conversionTrigger converTrigger :1;
	/*specifies if hardware average is enabled or not*/
	hardwareAverage averageEnabled :1;
	/*specifies how many samples will be taken on account*/
	hardwareAverageSamples averageSamples :2;
}ADC_ConfigType;

/**
 * This first set of functions, are for writting to status and control registers, and
 * configuration register of the ADC directly
 */

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function enables the clock gating for the indicated ADC
 	 \param[in] xchannel - ADC to be enabled
 	 \return void
 */
void ADC_clockGating(ADC_ChannelType xchannel);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function writes directly to the ADCx_SC1n, the statusAndControl1 configuration
 	 \param[in] xchannel - ADC channel
 	 \param[in] nchannel - A or B channel
 	 \param[in] statusAndControl1 - configuration to write in the register
 	 \return void
 */
void ADC_statusControlRegister1 (ADC_ChannelType xchannel, AB_ChannelType nchannel, uint32* statusAndControl1);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function writes directly to the ADCx_SC2, the statusAndControl2 configuration
 	 \param[in] xchannel - ADC channel
 	 \param[in] statusAndControl2 - configuration to write in the register
 	 \return void
 */
void ADC_statusControlRegister2 (ADC_ChannelType xchannel, uint32* statusAndControl2);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function writes directly to the ADCx_SC3, the statusAndControl3 configuration
 	 \param[in] xchannel - ADC channel
 	 \param[in] statusAndControl3 - configuration to write in the register
 	 \return void
 */
void ADC_statusControlRegister3 (ADC_ChannelType xchannel, uint32* statusAndControl3);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function writes directly to the ADCx_CFG1, the configuration1 configuration
 	 \param[in] xchannel - ADC channel
 	 \param[in] configuration1 - configuration to write in the register
 	 \return void
 */
void ADC_configurationRegister1 (ADC_ChannelType xchannel, uint32* configuration1);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function writes directly to the ADCx_CFG2, the configuration2 configuration
 	 \param[in] xchannel - ADC channel
 	 \param[in] configuration2 - configuration to write in the register
 	 \return void
 */
void ADC_configurationRegister2 (ADC_ChannelType xchannel, uint32* configuration2);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function reads the result of the conversion in the ADC
 	 \param[in] xchannel - ADC channel
 	 \param[in] nchannel - A or B channel
 	 \return float - ADC convertion result
 */
float ADC_dataResultRegister (ADC_ChannelType xchannel, AB_ChannelType nchannel);


/**
 * This second set of functions, are for initializing the ADC, with different parameters
 */

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function sets the ADC in the specified channel, as single ended or
 	 differential mode
 	 \param[in] xchannel - ADC channel
 	 \param[in] nchannel - A or B channel
 	 \param[in] snglDiff - single ended or differential mode
 	 \return void
 */
void ADC_singleOrDifferential(ADC_ChannelType xchannel, AB_ChannelType nchannel, singleOrDifferential snglDiff);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function sets the ADC in the specified channel, the input channel
 	 \param[in] xchannel - ADC channel
 	 \param[in] nchannel - A or B channel
 	 \param[in] inputChannelSelect - selects the input channel for the ADC
 	 \return void
 */
void ADC_inputChannel(ADC_ChannelType xchannel, AB_ChannelType nchannel, inputChannelSelect inputChannel);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function verifies that the single ended or differential, and input channel
 	 selected, doesn�t lead to a reserved point.
 	 \param[in] snglDiff - checks that snglDiff with inputChannel isn�t reserved
 	 \param[in] inputChannel - checks that snglDiff with inputChannel isn�t reserved
 	 \return uint8
 */
uint8 ADC_diffInputChannelVerify(singleOrDifferential snglDiff, inputChannelSelect inputChannel);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function sets the ADC in the specified power mode
 	 \param[in] xchannel - ADC Channel
 	 \param[in] powerMode - select the powerMode
 	 \return void
 */
void ADC_powerMode(ADC_ChannelType xchannel, normalOrLowPower powerMode);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function sets the ADC in the specified prescaler
 	 \param[in] xchannel - ADC Channel
 	 \param[in] clockDiv - clock pre-scaler
 	 \return void
 */
void ADC_clockDivider(ADC_ChannelType xchannel, clockDivider clockDiv);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function sets the ADC in the specified sampleSize
 	 \param[in] xchannel - ADC Channel
 	 \param[in] sampleSize - sampleSize mode
 	 \return void
 */
void ADC_sampleSize(ADC_ChannelType xchannel, sampleTimeSize sampleSize);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function sets the ADC in the specified convertion size
 	 \param[in] xchannel - ADC Channel
 	 \param[in] conversionSize - size of the conversion
 	 \return void
 */
void ADC_convertionSize(ADC_ChannelType xchannel, conversionMode conversionSize);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function sets the ADC in the specified clock source
 	 \param[in] xchannel - ADC Channel
 	 \param[in] clockSource
 	 \return void
 */
void ADC_clockSource(ADC_ChannelType xchannel, inputClockSelect clockSource);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function sets the ADC in the specified conversion trigger
 	 \param[in] xchannel - ADC Channel
 	 \param[in] converTrigget - conversion trigger
 	 \return void
 */
void ADC_conversionTrigger(ADC_ChannelType xchannel, conversionTrigger converTrigger);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function sets the ADC in the specified hardware average enable
 	 \param[in] xchannel - ADC Channel
 	 \param[in] averageEnabled - hardware average enabled
 	 \return void
 */
void ADC_hardwareAverage(ADC_ChannelType xchannel, hardwareAverage averageEnabled);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function sets the ADC in the specified hardware average samples
 	 \param[in] xchannel - ADC Channel
 	 \param[in] averageSamples - number of samples
 	 \return void
 */
void ADC_hardwareAverageSamples(ADC_ChannelType xchannel, hardwareAverageSamples averageSamples);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function sets the ADC in the initialization values specified in
 	 ADC_Config
 	 \param[in] ADC_config - pointer to struct that has the init values
 	 \return uint8 - return TRUE or FALSE, depending if the initialization was successful, or
 	 not
 */
uint8 ADC_init(const ADC_ConfigType* ADC_Config);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function get the value of the flag of the ADC mailbox
 	 \param[in] ADC_ChannelType - ADC Channel
 	 \return uint8 - return the value of the flag ADC1 or ADC2, depending the value of xchannel
 */
uint8 ADC_mailBoxFlag(ADC_ChannelType xchannel);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function get the value of the data of the ADC mailbox
 	 \param[in] ADC_ChannelType - ADC Channel
 	 \return float - return the value of the data ADC1 or ADC2, depending the value of xchannel
 */
float ADC_mailBoxData(ADC_ChannelType xchannel);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function star the seqential conversion depending of which ADC Channel, AB Channel and
 	 Input Channel is triggered
 	 \param[in] ADC_ChannelType - ADC Channel
 	 \param[in]	AB_ChannelType - AB Channel
 	 \param[in] inputChannelSelect - Input Channel
 	 \return void
*/
void ADC_startConvertion(ADC_ChannelType xchannel, AB_ChannelType nchannel, inputChannelSelect inputChannel);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function star the calibration of the ADC1 o ADC2, depending the value of
 	 xchannel
 	 Input Channel is triggered
 	 \param[in] ADC_ChannelType - ADC Channel
 	 \return uint8 - return the  value ADC_SC3_CALF_MASK depending of the xchannel
*/
uint8 ADC_calibration(ADC_ChannelType xchannel);


#endif /* SOURCES_ADC_H_ */
