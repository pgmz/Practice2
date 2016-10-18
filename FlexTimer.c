/**
	\file 
	\brief 
		This is the starter file of FlexTimer. 
		In this file the FlexTimer is configured in overflow mode. 
	\author J. Luis Pizano Escalante, luispizano@iteso.mx
	\date	7/09/2014
	\todo
	    Add configuration structures.
 */


#include "FlexTimer.h"
#include "NVIC.h"
#include "DataTypeDefinitions.h"
#include "GlobalFunctions.h"

/*uint8 testNumber, that indicates which test has been taken in the
 * input capture mode*/
static uint8 testNumber = 0;

/*FTM0_Mailbox initialization*/
FTM_MailBoxType FTM0_MailBox = {
		FALSE,
		0
};

/*FTM2_Mailbox2 initialization*/
FTM_MailBoxType FTM2_MailBox2 = {
		FALSE,
		0
};

/*FTM1_Mailbox initialization*/
FTM_MailBoxType FTM1_MailBox = {
		FALSE,
		0
};

/*FTM2_Mailbox initialization*/
FTM_MailBoxType FTM2_MailBox = {
		FALSE,
		0
};

/*FTM3_Mailbox initialization*/
FTM_MailBoxType FTM3_MailBox = {
		FALSE,
		0
};


void FTM0_IRQHandler()
{
	/**Clearing the overflow interrupt flag*/
	FTM0_SC &= ~FLEX_TIMER_TOF;
	FTM0_MailBox.flag = TRUE;
}

void FTM1_IRQHandler()
{
	/**Clearing the overflow interrupt flag*/
	FTM1_SC &= ~FLEX_TIMER_TOF;

	FTM1_MailBox.flag = TRUE;
}

void FTM2_IRQHandler(){
	/**Clearing the overflow interrupt flag*/
	FTM2_SC &= ~FLEX_TIMER_TOF;

	/*Making sure that the interruption is because of the channel 1 in the flex timer 2*/
	if(!(FTM2_C1SC & FLEX_TIMER_CHF)){
		return;
	}

	/**Clearing the channel interrupt flag*/
	FTM2_C1SC &= ~FLEX_TIMER_CHF;

	/*Clear FTM2 status*/
	FTM2_STATUS = 0;

	/*If test number is 1, the CnV is ignored*/
	if(testNumber == 0){

		/*get next test number*/
		testNumber = 1;
		return;

	/*If test number is 1, we get the first CnV value, that is time1*/
	} else if(testNumber == 1){
		FTM2_MailBox.MailBoxData = FTM_readCHValue(FTM_2, CHANNEL_N_1);

		/*get next test number*/
		testNumber = 2;
		return;

		/*If test number is 2, we get the second CnV value, that is time2*/
	} else {

		/*Set the flag, to indicate that we have 2 values*/
		FTM2_MailBox.flag = TRUE;
		FTM2_MailBox2.MailBoxData = FTM_readCHValue(FTM_2, CHANNEL_N_1);
		testNumber = 0;
	}
}

void FTM3_IRQHandler()
{
	/**Clearing the overflow interrupt flag*/
	FTM2_SC &= ~FLEX_TIMER_TOF;
	FTM3_MailBox.flag = TRUE;
}

/*Enable the clock gating according the Flex timer*/
void FTM_clockGating(FTM_ChannelType channel){
	switch(channel){
	case FTM_0:
		SIM_SCGC6  |= FTM0_CLOCK_GATING;
		break;

	case FTM_1:
		SIM_SCGC6  |= FTM1_CLOCK_GATING;
		break;

	case FTM_2:
		SIM_SCGC6  |= FTM2_CLOCK_GATING;
		break;

	case FTM_3:
		SIM_SCGC3  |= FTM3_CLOCK_GATING;
		break;
	}
}

