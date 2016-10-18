/*
 * ADC.c
 *
 *  Created on: 06/10/2016
 *      Author: Patricio Gomez
 */

#include "ADC.h"
#include "DataTypeDefinitions.h"
#include "MK64F12.h"
#include "NVIC.h"
/*ADC0_MailBox initialization*/
ADC_MailBoxType ADC0_MailBox = {
		FALSE,
		0
};
/*ADC1_MailBox initialization*/
ADC_MailBoxType ADC1_MailBox = {
		FALSE,
		0
};

void ADC0_IRQHandler(){
	/*Verify if the conversion of ADC0 is complete*/
	if(!(ADC0_SC1A & ADC_SC1_COCO_MASK)){
		return;
	}

	/*set the value of the flag*/
	ADC0_MailBox.flag = TRUE;
	/*set the value of the data */
	ADC0_MailBox.MailBoxData = ADC_dataResultRegister(ADC_0,A);

}
/*Set the value for the mailbox flag and mailbox data */
void ADC1_IRQHandler(){
	/*Verify if the conversion of ADC1 is complete*/
	if(!(ADC1_SC1A & ADC_SC1_COCO_MASK)){
		return;
	}
	/*set the value of the flag*/
	ADC1_MailBox.flag = TRUE;
	/*set the value of the data */
	ADC0_MailBox.MailBoxData = ADC_dataResultRegister(ADC_1,A);
}

/*Enable the clock gating for the ADC*/
void ADC_clockGating(ADC_ChannelType xchannel){
	switch(xchannel){
	case ADC_0:
		SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;
		break;

	case ADC_1:
		SIM_SCGC3 |= SIM_SCGC3_ADC1_MASK;
		break;
	}
}

/*modify the ADC channel an AB channel with the value of the pointer statusAndControl1*/
void ADC_statusControlRegister1 (ADC_ChannelType xchannel, AB_ChannelType nchannel, uint32* statusAndControl1){
	switch(xchannel){
	case ADC_0:
		switch(nchannel){
		case A:
			ADC0_SC1A &= ~(*statusAndControl1);
			ADC0_SC1A |= *statusAndControl1;
			break;

		case B:
			ADC0_SC1B &= ~(*statusAndControl1);
			ADC0_SC1B |= *statusAndControl1;
			break;
		}
		break;

	case ADC_1:
		switch(nchannel){
		case A:
			ADC1_SC1A &= ~(*statusAndControl1);
			ADC1_SC1A |= *statusAndControl1;
			break;

		case B:
			ADC1_SC1B &= ~(*statusAndControl1);
			ADC1_SC1B |= *statusAndControl1;
			break;
		}
		break;

	default:
		return;
	}
}
/*modify the ADC channel with the value of the pointer statusAndControl1*/
void ADC_statusControlRegister2 (ADC_ChannelType xchannel, uint32* statusAndControl2){
	switch(xchannel){
	case ADC_0:
		ADC0_SC2 &= ~(*statusAndControl2);
		ADC0_SC2 |= *statusAndControl2;
		break;

	case ADC_1:
		ADC1_SC2 &= ~(*statusAndControl2);
		ADC1_SC2 |= *statusAndControl2;
		break;

	default:
		return;
	}
}
/*modify the ADC channel with the value of the pointer statusAndControl1*/
void ADC_statusControlRegister3 (ADC_ChannelType xchannel, uint32* statusAndControl3){
	switch(xchannel){
	case ADC_0:
		ADC0_SC3 &= ~(*statusAndControl3);
		ADC0_SC3 |= *statusAndControl3;
		break;

	case ADC_1:
		ADC1_SC3 &= ~(*statusAndControl3);
		ADC1_SC3 |= *statusAndControl3;
		break;

	default:
		return;
	}
}

/*modify the ADC channel with the value of the pointer statusAndControl1*/
void ADC_configurationRegister1 (ADC_ChannelType xchannel, uint32* configuration1){
	switch(xchannel){
	case ADC_0:
		ADC0_CFG1 &= ~(*configuration1);
		ADC0_CFG1 |= (*configuration1);
		break;

	case ADC_1:
		ADC1_CFG1 &= ~(*configuration1);
		ADC1_CFG1 |= *configuration1;
		break;

	default:
		return;
	}
}

