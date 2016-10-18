/*
  * SYSUPD.c
 *
 *  Created on: 08/10/2016
 *      Author: Patricio Gomez
 */

#include "SYSUPD.h"
#include "BTTN.h"
#include "GPIO.h"

/**
 * MACRO that defines the conversion of a ADC conversion result, to an actual
 * temperature in Celsius degrees.
 * It is done by dividing the ADC temperature, by the max value, and multiplying by
 * the max Voltage. This gives the input voltage, that, when divided by 10m Volts, a
 * temperature in Celsius degrees is given.
 * **/
#define TEMPERATURE(temperature) ((temperature/0xFFFF)*MAX_VOLT)/(miliVolt_10)


/**
 * MACRO that defines the conversion of 2 counter values, when the clock hasn't overflow
 * this is done by dividing the system clock value by the period of the 2 counter values
 * **/
#define FREQUENCY(time1, time2) ((float)(SYSTEM_CLOCK/(time2-time1)))

/**
 * MACRO that defines the conversion of 2 counter values, when the clock has overflow
 * this is done by dividing the system clock value by the period of the 2 counter values
 * **/
#define FREQUENCY_OF(time1, time2) ((float)(SYSTEM_CLOCK/(time2+(0xFFFF-time1))))

/**
 * SystemUpdateFlags SUF, has the values that the system will take on account when checking
 * the motor, or the alarm. This values remain unaffected until setUpdate() function is called
 * **/
static SystemUpdateFlags SUF = {
		///Initial state
		DEFAULT_DISP,
		///Initial alarm
		30,
		///Inital speed
		80,
		///Initial temperature format
		CELSIUS,
		///Initial percentage increase or decrease
		15,
		///Initial motor control option
		AUTOMATIC,
		///Initial temperature (none)
		0,
		///Initial frequency (none)
		0
};

/**
 * SystemUpdateFlags SUFedit, has the values that will be taken on account when displaying
 * in the LCD screen. When we are increasing the alarm threshold, or setting the motor speed
 * what we are affecting, is actually this 'temporal' struct. When an update is set, this values
 * are copied to the SUF struct.
 * Initially, it has the same values of SUF
 * **/
static SystemUpdateFlags SUFedit = {
		DEFAULT_DISP,
		30,
		80,
		CELSIUS,
		15,
		AUTOMATIC,
		0,
		0
};

/**
 * SystemDisplayFlags SDF, has the values that will be displayed, as they are strings.
 * This values are based in SUFedit, by converting the SUFedit values to strings and storing
 * them here.
 * Again, this values are the ones that will be displayed, but not necessarily the ones the
 * system takes on account when setting the alarm, for example.
 * Initially, it has the same values than SUF and SUFedit.
 * **/
static SystemDisplayFlags SDF = {
		DEFAULT_DISP,
		"030",
		"080",
		CELSIUS,
		"015",
		AUTOMATIC,
		"ZZZ.ZZ",
		0,
		"ZZZZZZZ.ZZ"
};

//args, stores the argument that will be passed to the button functionality functions
static uint8 args = 0;

//buttonGlobal, stores the last pressed button, so it's available to every function in
//order to update the system
static uint16 buttonGlobal = 0;

//tempCompare, stores a temperature value, so it can be compared with another, and check
//if the motor has to decrease it's speed
static uint8 tempCompare = 0;

/**
 * Main State machine, or array that contains the actions to be done in the system,
 * it is an struct array, that indicates which funcionality will have a button, according
 * to the current manu state
 * **/
