/*
 * SYSUPD.h
 *
 *  Created on: 08/10/2016
 *      Author: Patricio Gomez
 */

#ifndef SOURCES_SYSUPD_H_
#define SOURCES_SYSUPD_H_

#include "BTTN.h"

/**
 * Define SYSTEM_CLOCK as the constant that represents the system clock frequency
 * for calculation purposes in the algorithm
 * */
#define SYSTEM_CLOCK 21000000

/**
 * Define MAX_VOLT as the constant that represents the max ADC convertion result voltage
 * for calculation purposes in the algorithm
 * */
#define MAX_VOLT 3.3

/**
 * Define miliVolt_10 as the constant that represents 10mV
 * for calculation purposes in the algorithm
 * */
#define miliVolt_10 0.01

/**
 * Define CELSIUS or FAHRENHEIT as possible options for the temperature format, that
 * will be displayed in the screen
 * */
#define CELSIUS	0
#define FAHRENHEIT	1

/**
 * Define AUTOMATIC or MANUAL as possible options for the motor control*/
#define AUTOMATIC	0
#define MANUAL	1

/**
 * Define the minimum and maximum temperatures, for the alarm
 * */
#define ALARM_MIN 15
#define ALARM_MAX 45

/**
 * Define the minimum and maximum percentages decrease, for the motor control
 */
#define PERCEN_MIN	5
#define PERCEN_MAX	100


/**
 * Enumeration that defines the type of menu state, the state machine has.
 * A type per menu is defined
 * */
typedef enum{
			DEFAULT_DISP = 0,
			MENU_DISP,
			ALARM_DISP,
			FORMAT_TEMP_DISP,
			PERCEN_DEC_DISP,
			CTRL_MANUAL_DISP,
			FREC_DISP
			}MenuStateType;

/**
 * Struct that defines the functionality a button has, according to the
 * current MenuStateType, this is because in some Menu states, a button may be
 * 'ignored'
 * */
typedef struct{
			/**
			 * uint16 button, is the issue button, that will have a functionality
			 * depending on the current state
			 * **/
			uint16 button;
			/**
			 * void (*fptr_Button_func)(uint8) is the button functionality, it is a
			 * function pointer, that points the function that will be called when this
			 * button is pushed. Also, it has an argument, that according to the function,
			 * it may be taken in account or not
			 * **/
			void (*fptr_Button_func)(uint8);
			/**
			 * uint8 args1 is the argument for the function that has the functionality
			 * of the button, according to the function it may be taken in account or
			 * not
			 * **/
			uint8 args1;
			}ButtonFunctStruct;

/**
 * Struct that defines the state machine for the system updater, it has the currentDisplayState
 * and an array of 7 button functionality structs.
 * **/
typedef struct{
			/**
			 * MenuStateType CurrentDisplayState, is the state that will be selected in the
			 * state machine
			 * **/
			MenuStateType CurrentDisplayState;
			/**
			 * ButtonFunctStruct ButtonFunct[7], is the array that has functionality for
			 * each button, according to the currentDisplayState
			 * **/
			ButtonFunctStruct ButtonFunct[7];
			}SystemUpdateStateMachine;

			/**
			 * SystemUpdateFlags struct, is the struct that has all the values that
			 * will be taken on account for by the system in order to enable the alarm,
			 * motor, etc.
			 *
			 * It has the 'raw' values, which aren´t able to be displayed in the LCD screen
			 * **/
typedef struct{
			/**
			 * MenuStateType currentState, is the currentState of the state machine
			 * **/
			MenuStateType currentState;
			/**
			 * uint8 currentAlarm, is the alarm that will be taken on account in the system
			 * **/
			uint8 currentAlarm;
			/**
			 * uint8 currentSpeed, is the speed at which the control will run
			 * **/
			uint8 currentSpeed;
			/**
			 * uint8 currentFormat, is the temperature format that will be displayed
			 * **/
			uint8 currentFormat :1;
			/**
			 * uint8 currentPerInc, is the percentage increase (or decrease) that will affect
			 * the motor
			 * **/
			uint8 currentPerInc;
			/**
			 * uint8 currentManual, is the state of the motor, is it will be controled manually or
			 * by temperature
			 * **/
			uint8 currentManual:1;
			/**
			 * float currentTemperature, is the 'raw' value of the current temperature
			 * **/
			float currentTemperature;
			/**
			 * float currentFrec, is the 'raw' value of the current frequency
			 * **/
			float currentFrec;
			}SystemUpdateFlags;

			/**
			 * SystemDisplayFlags struct, is the struct that contains the current values of
			 * system (alarm, temperature, speed, etc.), that will be actually displayed in the
			 * screen. This values, could be not the ones that the system takes on account,
			 *  (while editting, for example).
			 *  This struct, has the values 'transformed' to strings, in order to display them
			 * **/