/*modify the ADC channel with the value of the pointer statusAndControl1*/
void ADC_configurationRegister2 (ADC_ChannelType xchannel, uint32* configuration2){
	switch(xchannel){
	case ADC_0:
		ADC0_CFG2 &= ~(*configuration2);
		ADC0_CFG2 |= (*configuration2);
		break;

	case ADC_1:
		ADC1_CFG2 &= ~(*configuration2);
		ADC1_CFG2 |= *configuration2;
		break;

	default:
		return;
	}
}
/*Return the result of the ADC channel and AB channel conversion*/
float ADC_dataResultRegister (ADC_ChannelType xchannel, AB_ChannelType nchannel){
	switch(xchannel){
	case ADC_0:

		switch(nchannel){
		case A:

			return ADC0_RA;

			break;

		case B:

			return ADC0_RB;

			break;
		}

		break;

	case ADC_1:

		switch(nchannel){
		case A:

			return ADC1_RA;

			break;

		case B:

			return ADC1_RB;

			break;
		}

		break;
	}
}

/*Enables the differential mode*/
void ADC_singleOrDifferential(ADC_ChannelType xchannel, AB_ChannelType nchannel, singleOrDifferential snglDiff){
	switch(xchannel){
	case ADC_0:
		switch(nchannel){
		case A:
			ADC0_SC1A & ~(ADC_SC1_DIFF_MASK);
			ADC0_SC1A |= ADC_SC1_DIFF(snglDiff);
			break;

		case B:
			ADC0_SC1B & ~(ADC_SC1_DIFF_MASK);
			ADC0_SC1B |= ADC_SC1_DIFF(snglDiff);
			break;
		}
		break;

	case ADC_1:
		switch(nchannel){
		case A:
			ADC1_SC1A & ~(ADC_SC1_DIFF_MASK);
			ADC1_SC1A |= ADC_SC1_DIFF(snglDiff);
			break;

		case B:
			ADC1_SC1B & ~(ADC_SC1_DIFF_MASK);
			ADC1_SC1B |= ADC_SC1_DIFF(snglDiff);
			break;
		}
		break;

	default:
		return;
	}
}
/*Selects one of the input channels from ADC channel and AB Channel*/
void ADC_inputChannel(ADC_ChannelType xchannel, AB_ChannelType nchannel, inputChannelSelect inputChannel){
	switch(xchannel){
		case ADC_0:
			switch(nchannel){
			case A:
				ADC0_SC1A & ~(ADC_SC1_ADCH_MASK);
				ADC0_SC1A |= ADC_SC1_ADCH(inputChannel);
				break;

			case B:
				ADC0_SC1B & ~(ADC_SC1_ADCH_MASK);
				ADC0_SC1B |= ADC_SC1_ADCH(inputChannel);
				break;
			}
			break;

		case ADC_1:
			switch(nchannel){
			case A:
				ADC1_SC1A & ~(ADC_SC1_ADCH_MASK);
				ADC1_SC1A |= ADC_SC1_ADCH(inputChannel);
				break;

			case B:
				ADC1_SC1B & ~(ADC_SC1_ADCH_MASK);
				ADC1_SC1B |= ADC_SC1_ADCH(inputChannel);
				break;
			}
			break;

		default:
			return;
		}
}
/*Return false if any of the conditions below are true or true if the conditions are false*/
uint8 ADC_diffInputChannelVerify(singleOrDifferential snglDiff, inputChannelSelect inputChannel){
	/*Verify if the the diffetential mode is activated and the range of the input channel */
	if(snglDiff = DIFFERENTIAL && ((inputChannel >= AD4 && inputChannel <= AD23)||(inputChannel == VREFSH_SINGLE))){
		return FALSE;
	}
	/*Verify the value of the input channel*/
	if(inputChannel == 24 || inputChannel == 25 || inputChannel == 28){
		return FALSE;
	}
	return TRUE;
}

/*Enables the low power configuration*/
void ADC_powerMode(ADC_ChannelType xchannel, normalOrLowPower powerMode){
	switch(xchannel){
	case ADC_0:
		ADC0_CFG1 &= ~ADC_CFG1_ADLPC_MASK;
		ADC0_CFG1 |= ADC_CFG1_ADLPC(powerMode);
		break;
	case ADC_1:
		ADC1_CFG1 &= ~ADC_CFG1_ADLPC_MASK;
		ADC1_CFG1 |= ADC_CFG1_ADLPC(powerMode);
		break;
	default:
		break;
	}
}

/*Selects the divide ratio used by the ADC to generate the internal clock ADCK.*/
void ADC_clockDivider(ADC_ChannelType xchannel, clockDivider clockDiv){
	switch(xchannel){
	case ADC_0:
		ADC0_CFG1 &= ~ADC_CFG1_ADIV_MASK;
		ADC0_CFG1 |= ADC_CFG1_ADIV(clockDiv);
		break;
	case ADC_1:
		ADC1_CFG1 &= ~ADC_CFG1_ADIV_MASK;
		ADC1_CFG1 |= ADC_CFG1_ADIV(clockDiv);
		break;
	default:
		break;
	}
}