const SystemUpdateStateMachine SUSM[7] = {

		/**
		 * In the Default display menu, only the BUTTON_0 has the functionality of
		 * switching the current menu, to the Menu display state
		 * **/
		{DEFAULT_DISP,{
				{BUTTON_0, switchMenu, MENU_DISP},
				{BUTTON_1, noFunct, 0},
				{BUTTON_2, noFunct, 0},
				{BUTTON_3, noFunct, 0},
				{BUTTON_4, noFunct, 0},
				{BUTTON_5, noFunct, 0},
				{NULL_BUTTON, noFunct, 0}
		}},

		/**
		 * In the Menu display, every button (except NULL_BUTTON), has the funcionality
		 * of switching the current menu, to one according to the button:
		 *
		 *		BUTTON_0 -> return to the default display
		 *		BUTTON_1 -> go to the alarm display
		 *		BUTTON_2 -> go to the format temperature display
		 *		BUTTON_3 -> go to the percentage decrease or increase display
		 *		BUTTON_4 -> go to the motor control display
		 *		BUTTON_5 -> go to the frequenciometer display
		 * **/
		{MENU_DISP,{
				{BUTTON_0, switchMenu, DEFAULT_DISP},
				{BUTTON_1, switchMenu, ALARM_DISP},
				{BUTTON_2, switchMenu, FORMAT_TEMP_DISP},
				{BUTTON_3, switchMenu, PERCEN_DEC_DISP},
				{BUTTON_4, switchMenu, CTRL_MANUAL_DISP},
				{BUTTON_5, switchMenu, FREC_DISP},
				{NULL_BUTTON, noFunct, 0}

		}},

		/**
		 * In the Alarm display menu, as we are going to change the alarm threshold,
		 * we have the following funcionalities:
		 *
		 * 		BUTTON_0 -> return to the default display
		 * 		BUTTON_1 -> decrease the alarm threshold by 1 degree
		 * 		BUTTON_2 -> increase the alarm threshold by 1 degree
		 * 		BUTTON_3 -> set the alarm threshold
		 *
		 * **/
		{ALARM_DISP,{
				{BUTTON_0, switchMenu, DEFAULT_DISP},
				{BUTTON_1, incUpdate, -1},
				{BUTTON_2, incUpdate, 1},
				{BUTTON_3, setUpdate, 0},
				{BUTTON_4, noFunct, 0},
				{BUTTON_5, noFunct, 0},
				{NULL_BUTTON, noFunct, 0}

		}},

		/**
		 * In the Format temperature display menu, as we are going to change
		 * the format temperature threshold, we have the following funcionalities:
		 *
		 * 		BUTTON_0 -> return to the default display
		 * 		BUTTON_1 -> set CELSIUS format
		 * 		BUTTON_2 -> set FAHRENHEIT format
		 * 		BUTTON_3 -> set the temperature format
		 *
		 * **/
		{FORMAT_TEMP_DISP,{
				{BUTTON_0, switchMenu, DEFAULT_DISP},
				{BUTTON_1, incUpdate, CELSIUS},
				{BUTTON_2, incUpdate, FAHRENHEIT},
				{BUTTON_3, setUpdate, 0},
				{BUTTON_4, noFunct, 0},
				{BUTTON_5, noFunct, 0},
				{NULL_BUTTON, noFunct, 0}

		}},

		/**
		 * In the percentage increase or decrease menu, as we are going to change
		 * the percentage increase or decrease, we have the following funcionalities:
		 *
		 * 		BUTTON_0 -> return to the default display
		 * 		BUTTON_1 -> decrease the percentage by 5
		 * 		BUTTON_2 -> increase the percentage by 5
		 * 		BUTTON_3 -> set the percentage of the increase or decrease
		 *
		 * **/
		{PERCEN_DEC_DISP,{
				{BUTTON_0, switchMenu, DEFAULT_DISP},
				{BUTTON_1, incUpdate, -5},
				{BUTTON_2, incUpdate, 5},
				{BUTTON_3, setUpdate, },
				{BUTTON_4, noFunct, 0},
				{BUTTON_5, noFunct, 0},
				{NULL_BUTTON, noFunct, 0}

		}},

		/**
		 * In the motor control display menu, as we are going to change the motor speed, we
		 * have the following functionalities:
		 *
		 * 		BUTTON_0 -> return to the default display
		 * 		BUTTON_1 -> set the motor control as automatic
		 * 		BUTTON_2 -> set the motor control as manual
		 * 		BUTTON_3 -> set the motor control speed and mode
		 * 		BUTTON_4 -> increase the motor speed by the percentage increase(if manual)
		 * 		BUTTON_5 -> decrease the motor speed by the percentage decrease(if manual)
		 *
		 * **/
		{CTRL_MANUAL_DISP,{
				{BUTTON_0, switchMenu, DEFAULT_DISP},
				{BUTTON_1, turnUpdate, AUTOMATIC},
				{BUTTON_2, turnUpdate, MANUAL},
				{BUTTON_3, setUpdate, },
				{BUTTON_4, incUpdate, },
				{BUTTON_5, incUpdate, },
				{NULL_BUTTON, noFunct, 0}

		}},

		/**
		 * In the motor control display menu, as we are only able to see the input frequency,
		 * we have the followinf functionality
		 *
		 * 		BUTTON_0 -> return to the default display
		 *
		 * **/
		{FREC_DISP,{
				{BUTTON_0, switchMenu, DEFAULT_DISP},
				{BUTTON_1, noFunct, 0},
				{BUTTON_2, noFunct, 0},
				{BUTTON_3, noFunct, 0},
				{BUTTON_4, noFunct, 0},
				{BUTTON_5, noFunct, 0},
				{NULL_BUTTON, noFunct, 0}

		}},
};