/*Disable protection disable according to the Flex timer*/
void FTM_WPDIS(FTM_ChannelType channel){
	switch(channel){
	case FTM_0:
		FTM0_MODE = FTM_MODE_WPDIS_MASK;

		break;

	case FTM_1:
		FTM1_MODE = (FTM_MODE_WPDIS_MASK);

		break;

	case FTM_2:
		FTM2_MODE = (FTM_MODE_WPDIS_MASK);

		break;

	case FTM_3:
		FTM3_MODE = (FTM_MODE_WPDIS_MASK);

		break;
	}
}

/*Write in the COMBINE register, according to the flex timer and combine config value*/
void FTM_COMBINE(FTM_ChannelType channel, uint32* combineConfig){
	switch(channel){
	case FTM_0:
		FTM0_MODE &= ~(*combineConfig);
		FTM0_MODE |= *combineConfig;
		break;

	case FTM_1:
		FTM1_MODE &= ~(*combineConfig);
		FTM1_MODE |= *combineConfig;
		break;

	case FTM_2:
		FTM2_MODE &= ~(*combineConfig);
		FTM2_MODE |= *combineConfig;
		break;

	case FTM_3:
		FTM3_MODE &= ~(*combineConfig);
		FTM3_MODE |= *combineConfig;
		break;
	}
}

/*Write in the MOD register, according to the flex timer and MOD value*/
void FTM_MOD(FTM_ChannelType channel, uint16 MOD){
	switch(channel){
	case FTM_0:
		FTM0_MOD = MOD;
		break;

	case FTM_1:
		FTM1_MOD = MOD;
		break;

	case FTM_2:
		FTM2_MOD = MOD;
		break;

	case FTM_3:
		FTM3_MOD = MOD;
		break;

	}
}

/*Read the CNT register according to flex timer*/
uint32 FTM_CNT(FTM_ChannelType channel){
	switch(channel){
	case FTM_0:
		return FTM0_CNT;

	case FTM_1:
		return FTM1_CNT;

	case FTM_2:
		return FTM2_CNT;


	case FTM_3:
		return FTM3_CNT;

	default:
		return FALSE;
	}
}

/*Write in the SC register, according to the flex timer and sc config value*/
void FTM_SC(FTM_ChannelType channel, uint32* statusAndControl){
	switch(channel){
	case FTM_0:
		FTM0_SC &= ~(*statusAndControl);
		FTM0_SC |= *statusAndControl;
		break;

	case FTM_1:
		FTM1_SC &= ~(*statusAndControl);
		FTM1_SC |= *statusAndControl;
		break;

	case FTM_2:
		FTM2_SC &= ~(*statusAndControl);
		FTM2_SC |= *statusAndControl;
		break;

	case FTM_3:
		FTM3_SC &= ~(*statusAndControl);
		FTM3_SC |= *statusAndControl;
		break;
	}
}

