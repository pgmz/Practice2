/*
 * FlexTimer.h
 *
 *  Created on: Oct 3, 2014
 *      Author: Luis
 */

#ifndef FLEXTIMER_H_
#define FLEXTIMER_H_

#include "MK64F12.h"
#include "DataTypeDefinitions.h"
#include "GPIO.h"

/**
 * Enumeration WP_EnableType that indicates if Write Protection is
 * enabled or disabled
 * **/
typedef enum {WP_DISABLE, WP_ENABLE} WP_EnableType;

/**
 * Enumeration N_ChannelType that indicates which channel (in the FlexTimerX) is
 * going to be used
 * **/
typedef enum{CHANNEL_N_0,
			CHANNEL_N_1,
			CHANNEL_N_2,
			CHANNEL_N_3,
			CHANNEL_N_4,
			CHANNEL_N_5,
			CHANNEL_N_6,
			CHANNEL_N_7
			}N_ChannelType;

/**
 * Enumeration FTM_ChannelType that indicates which FlexTimer is going to be used
 * **/
typedef enum{FTM_0,
			FTM_1,
			FTM_2,
			FTM_3
			}FTM_ChannelType;

/**
 * Struct FTM_Config, has all the needed members to initialize a Flex timer
 * **/
typedef struct{
	/*FlexTimer that will be used*/
	FTM_ChannelType FTM_Channel;
	/*N channel in the flex timer to be used*/
	N_ChannelType N_Channel;
	/*Decapen value (For mode selection)*/
	uint8 DECAPEN :1;
	/*Combine value (For mode selection)*/
	uint8 COMBINE :1;
	/*CPWMS value (For mode selection)*/
	uint8 CPWMS :1;
	/*MSnB value (For mode selection)*/
	uint8 MSnB :1;
	/*MSnA value (For mode selection)*/
	uint8 MSnA :1;
	/*ELSB value (For mode selection)*/
	uint8 ELSB :1;
	/*ELSA value (For mode selection)*/
	uint8 ELSA :1;
	/*MOD values, is the limit for the Flextimer to count*/
	uint16 MOD;
	/*CNV channel value, if used, and depending on the mode, is the duty cycle of the
	 * PWM*/
	uint16 CNV;
	/*clockSource for the flexTimer (usually Bus Clock)*/
	uint8 clockSource;
	/*clockPrescaler for the flexTimer (usually 1 or 128)*/
	uint8 clockPrescaler;
	/*Indicates if interruptions should be enabled, this is because in certain modes,
	 * there is no need to attend interruptions. Example: PWM*/
	uint8 interrupt :1;
	/*Indicates if channel interruptions should be enabled, this is because in certain
	 * modes, there is no need to attend individual channel interruptions. Example: Counter*/
	uint8 channelInterrup :1;
}FTM_ConfigType;

/**
 * Struct FTM_MailBoxType has the information needed to attend an interruption,
 * the flag that indicates if an interruption has occurred, and a MailBoxData (sometimes
 * isn't needed)
 * **/
typedef struct{
	/*flag that takes the values of TRUE or FALSE, to indicate if an interruption
	 * has occured*/
	uint8 flag :1;
	/*received data (sometimes not needed). When the data is readed, the flag is set to
	 * FALSE*/
	uint16 MailBoxData;
}FTM_MailBoxType;

/*defines for enabling clock gating for different flex timers*/
#define FTM0_CLOCK_GATING 0x01000000
#define FTM1_CLOCK_GATING 0x02000000
#define FTM2_CLOCK_GATING 0x04000000
#define FTM3_CLOCK_GATING 0x02000000

/*defines for different flex timer configuration parameters*/
#define FLEX_TIMER_FAULTIE  0x80
#define FLEX_TIMER_FAULTM_0   0x00
#define FLEX_TIMER_FAULTM_1   0x20
#define FLEX_TIMER_FAULTM_2   0x40
#define FLEX_TIMER_FAULTM_3   0x60
#define FLEX_TIMER_CAPTEST  0x10
#define FLEX_TIMER_PWMSYNC  0x08
#define FLEX_TIMER_WPDIS    0x04
#define FLEX_TIMER_INIT     0x02
#define FLEX_TIMER_FTMEN    0x01

/*defines for different flex timer configuration parameters*/
#define FLEX_TIMER_TOF     0x80
#define FLEX_TIMER_TOIE    0x40
#define FLEX_TIMER_CPWMS   0x20
#define FLEX_TIMER_CLKS_0  0x00
#define FLEX_TIMER_CLKS_1  0x08
#define FLEX_TIMER_CLKS_2  0x10
#define FLEX_TIMER_CLKS_3  0x18
#define FLEX_TIMER_PS_1    0x00
#define FLEX_TIMER_PS_2    0x01
#define FLEX_TIMER_PS_4    0x02
#define FLEX_TIMER_PS_8    0x03
#define FLEX_TIMER_PS_16    0x04
#define FLEX_TIMER_PS_32    0x05
#define FLEX_TIMER_PS_64    0x06
#define FLEX_TIMER_PS_128    0x07