/**
 * Initialize the System Update.
 * Sets the PTC2, PTB19 PTB18 as needed for using this for the PWM, Input Capture, and
 * Alarm buzzer
 * **/
void SYSUPD_init(){

	/*Enable the clock gating for PORTC*/
	GPIO_clockGating(GPIOC); //PWM
	/*Sets the PORTC PIN2, with the functionality of MUX4, so the Flex Timer 0
	 * Channel 1 may use this pin*/
	GPIO_pinControlRegisterType	pinControlRegisterPORTB = GPIO_MUX4;
	GPIO_pinControlRegister(GPIOC,BIT2,&pinControlRegisterPORTB);

	/*Enable the clock gating for PORTB*/
	GPIO_clockGating(GPIOB); //INPUT CAPTURE
	/*Sets the PORTB PIN19, with the functionality of MUX3, so the Flex Timer 2
	 * Channel 1 may use this pin*/
	GPIO_pinControlRegisterType	pinControlRegisterPORTC = GPIO_MUX3;
	GPIO_pinControlRegister(GPIOB,BIT19,&pinControlRegisterPORTC);

	/*Sets the PORTb PIN18, with the functionality of MUX1, so it is used
	 * as an output*/
	GPIO_pinControlRegisterType	pinControlRegisterPORTB18 = GPIO_MUX1;
	GPIO_pinControlRegister(GPIOB,BIT18,&pinControlRegisterPORTB18);
	GPIO_clearPIN(GPIOB,BIT18); // BUZZER
	/*Sets PORT B pin 18 as an output*/
	GPIO_dataDirectionPIN(GPIOB,GPIO_OUTPUT,BIT18);
}

/**
 * This function is called, everytime a button is pressed
 * **/
void SYSUPD_update(uint16 button){
	/*Set the button received as a global variable*/
	buttonGlobal = button;
	/*Store in args, the argument for the function to be call (if needed), according
	 * to the current state and the button pressed*/
	args = SUSM[SUFedit.currentState].ButtonFunct[buttonGlobal].args1;
	/*Call the button functionality, according to the current state and the button
	 * pressed*/
	SUSM[SUFedit.currentState].ButtonFunct[buttonGlobal].fptr_Button_func(args);

	/*Copy the format and manual parameters of the editable SUF, to the displayable SDF
	 * it format or manual wasn´t edited, this isn´t needed*/
	SDF.currentFormat = SUFedit.currentFormat;
	SDF.currentManual = SUFedit.currentManual;

	/*Convert to string the values in SDF, taking in count the ones in SUFedit*/
	floatToString();
	uint8ToString();
}