/*Write in the CSC register, according to the flex timer, channel and config value*/
void FTM_CSC(FTM_ChannelType channel, N_ChannelType n_channel, uint32* channelStatusAndControl){
	switch(channel){
	case FTM_0:

		switch(n_channel){
		case CHANNEL_N_0:

			FTM0_C0SC &= ~(*channelStatusAndControl);
			FTM0_C0SC |= *channelStatusAndControl;

			break;

		case CHANNEL_N_1:

			FTM0_C1SC &= ~(*channelStatusAndControl);
			FTM0_C1SC |= *channelStatusAndControl;

			break;

		case CHANNEL_N_2:

			FTM0_C2SC &= ~(*channelStatusAndControl);
			FTM0_C2SC |= *channelStatusAndControl;
			break;

		case CHANNEL_N_3:

			FTM0_C3SC &= ~(*channelStatusAndControl);
			FTM0_C3SC |= *channelStatusAndControl;
			break;

		case CHANNEL_N_4:

			FTM0_C4SC &= ~(*channelStatusAndControl);
			FTM0_C4SC |= *channelStatusAndControl;
			break;

		case CHANNEL_N_5:

			FTM0_C5SC &= ~(*channelStatusAndControl);
			FTM0_C5SC |= *channelStatusAndControl;
			break;

		case CHANNEL_N_6:

			FTM0_C6SC &= ~(*channelStatusAndControl);
			FTM0_C6SC |= *channelStatusAndControl;
			break;

		case CHANNEL_N_7:

			FTM0_C7SC &= ~(*channelStatusAndControl);
			FTM0_C7SC |= *channelStatusAndControl;
			break;

		}

		break;

	case FTM_1:

		switch(n_channel){
		case CHANNEL_N_0:

			FTM1_C0SC &= ~(*channelStatusAndControl);
			FTM1_C0SC |= *channelStatusAndControl;
			break;

		case CHANNEL_N_1:

			FTM1_C1SC &= ~(*channelStatusAndControl);
			FTM1_C1SC |= *channelStatusAndControl;
			break;

		}

		break;

	case FTM_2:

		switch(n_channel){
		case CHANNEL_N_0:

			FTM2_C0SC &= ~(*channelStatusAndControl);
			FTM2_C0SC |= *channelStatusAndControl;
			break;

		case CHANNEL_N_1:

			FTM2_C1SC &= ~(*channelStatusAndControl);
			FTM2_C1SC |= *channelStatusAndControl;
			break;

		}

		break;

	case FTM_3:

		switch(n_channel){
		case CHANNEL_N_0:

			FTM3_C0SC &= ~(*channelStatusAndControl);
			FTM3_C0SC |= *channelStatusAndControl;
			break;

		case CHANNEL_N_1:

			FTM3_C1SC &= ~(*channelStatusAndControl);
			FTM3_C1SC |= *channelStatusAndControl;
			break;

		case CHANNEL_N_2:

			FTM3_C2SC &= ~(*channelStatusAndControl);
			FTM3_C2SC |= *channelStatusAndControl;
			break;

		case CHANNEL_N_3:

			FTM3_C3SC &= ~(*channelStatusAndControl);
			FTM3_C3SC |= *channelStatusAndControl;
			break;

		case CHANNEL_N_4:

			FTM3_C4SC &= ~(*channelStatusAndControl);
			FTM3_C4SC |= *channelStatusAndControl;
			break;

		case CHANNEL_N_5:

			FTM3_C5SC &= ~(*channelStatusAndControl);
			FTM3_C5SC |= *channelStatusAndControl;
			break;

		case CHANNEL_N_6:

			FTM3_C6SC &= ~(*channelStatusAndControl);
			FTM3_C6SC |= *channelStatusAndControl;
			break;

		case CHANNEL_N_7:

			FTM3_C7SC &= ~(*channelStatusAndControl);
			FTM3_C7SC |= *channelStatusAndControl;
			break;

		}

		break;
	}
}

void FTM_updateCHValue(FTM_ChannelType channel, N_ChannelType n_channel, sint16 channelValue){
	/**Assigns a new value for the duty cycle*/
	switch(channel){
	case FTM_0:

		switch(n_channel){
		case CHANNEL_N_0:

			FTM0_C0V = channelValue;

			break;

		case CHANNEL_N_1:

			FTM0_C1V = channelValue;

			break;

		case CHANNEL_N_2:

			FTM0_C2V = channelValue;

			break;

		case CHANNEL_N_3:

			FTM0_C3V = channelValue;

			break;

		case CHANNEL_N_4:

			FTM0_C4V = channelValue;

			break;

		case CHANNEL_N_5:

			FTM0_C5V = channelValue;

			break;

		case CHANNEL_N_6:

			FTM0_C6V = channelValue;

			break;

		case CHANNEL_N_7:

			FTM0_C7V = channelValue;

			break;

		}

		break;

	case FTM_1:

		switch(n_channel){
		case CHANNEL_N_0:

			FTM1_C0V = channelValue;

			break;

		case CHANNEL_N_1:

			FTM1_C1V = channelValue;

			break;

		}

		break;

	case FTM_2:

		switch(n_channel){
		case CHANNEL_N_0:

			FTM2_C0V = channelValue;

			break;

		case CHANNEL_N_1:

			FTM2_C1V = channelValue;

			break;

		}

		break;

	case FTM_3:

		switch(n_channel){
		case CHANNEL_N_0:

			FTM3_C0V = channelValue;

			break;

		case CHANNEL_N_1:

			FTM3_C1V = channelValue;

			break;

		case CHANNEL_N_2:

			FTM3_C2V = channelValue;

			break;

		case CHANNEL_N_3:

			FTM3_C3V = channelValue;

			break;

		case CHANNEL_N_4:

			FTM3_C4V = channelValue;

			break;

		case CHANNEL_N_5:

			FTM3_C5V = channelValue;

			break;

		case CHANNEL_N_6:

			FTM3_C6V = channelValue;

			break;

		case CHANNEL_N_7:

			FTM3_C7V = channelValue;

			break;

		}

		break;
	}

}