/*defines for different flex timer configuration parameters*/
#define FLEX_TIMER_PWMLOAD_CH0 0x01
#define FLEX_TIMER_PWMLOAD_CH1 0x02
#define FLEX_TIMER_PWMLOAD_CH2 0x04
#define FLEX_TIMER_PWMLOAD_CH3 0x08
#define FLEX_TIMER_PWMLOAD_CH4 0x10
#define FLEX_TIMER_PWMLOAD_CH5 0x20
#define FLEX_TIMER_PWMLOAD_CH6 0x40
#define FLEX_TIMER_PWMLOAD_CH7 0x80
#define FLEX_TIMER_LDOK        0x200

/*defines for different flex timer configuration parameters*/
#define  FLEX_TIMER_DMA   0x01
#define  FLEX_TIMER_ELSA  0x04
#define  FLEX_TIMER_ELSB  0x08
#define  FLEX_TIMER_MSA   0x10
#define  FLEX_TIMER_MSB   0x20
#define  FLEX_TIMER_CHIE  0x40
#define  FLEX_TIMER_CHF   0x80

/**
 * This first set of functions, are for writting to configuration or control registers
 * for a defined Flex timer and channel
 */

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function enables the clock gating for the indicated Flex timer
 	 \param[in] channel - Flex timer to be enabled
 	 \return void
 */
void FTM_clockGating(FTM_ChannelType channel);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function disables the writing protection for a indicated flex timer
 	 \param[in] channel - Flex timer that will have the writing protection disabled
 	 \return void
 */
void FTM_WPDIS(FTM_ChannelType channel);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function writes directly to the MOD register in a Flex timer, to
 	 indicate the MOD value
 	 \param[in] channel - Flex timer where to write MOD
 	 \param[in] MOD - Max counter value
 	 \return void
 */
void FTM_MOD(FTM_ChannelType channel, uint16 MOD);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function reads the count value for a flex timer
 	 \param[in] channel - Flex timer where to read the CNT
 	 \return uint32 FTMx_CNT - counter value
 */
uint32 FTM_CNT(FTM_ChannelType channel);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function writes directly to the FTMx_SC in a Flex timer indicated
 	 \param[in] channel - Flex timer where to write FTM_SC
 	 \param[in] statusAndControl - pointer of the desired configuration
 	 \return void
 */
void FTM_SC(FTM_ChannelType channel, uint32* statusAndControl);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function writes directly to the FTMx_CnSC in a Flex timer and a channel
 	 indicated
 	 \param[in] channel - Flex timer where to write
 	 \param[in] n_channel - channel where to write
 	 \param[in] channelStatusAndControl - pointer of the desired configuration
 	 \return void
 */
void FTM_CSC(FTM_ChannelType channel, N_ChannelType n_channel, uint32* channelStatusAndControl);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function writes directly to the FTMx_CnV in a Flex timer indicated
 	 indicated
 	 \param[in] channel - Flex timer where to write
 	 \param[in] n_channel - channel where to write
 	 \param[in] channelValue - CnV value to write
 	 \return void
 */
void FTM_updateCHValue(FTM_ChannelType channel, N_ChannelType n_channel, sint16 channelValue);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function reads the FTMx_CnV in a Flex timer and a channel
 	 indicated
 	 \param[in] channel - Flex timer where to read
 	 \param[in] n_channel - channel where to read
 	 \return sint16 - FTMx_CnV
 */
sint16 FTM_readCHValue(FTM_ChannelType channel, N_ChannelType n_channel);

/**
 * This first set of functions, are for the initialization that we will handle in this
 * pratice
 */

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function reads the mailBox flag of a Flex timer, to indicate if an
 	 interruption has occured in that channel.
 	 indicated
 	 \param[in] channel - Flex timer where to read
 	 \return uint8 - flag that indicates if an interruption has occured or not
 */
uint8 FTM_mailBoxFlag(FTM_ChannelType channel);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function reads the mailBox Data of a Flex timer, and erases the flag
 	 in the mailBox that indicates if an interruption has occurred.
 	 indicated
 	 \param[in] channel - Flex timer where to read
 	 \return uint16 - Data saved when an interruption occurred
 */
uint16 FTM_readMailBoxData(FTM_ChannelType channel);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function enables the NVIC interruption for a Flex timer
 	 \param[in] channel - Flex timer to enable the interruption
 	 \return void
 */
void FTM_IRQEnable(FTM_ChannelType channel);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function initializes a Flex timer, according to the values in the
 	 FTM_Config struct
 	 \param[in] FTM_Config - pointer to a struct that contains everything related to
 	 the parameters for initializating the flex timer
 	 \return uint8 - returns TRUE or FALSE, to indicate if the initialization was
 	 succesful or not
 */
uint8 FTM_init(const FTM_ConfigType* FTM_Config);

#endif /* FLEXTIMER_H_ */