/*Selects between different sample times based on the conversion mode selected*/
void ADC_sampleSize(ADC_ChannelType xchannel, sampleTimeSize sampleSize){
	switch(xchannel){
	case ADC_0:
		ADC0_CFG1 &= ~ADC_CFG1_ADLSMP_MASK;
		ADC0_CFG1 |= ADC_CFG1_ADLSMP(sampleSize);
		break;
	case ADC_1:
		ADC1_CFG1 &= ~ADC_CFG1_ADLSMP_MASK;
		ADC1_CFG1 |= ADC_CFG1_ADLSMP(sampleSize);
		break;
	default:
		break;
	}
}
/*Selects the ADC resolution mode*/
void ADC_convertionSize(ADC_ChannelType xchannel, conversionMode conversionSize){
	switch(xchannel){
	case ADC_0:
		ADC0_CFG1 &= ~ADC_CFG1_MODE_MASK;
		ADC0_CFG1 |= ADC_CFG1_MODE(conversionSize);
		break;
	case ADC_1:
		ADC1_CFG1 &= ~ADC_CFG1_MODE_MASK;
		ADC1_CFG1 |= ADC_CFG1_MODE(conversionSize);
		break;
	default:
		break;
	}
}
/*Selects the input clock source to generate the internal clock*/
void ADC_clockSource(ADC_ChannelType xchannel, inputClockSelect clockSource){
	switch(xchannel){
	case ADC_0:
		ADC0_CFG1 &= ~ADC_CFG1_ADICLK_MASK;
		ADC0_CFG1 |= ADC_CFG1_ADICLK(clockSource);
		break;
	case ADC_1:
		ADC1_CFG1 &= ~ADC_CFG1_ADICLK_MASK;
		ADC1_CFG1 |= ADC_CFG1_ADICLK(clockSource);
		break;
	default:
		break;
	}
}

/*Selects the type of trigger used for initiating a conversion*/
void ADC_conversionTrigger(ADC_ChannelType xchannel, conversionTrigger converTrigger){
	switch(xchannel){
	case ADC_0:
		ADC0_SC2 &= ~ADC_SC2_ADTRG_MASK;
		ADC0_SC2 |= ADC_SC2_ADTRG(converTrigger);
		break;
	case ADC_1:
		ADC1_SC2 &= ~ADC_SC2_ADTRG_MASK;
		ADC1_SC2 |= ADC_SC2_ADTRG(converTrigger);
		break;
	default:
		break;
	}
}
/*Enables continuous conversions*/
void ADC_hardwareAverage(ADC_ChannelType xchannel, hardwareAverage averageEnabled){
	switch(xchannel){
	case ADC_0:
		ADC0_SC3 &= ~ADC_SC3_AVGE_MASK;
		ADC0_SC3 |= ADC_SC3_AVGE(averageEnabled);
		break;
	case ADC_1:
		ADC1_SC3 &= ~ADC_SC3_AVGE_MASK;
		ADC1_SC3 |= ADC_SC3_AVGE(averageEnabled);
		break;
	default:
		break;
	}
}
/*Clear Hardware Average Select field and determines how many ADC conversions will be averaged to create the ADC average result.*/
void ADC_hardwareAverageSamples(ADC_ChannelType xchannel, hardwareAverageSamples averageSamples){
	switch(xchannel){
	case ADC_0:
		ADC0_SC3 &= ~ADC_SC3_AVGS_MASK;
		ADC0_SC3 |= ADC_SC3_AVGS(averageSamples);
		break;
	case ADC_1:
		ADC1_SC3 &= ~ADC_SC3_AVGS_MASK;
		ADC1_SC3 |= ADC_SC3_AVGS(averageSamples);
		break;
	default:
		break;
	}
}

/*Return the value of the flag of the mailbox */
uint8 ADC_mailBoxFlag(ADC_ChannelType xchannel){
	switch(xchannel){
	case ADC_0:
		return ADC0_MailBox.flag;

	case ADC_1:
		return ADC1_MailBox.flag;
	}
}