/*Button functionality: switchMenu*/
void switchMenu(MenuStateType nextState){
	/*currentState is the next one, according to the current state and button pressed*/
	SUF.currentState = nextState;
	/*Set the change, also in SUFedit*/
	SUFedit.currentState = nextState;
	/*Set the change, also in SDF*/
	SDF.currentState = SUF.currentState;
	/*Copy SUF data to SUFeddit, this happens, because if we were editting a parameter,
	 * for example: alarm, and if we didn´t set the update, when we get back we get the
	 * previous alarm, before editting. */
	SUFedit = SUF;
	return;
}

/*Button functionality: incUpdate*/
void incUpdate(uint8 currentInc){

	/*According to the currentState, we change a paremeter*/
	switch(SUFedit.currentState){

	/*If we are in the ALARMD_DISP, we will change the alarm threshold*/
	case ALARM_DISP:

		/*We make sure that we don´t go over or under the permitted values*/
		if((SUFedit.currentAlarm > ALARM_MIN)&&(currentInc < 0) || (SUFedit.currentAlarm < ALARM_MAX)&&(currentInc > 0)){
			SUFedit.currentAlarm = SUFedit.currentAlarm + currentInc;
		}
		break;

	/*If we are in the FORMAT_TEMP_DISP, we will change the temperature format*/
	case FORMAT_TEMP_DISP:

		/*We change the current format, as desired*/
		SUFedit.currentFormat = currentInc;

		break;

	/*If we are in the PERCEN_DEC_DISP, we will change the percentage increase or
	 * decrease*/
	case PERCEN_DEC_DISP:

		/*We make sure that we don´t go over or under the permitted values*/
		if(((SUFedit.currentPerInc - 5)>= PERCEN_MIN) &&(buttonGlobal == BUTTON_1)){
			SUFedit.currentPerInc = SUFedit.currentPerInc - 5;
		}else if(((SUFedit.currentPerInc + 5) <= PERCEN_MAX)&&(buttonGlobal == BUTTON_2)){
			SUFedit.currentPerInc = SUFedit.currentPerInc + 5;
		}

		break;

	/*If we are in the CTRL_MANUAL_DISP, we will change the motor speed*/
	case CTRL_MANUAL_DISP:

		/*If we are in the automatic mode, we aren´t able to change the motor speed
		 * manually*/
		if(SUFedit.currentManual == AUTOMATIC){
			break;
		}

		/*We make sure that we don´t go over or under the permitted values*/
		if(((SUFedit.currentSpeed - SUF.currentPerInc)> PERCEN_MIN) &&(buttonGlobal == BUTTON_4)){
			SUFedit.currentSpeed = SUFedit.currentSpeed - SUFedit.currentPerInc;
		}else if(((SUFedit.currentSpeed + SUF.currentPerInc) < PERCEN_MAX)&&(buttonGlobal == BUTTON_5)){
			SUFedit.currentSpeed = SUFedit.currentSpeed + SUFedit.currentPerInc;
		}

		break;

	default:
		break;
	}
}

/*Button functionality: turnUpdate*/
void turnUpdate(uint8 currentTurn){

	/*This function is called when we wan´t to change from manual to automatic
	 * or viceversa. We set the current mode, manual or automatic*/
	SUFedit.currentManual = currentTurn;
	return;
}

/*Button functionality: setUpdate*/
void setUpdate(uint8 currentParam){

	/*Set currentState as DEFAULT_DISP, meaning that we go back to the default display*/
	SUFedit.currentState = DEFAULT_DISP;

	/*The editable SUF, now is the current SUF that the system will take on account*/
	SUF = SUFedit;
	SDF.currentState = DEFAULT_DISP;
	return;
}

/*Button functionality: noFunct*/
void noFunct(uint8 none){
	/*Does nothing*/
	return;
}

/*Return the direction of the SDF to display the values in the screen*/
SystemDisplayFlags* SYSUPD_SDF(){
	return &SDF;
}

/*Return the direction of the SUF to read values from this struct*/
SystemUpdateFlags* SYSUPD_SUF(){
	return &SUF;
}