typedef struct{
			/**
			 * MenuStateType currentState, indicates the current menu that will be displayed
			 * **/
			MenuStateType currentState;
			/**
			 * char currentAlarm[6] is the char array that contains the "displayable" alarm
			 * value
			 * **/
			char currentAlarm[6];
			/**
			 * char currentSpeed[6] is the char array that contains the "displayable" speed
			 * value
			 * **/
			char currentSpeed[6];
			/**
			 * uint8 currentFormat, is the temperature format that will be displayed
			 * **/
			uint8 currentFormat :1;
			/**
			 * char currentPerInc[6] is the char array that contains the "displayable"
			 * percentage increase value
			 * **/
			char currentPerInc[6];
			/**
			 * uint8 currentManual, is the state of the motor, is it will be controled manually or
			 * by temperature
			 * **/
			uint8 currentManual :1;
			/**
			 * char currentTemperature[6] is the char array that contains the "displayable"
			 * temperature value
			 * **/
			char currentTemperature[6];
			/**
			 * This value, is used as a divider between the temperature string and the
			 * frequency string
			 * **/
			uint8 div:1;
			/**
			 * char currentFrec[10] is the char array that contains the "displayable"
			 * frequency value
			 * **/
			char currentFrec[10];
			}SystemDisplayFlags;


/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
	 \brief
		 This function is initializes everything related to the system, such as output pins,
		 for PWM, input pins for frequencymeter, etc.
	 \return void

 */
void SYSUPD_init();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
	 \brief
		 This function receives a button that will update the system, according to the
		 current state, and the button functionality.
	 \param[in] button - button pressed, that will update the system
	 \return void

 */
void SYSUPD_update(uint16 button);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
	 \brief
		 This function is a button functionality, that switches between menus.
		 Example: Switching from DEFAULT_DISP to MENU_DISP
	 \param[in] MenuStateType - Menu State that will be the new, current state
	 \return void

 */
void switchMenu(MenuStateType);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
	 \brief
		 This function is a button functionality, that increments(or decreases)
		 a parameter (alarm,percentage increase, speed, etc.). According to the
		 state, the input parameter, could be not taken on account.
		 Example: Increase the alarm threshold
	 \param[in] currentInc - increment that will be taken on account
	 \return void

 */
void incUpdate(uint8 currentInc);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
	 \brief
		 This function is a button functionality, that turns on or off the automatic
		 speed control of the motor
		 Example: Set the motor control as manual
	 \param[in] currentTurn - state that the motor control will have (manual or automatic)
	 \return void

 */
void turnUpdate(uint8 currentTurn);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
	 \brief
		 This function is a button functionality, that sets the edited parameters, as fixed
		 parameters, this is: when this function is called, is because the B3 was pressed,
		 meaning that all the changes that the user made, will now be taken on account.
		 Example: Set the edited alarm, as the current alarm
	 \param[in] currentParam - new fixed parameter
	 \return void

 */
void setUpdate(uint8 currentParam);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
	 \brief
		 This function is a button functionality, that skips or ignores a button
		 by doing nothing
	 \param[in] none
	 \return void

 */
void noFunct(uint8 none);



/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
	 \brief
		 This function returns the system flags (alarm, speed, temperature) that will be
		 displayed in the LCD, this could be not the ones that the system takes on account
		 (When editting, for example)
	 \return SystemDisplayFlags* - pointer of the displayable data struct

 */
SystemDisplayFlags* SYSUPD_SDF();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
	 \brief
		 This function returns the system flags (alarm, speed, temperature) that will be
		 taken on account by the system, this could be not the ones that are displayed
		 (when editting for example)
	 \return SystemUpdateFlags* - pointer of the data struct

 */
SystemUpdateFlags* SYSUPD_SUF();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
	 \brief
		 This function converts the 'raw' data to 'displayable' data, meaning that the data
		 in the SystemUpdateFlags is converted in to strings, and stored in the SystemDisplay
		 Flags. (Converts frequency and temperature)
	 \return void

 */
void floatToString();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
	 \brief
		 This function converts the 'raw' data to 'displayable' data, meaning that the data
		 in the SystemUpdateFlags is converted in to strings, and stored in the SystemDisplay
		 Flags. (Converts Alarm, Speed and Percentage Increase)
	 \return void

 */
void uint8ToString();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
	 \brief
		 This function receives a value, that will be converted to the current temperature.
		 The value received is a float that is the result of the ADC convertion, this value
		 is divided by the maximum value that the ADC can give in 16 bits, 65535, and multiplied
		 by the max voltage, 3.3V, the result is the input voltage. Then is divided by 10mV, to
		 get the Celsius temperature. The convertion is donde through a MACRO
	 \param[in] temperature - ADC convertion result
	 \return void

 */
void changeTemperature(float temperature);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
	 \brief
		 This function is called after receiving a temperature value. Checks that the temperature
		 is under the alarm threshold, if not, it turns on a buzzer.
		 The alarm check, is done always, with the temperature format as Celsius.
	 \return void

 */
void temperatureAlarmCheck();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
	 \brief
		 This function is called after receiving a temperature value. Checks that the
		 temperature hasn´t increase 2 Celsius degrees, if not, it decreases the motor
		 speed by %15. This is done, if the motor control is automatic
		 The motor check, is done always, with the temperature format as Celsius.
	 \return void

 */
void temperatureMotorControl();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
	 \brief
		 This function receives two values, that will be converted to the current frequency
		 being measured. time1 is the first time an rising edge is detected, and time2 is the
		 second time an rising edge is detected. According to these values, and considering
		 that there could be an overflow, we get the period, and by dividing the system clock
		 frequency by the period, we get the current frequency. The conversion is done
		 through a MACRO
	 \param[in] time1 - counter value when the first rising edge ocurred.
	 \param[in] time2 - counter value when the second rising edge ocurred.
	 \return void

 */
void changeFrequency(uint16 time1, uint16 time2);

#endif /* SOURCES_SYSUPD_H_ */