/*Return the value of the Data of the mailbox */
float ADC_mailBoxData(ADC_ChannelType xchannel){
	switch(xchannel){
	case ADC_0:
		ADC0_MailBox.flag = FALSE;
		return ADC0_MailBox.MailBoxData;

	case ADC_1:
		ADC1_MailBox.flag = FALSE;
		return ADC1_MailBox.MailBoxData;
	}
}
/*Start the conversion of the ADC and activate the interruption*/
void ADC_startConvertion(ADC_ChannelType xchannel, AB_ChannelType nchannel, inputChannelSelect inputChannel){
	switch(xchannel){
	case ADC_0:
		switch(nchannel){
		case A:
			ADC0_SC1A = (inputChannel & ADC_SC1_ADCH_MASK) |
							(ADC0_SC1A & (ADC_SC1_AIEN_MASK));
			break;

		case B:
			ADC0_SC1B = (inputChannel & ADC_SC1_ADCH_MASK) |
							(ADC0_SC1B & (ADC_SC1_AIEN_MASK));
			break;
		}
		break;

	case ADC_1:
		switch(nchannel){
		case A:
			ADC1_SC1A = (inputChannel & ADC_SC1_ADCH_MASK) |
							(ADC1_SC1A & (ADC_SC1_AIEN_MASK));
			break;

		case B:
			ADC1_SC1B = (inputChannel & ADC_SC1_ADCH_MASK) |
							(ADC1_SC1B & (ADC_SC1_AIEN_MASK));

			break;
		}
		break;

	default:
		return;
	}

}
/*Verify if the calibration of ADC Channel failed */
uint8 ADC_calibration(ADC_ChannelType xchannel){
	switch(xchannel){
	case ADC_0:
		ADC0_SC3 |= ADC_SC3_CAL_MASK;
		while(ADC0_SC3 & ADC_SC3_CAL_MASK);
		return ~(ADC0_SC3 & ADC_SC3_CALF_MASK);

	case ADC_1:
		ADC1_SC3 |= ADC_SC3_CAL_MASK;
		while(ADC1_SC3 & ADC_SC3_CAL_MASK);
		return ~(ADC1_SC3 & ADC_SC3_CALF_MASK);

	default:
		return FALSE;
	}
}

/*Initialize the ADC whit the value of ADC_Config*/
uint8 ADC_init(const ADC_ConfigType* ADC_Config){
	/*Verify the configuration of differential mode and the input channel*/
	if(ADC_diffInputChannelVerify(ADC_Config->singleOrDifferentialMode, ADC_Config->inputChannel) == FALSE){
		return FALSE;
	}

	ADC_clockGating(ADC_Config->xchannel);
	/*Verify the the result of the adc calibration */
	if(ADC_calibration(ADC_Config->xchannel) == FALSE){
		return FALSE;
	}

	uint32 statusAndControl1IE = ADC_SC1_AIEN_MASK;
	/*set the ADC channel, AB channel and the interrupt enable mask */
	ADC_statusControlRegister1(ADC_Config->xchannel, ADC_Config->nchannel, &statusAndControl1IE);
	/*set the ADC channel, AB channel and the differential mode mask */
	ADC_singleOrDifferential(ADC_Config->xchannel, ADC_Config->nchannel, ADC_Config->singleOrDifferentialMode);
	/*set the ADC channel, AB channel and the input channel */
	ADC_inputChannel(ADC_Config->xchannel, ADC_Config->nchannel, ADC_Config->inputChannel);
	/*set the ADC channel and low power configuration mask */
	ADC_powerMode(ADC_Config->xchannel, ADC_Config->powerMode);
	/*set the ADC channel and the interrupt the divide ratio */
	ADC_clockDivider(ADC_Config->xchannel, ADC_Config->clockDiv);
	/*set the ADC channel and the sample size */
	ADC_sampleSize(ADC_Config->xchannel, ADC_Config->sampleSize);
	/*set the ADC channel and the resolution of the ADC */
	ADC_convertionSize(ADC_Config->xchannel, ADC_Config->conversionSize);
	/*set the ADC channel and the input clock source */
	ADC_clockSource(ADC_Config->xchannel, ADC_Config->clockSource);
	/*set the ADC channel and trigger to star the conversion */
	ADC_conversionTrigger(ADC_Config->xchannel, ADC_Config->converTrigger);
	/*set the ADC channel and average conversions */
	ADC_hardwareAverage(ADC_Config->xchannel, ADC_Config->averageEnabled);
	/*set the ADC channel and determine the number of conversions*/
	ADC_hardwareAverageSamples(ADC_Config->xchannel, ADC_Config->averageSamples);

	/*set the interruption of ADC0 and priority */
	NVIC_enableInterruptAndPriority(ADC0_IRQ, PRIORITY_10);
	/*set the interruption of ADC1 and priority */
	NVIC_enableInterruptAndPriority(ADC1_IRQ, PRIORITY_10);

	/*Enables the interruptions */
	EnableInterrupts;

	return TRUE;
}