/**/
void floatToString(){

	float data_in = SUFedit.currentTemperature * 100;

	if(SUFedit.currentFormat != CELSIUS){
		data_in = (data_in*1.8) + 3200;
	}

	int data_out = data_in;

	SDF.currentTemperature[0] = (char)(data_out / 10000) + 48;
	SDF.currentTemperature[1] = (char)((data_out - ((SDF.currentTemperature[0] - 48) * 10000)) / 1000) + 48;
	SDF.currentTemperature[2] = (char)((data_out - ((SDF.currentTemperature[0] - 48) * 10000 + (SDF.currentTemperature[1] - 48) * 1000)) / 100) + 48;
	SDF.currentTemperature[4] = (char)((data_out - ((SDF.currentTemperature[0] - 48) * 10000 + (SDF.currentTemperature[1] - 48) * 1000 + (SDF.currentTemperature[2] - 48) * 100)) / 10) + 48;
	SDF.currentTemperature[5] = (char)(data_out - ((SDF.currentTemperature[0] - 48) * 10000 + (SDF.currentTemperature[1] - 48) * 1000 + (SDF.currentTemperature[2] - 48)* 100 + (SDF.currentTemperature[4] - 48) * 10)) + 48;

	data_in = SUFedit.currentFrec * 100;

	data_out = data_in;

	SDF.currentFrec[0] = (char)(data_out / 100000000) + 48;
	SDF.currentFrec[1] = (char)((data_out - ((SDF.currentFrec[0] - 48) * 100000000)) / 10000000) + 48;
	SDF.currentFrec[2] = (char)((data_out - ((SDF.currentFrec[0] - 48) * 100000000 + (SDF.currentFrec[1] - 48) * 10000000)) / 1000000) + 48;
	SDF.currentFrec[3] = (char)((data_out - ((SDF.currentFrec[0] - 48) * 100000000 + (SDF.currentFrec[1] - 48) * 10000000 + (SDF.currentFrec[2] - 48) * 1000000)) / 100000) + 48;
	SDF.currentFrec[4] = (char)((data_out - ((SDF.currentFrec[0] - 48) * 100000000 + (SDF.currentFrec[1] - 48) * 10000000 + (SDF.currentFrec[2] - 48) * 1000000 + (SDF.currentFrec[3] - 48) * 100000)) / 10000) + 48;
	SDF.currentFrec[5] = (char)((data_out - ((SDF.currentFrec[0] - 48) * 100000000 + (SDF.currentFrec[1] - 48) * 10000000 + (SDF.currentFrec[2] - 48) * 1000000 + (SDF.currentFrec[3] - 48) * 100000 + (SDF.currentFrec[4] - 48) * 10000)) / 1000) + 48;
	SDF.currentFrec[6] = (char)((data_out - ((SDF.currentFrec[0] - 48) * 100000000 + (SDF.currentFrec[1] - 48) * 10000000 + (SDF.currentFrec[2] - 48) * 1000000 + (SDF.currentFrec[3] - 48) * 100000 + (SDF.currentFrec[4] - 48) * 10000 + (SDF.currentFrec[5] - 48) * 1000)) / 100) + 48;
	SDF.currentFrec[8] = (char)((data_out - ((SDF.currentFrec[0] - 48) * 100000000 + (SDF.currentFrec[1] - 48) * 10000000 + (SDF.currentFrec[2] - 48) * 1000000 + (SDF.currentFrec[3] - 48) * 100000 + (SDF.currentFrec[4] - 48) * 10000 + (SDF.currentFrec[5] - 48) * 1000 + (SDF.currentFrec[6] - 48) * 100)) / 10) + 48;
	SDF.currentFrec[9] = (char)(data_out -  ((SDF.currentFrec[0] - 48) * 100000000 + (SDF.currentFrec[1] - 48) * 10000000 + (SDF.currentFrec[2] - 48) * 1000000 + (SDF.currentFrec[3] - 48) * 100000 + (SDF.currentFrec[4] - 48) * 10000 + (SDF.currentFrec[5] - 48) * 1000 + (SDF.currentFrec[6] - 48) * 100 + (SDF.currentFrec[8] - 48) * 10)) + 48;
}

