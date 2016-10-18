/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "MK64F12.h"
#include "BTTN.h"
#include "ADC.h"
#include "FlexTimer.h"
#include "DataTypeDefinitions.h"
#include "SPI.h"
#include "LCDNokia5110.h"
#include "SYSUPD.h"
#include "DISP.h"

static int i = 0;

/**
 * Constant structure for initiazing the SPI
 * **/
const SPI_ConfigType SPI_Config={
							/*Sets the values or parameters that enables
							 * the SPI with the desired characteristics*/
							SPI_DISABLE_FIFO,
							SPI_LOW_POLARITY,
							SPI_LOW_PHASE,
							SPI_MSB,
							/*Use SPI0*/
							SPI_0,
							/*Set this SPI as MASTER*/
							SPI_MASTER,
							/*Sets the used pin as MUX2*/
							GPIO_MUX2,
							/*Set baudrate and framesize*/
							SPI_BAUD_RATE_2,
							SPI_FSIZE_8,
							/*Sets this pins as the ones that will be used*/
							{GPIOD,BIT1,BIT2}};

/**
 * Constant structure for initiazing the ADC
 * **/
const ADC_ConfigType ADC_Config = {
							/*We will be using ADC9*/
							ADC_0,
							/*And within, channel A*/
							A,
							/*As single ended (Vmax = 3.3V, Vmin = 0V)*/
							SINGLE_ENDED,
							/*Use the first input channel*/
							DAD0,
							/*Low power configuration*/
							LOW_POWER,
							/*prescaler of 8*/
							ADIV_8,
							/*use long samples*/
							LONG_SAMPLE,
							/*convertions are 16 bits widdth*/
							BITS_16,
							/*Input clock, is bus clock*/
							BUS_CLOCK,
							/*Software triggered*/
							SOFTWARE_TRIGGER,
							/*Enable hardware samples, with 32 samples*/
							HW_AVRG_ENABLED,
							SAMPLES_32};

/**
 * Constant structure for initiazing the FTM for ADC convertions counter
 * **/
const FTM_ConfigType ADC_FTM_Config = {
							/*We will be using FTM1, Channel 0*/
							FTM_1,
							CHANNEL_N_0,
							/*Set the configuration for counter mode*/
							FALSE, //DECAPEN
							FALSE, //COMBINE
							FALSE, //CPWMS
							FALSE, //MSnB
							FALSE, //MSnA
							FALSE, //ELSB
							FALSE, //ELSA
							/*Set the MOD value*/
							0xFFFF,
							/*Set the Cnv value*/
							0,
							/*Set clock source and pre scaler*/
							FLEX_TIMER_CLKS_1,
							FLEX_TIMER_PS_128,
							/*Enable overflow interruptions, disable channel interruptions*/
							TRUE,
							FALSE};

/**
 * Constant structure for initiazing the FTM for Input Capture
 * **/
const FTM_ConfigType Input_FTM_Config = {
							/*We will be using FTM2, Channel 1*/
							FTM_2,
							CHANNEL_N_1,
							/*Set the configuration for input capture mode*/
							FALSE, //DECAPEN
							FALSE, //COMBINE
							FALSE, //CPWMS
							FALSE, //MSnB
							FALSE, //MSnA
							FALSE, //ELSB
							TRUE,  //ELSA
							/*Set the MOD value*/
							0,
							/*Set the CnV value*/
							0,
							/*Set clock source and pre scaler*/
							FLEX_TIMER_CLKS_1,
							FLEX_TIMER_PS_1,
							/*Enable overflow interruptions, enable channel interruptions*/
							TRUE,
							TRUE};

/**
 * Constant structure for initiazing the FTM for PWM
 * **/
const FTM_ConfigType PWM_FTM_Config = {
							/*We will be using FTM0, Channel 1*/
							FTM_0,
							CHANNEL_N_1,
							/*Set the configuration for center - alligned PWM mode*/
							FALSE, //DECAPEN
							FALSE, //COMBINE
							TRUE, //CPWMS
							TRUE, //MSnB
							FALSE, //MSnA
							TRUE, //ELSB
							FALSE, //ELSA
							/*Set the MOD value, for 490 Hz*/
							167,
							/*Set the CnV value*/
							0,
							/*Set clock source and pre scaler*/
							FLEX_TIMER_CLKS_1,
							FLEX_TIMER_PS_1,
							/*Disable overflow interruptions, disable channel interruptions*/
							FALSE,
							FALSE};