/*Read in the CnV register, according to the flex timer and channel*/
sint16 FTM_readCHValue(FTM_ChannelType channel, N_ChannelType n_channel){
	/**Assigns a new value for the duty cycle*/
	switch(channel){
	case FTM_0:

		switch(n_channel){
		case CHANNEL_N_0:

			return FTM0_C0V;

			break;

		case CHANNEL_N_1:

			return FTM0_C1V;

			break;

		case CHANNEL_N_2:

			return FTM0_C2V;

			break;

		case CHANNEL_N_3:

			return FTM0_C3V;

			break;

		case CHANNEL_N_4:

			return FTM0_C4V;

			break;

		case CHANNEL_N_5:

			return FTM0_C5V;

			break;

		case CHANNEL_N_6:

			return FTM0_C6V;

			break;

		case CHANNEL_N_7:

			return FTM0_C7V;

			break;

		}

		break;

	case FTM_1:

		switch(n_channel){
		case CHANNEL_N_0:

			return FTM1_C0V;

			break;

		case CHANNEL_N_1:

			return FTM1_C1V;

			break;

		}

		break;

	case FTM_2:

		switch(n_channel){
		case CHANNEL_N_0:

			return FTM2_C0V;

			break;

		case CHANNEL_N_1:

			return FTM2_C1V;

			break;

		}

		break;

	case FTM_3:

		switch(n_channel){
		case CHANNEL_N_0:

			return FTM3_C0V;

			break;

		case CHANNEL_N_1:

			return FTM3_C1V;

			break;

		case CHANNEL_N_2:

			return FTM3_C2V;

			break;

		case CHANNEL_N_3:

			return FTM3_C3V;

			break;

		case CHANNEL_N_4:

			return FTM3_C4V;

			break;

		case CHANNEL_N_5:

			return FTM3_C5V;

			break;

		case CHANNEL_N_6:

			return FTM3_C6V;

			break;

		case CHANNEL_N_7:

			return FTM3_C7V;

			break;

		}

		return FALSE;
	}

}

/*Enable the NVIC interruption according to the flex timer*/
void FTM_IRQEnable(FTM_ChannelType channel){
	switch(channel){
	case FTM_0:
		NVIC_enableInterruptAndPriority(FTM0_IRQ, PRIORITY_9);

		break;

	case FTM_1:
		NVIC_enableInterruptAndPriority(FTM1_IRQ, PRIORITY_9);

		break;

	case FTM_2:
		NVIC_enableInterruptAndPriority(FTM2_IRQ, PRIORITY_7);

		break;

	case FTM_3:
		NVIC_enableInterruptAndPriority(FTM3_IRQ, PRIORITY_9);

		break;
	}


}

/*Reads the mail box flag according to the Flex timer*/
uint8 FTM_mailBoxFlag(FTM_ChannelType channel){
	switch(channel){
	case FTM_0:
		return FTM0_MailBox.flag;

	case FTM_1:
		return FTM1_MailBox.flag;

	case FTM_2:
		return FTM2_MailBox.flag;

	case FTM_3:
		return FTM3_MailBox.flag;

	default:
		return FALSE;

	}
}