void uint8ToString(){
	int data_out = SUFedit.currentAlarm;

	if(SUFedit.currentFormat != CELSIUS){
		data_out = (data_out*1.8) + 32;
	}

	SDF.currentAlarm[0] = (char)(data_out / 100) + 48;
	SDF.currentAlarm[1] = (char)((data_out - ((SDF.currentAlarm[0] - 48) * 100)) / 10) + 48;
	SDF.currentAlarm[2] = (char)(data_out - ((SDF.currentAlarm[0] - 48) * 100 + (SDF.currentAlarm[1] - 48) * 10)) + 48;

	data_out = SUFedit.currentPerInc;
	SDF.currentPerInc[0] = (char)(data_out / 100) + 48;
	SDF.currentPerInc[1] = (char)((data_out - ((SDF.currentPerInc[0] - 48) * 100)) / 10) + 48;
	SDF.currentPerInc[2] = (char)(data_out - ((SDF.currentPerInc[0] - 48) * 100 + (SDF.currentPerInc[1] - 48) * 10)) + 48;

	data_out = SUFedit.currentSpeed;
	SDF.currentSpeed[0] = (char)(data_out / 100) + 48;
	SDF.currentSpeed[1] = (char)((data_out - ((SDF.currentSpeed[0] - 48) * 100)) / 10) + 48;
	SDF.currentSpeed[2] = (char)(data_out - ((SDF.currentSpeed[0] - 48) * 100 + (SDF.currentSpeed[1] - 48) * 10)) + 48;



}

/*Changes the current temperature*/
void changeTemperature(float temperature){
	/*SUFedit and SUF get the new temperature, which is based in the ADC
	 * convertion result and TEMPERATURE() Macro function*/
	SUFedit.currentTemperature = TEMPERATURE(temperature);
	SUF.currentTemperature = TEMPERATURE(temperature);
	/*Convert the SUFedit temperature to float in the SDF*/
	floatToString();
}

/*Check that the temperature is below the threshold*/
void temperatureAlarmCheck(){
	/*If the current alarm is greater or equal to the threshold...*/
	if(SUF.currentTemperature >= SUF.currentAlarm){
		/*... turn on the BUZZER*/
		GPIO_setPIN(GPIOB,BIT18); //BUZZER
	} else {
		/*... else, turn off the BUZZER*/
		GPIO_clearPIN(GPIOB,BIT18); //BUZZER
	}
}

/*Check that the temperature has increased 2 degrees*/
void temperatureMotorControl(){
	/*Make sure that we aren´t in MANUAl mode*/
	if(SUF.currentManual == MANUAL){
		return;
	}

	/*If it's the first time we compare, we get the current temperature in the tempCompare*/
	if(tempCompare == 0){
		tempCompare = SUF.currentTemperature;
		return;
	}
	/*If there has been a change of 2 degrees, decrease motor, and store a new
	 * temperature reference*/
	if((SUF.currentTemperature - tempCompare)>=2){

		//If we are able to decrease, we decrease the motor speed
		if(SUF.currentSpeed - 15 > PERCEN_MIN){
			SUF.currentSpeed = SUF.currentSpeed - 15;
			SUFedit.currentSpeed = SUF.currentSpeed;
			uint8ToString();
		}
		tempCompare = SUF.currentTemperature;

	}

}

/*Changes the current frequency displayed*/
void changeFrequency(uint16 time1, uint16 time2){

	/*Using the FREQUENCY and FREQUENCY_OF Macro functions, we get the
	 * actual frequency from time1 and time2 values*/
	if(time2>time1){
		SUFedit.currentFrec = FREQUENCY(time1, time2);
	} else {
		SUFedit.currentFrec = FREQUENCY_OF(time1, time2);
	}
	SUF.currentFrec = SUFedit.currentFrec;

	/*Convert the measured frequency in SUFedit, to string in SDF*/
	floatToString();
}