int main(void)
{

    /* Write your code here */

	/*Initialize SYSUPD, the main state machine*/
	SYSUPD_init();
	/*Initialize BTTN, the receptor of buttons*/
	BTTN_init();
	/*Initialize SPI*/
	SPI_init(&SPI_Config); /*! Configuration function for the LCD port*/
	/*Initialize LCDNokia*/
	LCDNokia_init(); /*! Configuration function for the LCD */

	/*Initialize ADC*/
	ADC_init(&ADC_Config);
	/*Initialize FTM for ADC counter*/
	FTM_init(&ADC_FTM_Config);
	/*Initialize FTM for Input capture*/
	FTM_init(&Input_FTM_Config);
	/*Initialize FTM for PWM counter*/
	FTM_init(&PWM_FTM_Config);



	/*function pointers to get BTTN mailBox flag and Data*/
	uint8 (*button_ready)() = BTTN_mailBoxFlag;
	uint16 (*button_read)() = BTTN_mailBoxData;

	/*function pointers to start convertion of ADC, knowm if the convertion is
	 * completed, and get the conversion result*/
	void (*adc_convertData)(ADC_ChannelType, AB_ChannelType, inputChannelSelect) = ADC_startConvertion;
	uint8 (*adc_mailBoxFlag)(ADC_ChannelType) = ADC_mailBoxFlag;
	float (*adc_mailBoxData)(ADC_ChannelType) = ADC_mailBoxData;

	/*function pointers to get the system flags for display and system*/
	SystemDisplayFlags* (*SystemDisplayingFlags)() = SYSUPD_SDF;
	SystemUpdateFlags* (*SystemWorkingFlags)() = SYSUPD_SUF;

	/*function pointers to get FTM mailBox flag and Data*/
	uint8 (*ftm_mailBoxFlag)(FTM_ChannelType) = FTM_mailBoxFlag;
	uint16 (*ftm_mailBoxData)(FTM_ChannelType) = FTM_readMailBoxData;

	/*function pointer to update the motor speed*/
	void (*PWM_update)(FTM_ChannelType, N_ChannelType, sint16) = FTM_updateCHValue;

	/*function pointer to update the display in the LCD, and to update the main
	 * state machine in SYSUPD*/
	void (*update_display)(SystemDisplayFlags*) = update_Display;
	void (*system_update)(uint16) = SYSUPD_update;

	/*functions pointers to set the temperature and frequency in the system flags struct,
	 * also to verify that the temperature is under the alarm threshold, and to modify
	 * the motor speed according to thetemperature*/
	void (*change_temp)(float) = changeTemperature;
	void (*alarm_check)() = temperatureAlarmCheck;
	void (*motor_check)() = temperatureMotorControl;
	void (*change_Frequency)(uint16, uint16) = changeFrequency;

	/*Enable the interruptions*/
	EnableInterrupts;

	/////////////////////////////////////////////////////////////////////////////////////


	/*Update the display for first time*/
	(*update_display)((*SystemDisplayingFlags)());

	/*First start convertion in the ADC*/
	(*adc_convertData)(ADC_Config.xchannel, ADC_Config.nchannel, ADC_Config.inputChannel);

	for (;;) {

		/*If a button was pressed*/
		if((*button_ready)()){
			/*Update the system state machine according to the button*/
			(*system_update)((*button_read)());
			/*Update the PWM*/
			(*PWM_update)(PWM_FTM_Config.FTM_Channel, PWM_FTM_Config.N_Channel, 0.01*PWM_FTM_Config.MOD*(*SystemWorkingFlags)()->currentSpeed);
			/*Update the screen (it may be not be needed)*/
			(*update_display)((*SystemDisplayingFlags)());

		}

		/*If the ADC counter FTM has overflowed, start a new ADC convertion*/
    	if((*ftm_mailBoxFlag)(ADC_FTM_Config.FTM_Channel)){
    		/*Start a new convertion in the ADC*/
        	(*adc_convertData)(ADC_Config.xchannel, ADC_Config.nchannel, ADC_Config.inputChannel);
        	/*Clear the flag in the mailBox*/
        	(*ftm_mailBoxData)(ADC_FTM_Config.FTM_Channel);
    	}

    	/*If the convertion is completed, get the temperature*/
    	if((*adc_mailBoxFlag)(ADC_Config.xchannel)){
    		/*change the currente temperature with the most recent one*/
    		(*change_temp)((*adc_mailBoxData)(ADC_Config.xchannel));
    		/*Check the alarm threshold and motor conditions*/
    		(*alarm_check)();
    		(*motor_check)();
    		/*Update the PWM*/
    		(*PWM_update)(PWM_FTM_Config.FTM_Channel, PWM_FTM_Config.N_Channel, 0.01*PWM_FTM_Config.MOD*(*SystemWorkingFlags)()->currentSpeed);
    		/*Update the screen (it may not be needed)*/
    		(*update_display)((*SystemDisplayingFlags)());
    	}

    	/*If the Input capture has 2 values, get the frequence*/
    	if((*ftm_mailBoxFlag)(Input_FTM_Config.FTM_Channel)){
    		/*Change the current frequency with the most recent one*/
    		(*change_Frequency)((*ftm_mailBoxData)(Input_FTM_Config.FTM_Channel), (*ftm_mailBoxData)(4));
    		/*Update the screen (it may not be needed)*/
    		(*update_display)((*SystemDisplayingFlags)());
    	}
	}
    /* Never leave main */
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