/*Reads the mail box data according to the Flex timer*/
uint16 FTM_readMailBoxData(FTM_ChannelType channel){
	switch(channel){
	case FTM_0:
		FTM0_MailBox.flag = FALSE;
		return FTM0_MailBox.MailBoxData;

	case FTM_1:
		FTM1_MailBox.flag = FALSE;
		return FTM1_MailBox.MailBoxData;

	case FTM_2:
		FTM2_MailBox.flag = FALSE;
		return FTM2_MailBox.MailBoxData;

	case FTM_3:
		FTM3_MailBox.flag = FALSE;
		return FTM3_MailBox.MailBoxData;

	case 4:
		FTM2_MailBox.flag = FALSE;
		return FTM2_MailBox2.MailBoxData;

	default:
		return FALSE;
	}
}

/*Initializes the flex timer, according to the struct config*/
uint8 FTM_init(const FTM_ConfigType* FTM_Config){

	/*uint32 value, to store different configurations*/
	uint32 channelSttsCntrlCnfg;

	/*Enable the clock gating*/
	FTM_clockGating(FTM_Config->FTM_Channel);

	/*Disable writing protection*/
	FTM_WPDIS(FTM_Config->FTM_Channel);

	/*set the Status and Control configuration*/
	channelSttsCntrlCnfg = FTM_SC_CPWMS(FTM_Config->CPWMS) | FTM_Config->clockSource |
			FTM_Config->clockPrescaler;

	FTM_SC(FTM_Config->FTM_Channel, &channelSttsCntrlCnfg);

	/*set the Combine configuration*/
	/*As, the combine config, affects n and n+1 channels in a flex timer, we
	 * see if the channel in the flex timer is odd or even, and then we shift the
	 * config, if needed*/
	if((FTM_Config->N_Channel)%2 == 0){
	channelSttsCntrlCnfg = FTM_COMBINE_COMBINE0(FTM_Config->COMBINE) | FTM_COMBINE_DECAPEN0(FTM_Config->DECAPEN)
			<< (FTM_Config->N_Channel * 8);
	} else {
	channelSttsCntrlCnfg = FTM_COMBINE_COMBINE0(FTM_Config->COMBINE) | FTM_COMBINE_DECAPEN0(FTM_Config->DECAPEN)
			<< ((FTM_Config->N_Channel - 1) * 8);
	}


	FTM_COMBINE(FTM_Config->FTM_Channel, &channelSttsCntrlCnfg);

	/*set the Channel Status and control configuration*/
	channelSttsCntrlCnfg = FTM_CnSC_MSB(FTM_Config->MSnB) | FTM_CnSC_MSA(FTM_Config->MSnA) |
			FTM_CnSC_ELSB(FTM_Config->ELSB) | FTM_CnSC_ELSA(FTM_Config->ELSA);

	FTM_CSC(FTM_Config->FTM_Channel, FTM_Config->N_Channel, &channelSttsCntrlCnfg);

	/*set the Mod*/
	FTM_MOD(FTM_Config->FTM_Channel, FTM_Config->MOD);

	/*set the CnV*/
	FTM_updateCHValue(FTM_Config->FTM_Channel, FTM_Config->N_Channel, FTM_Config->CNV);

	/*If flex timer interruptions are needed, we enable them*/
	if(FTM_Config->interrupt){

		channelSttsCntrlCnfg = FTM_SC_TOIE_MASK;

		FTM_SC(FTM_Config->FTM_Channel, &channelSttsCntrlCnfg);

		FTM_IRQEnable(FTM_Config->FTM_Channel);
	}

	/*If channel interruptions are needed, we enable them*/
	if(FTM_Config->channelInterrup){

		channelSttsCntrlCnfg = FLEX_TIMER_CHIE;
		FTM_CSC(FTM_Config->FTM_Channel,FTM_Config->N_Channel,&channelSttsCntrlCnfg);
	}

	return TRUE;
 }
