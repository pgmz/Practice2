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
#include "DataTypeDefinitions.h"
#include "GlobalFunctions.h"
#include "GPIO.h"
#include "DISP.h"
#include "SPI.h"
#include "LCDNokia5110.h"
#include "stdio.h"

/*Struct array, that contains a function pointer according to the
 * current State, indicating what will be printed in the LCD*/
StateDisplay stateDisplay[7] = {
		{DEFAULT_DISP, defaultMenu},
		{MENU_DISP, mainMenu},
		{ALARM_DISP, alarmMenu},
		{FORMAT_TEMP_DISP, temperatureMenu},
		{PERCEN_DEC_DISP, percentageMenu},
		{CTRL_MANUAL_DISP, motorControlMenu},
		{FREC_DISP, frequencyMenu}
};

/*Print in the LCD the alarm menu, that takes in count the values in SDF*/
void alarmMenu(SystemDisplayFlags* SDF){
	//Clear screen
	LCDNokia_clear();
	//Write alarm
	LCDNokia_gotoXY(24,1);
	LCDNokia_sendString("Alarm");
	//Write alarm threshold
	LCDNokia_gotoXY(28,2);
	LCDNokia_sendString( SDF->currentAlarm);
	//Write 'C or 'F
	if(SDF->currentFormat == CELSIUS){
	LCDNokia_sendString("'C");
	} else {
	LCDNokia_sendString("'F");
	}
	//Write the option buttons
	LCDNokia_gotoXY(7,3);
	LCDNokia_sendString("(-)B1(+)B2");
	LCDNokia_gotoXY(21,4);
	LCDNokia_sendString("(OK)B3");
}

/*Print in the LCD the temperature menu, that takes in count the values in SDF*/
void temperatureMenu(SystemDisplayFlags* SDF){
	//Clear LCD
	LCDNokia_clear();
	//Write the temp format
	LCDNokia_gotoXY(3,1);
	LCDNokia_sendString("Temp Format");
	//Write the temperature
	LCDNokia_gotoXY(0,2);
	LCDNokia_sendString("Temp=");
	LCDNokia_sendString(SDF->currentTemperature);
	//Write 'C or 'F
	if(SDF->currentFormat == CELSIUS){
	LCDNokia_sendString("'C");
	} else {
	LCDNokia_sendString("'F");
	}
	//Write the option buttons
	LCDNokia_gotoXY(7,3);
	LCDNokia_sendString("(C)B1(F)B2");
	LCDNokia_gotoXY(21,4);
	LCDNokia_sendString("(OK)B3");
}

/*Print in the LCD the percentage menu, that takes in count the values in SDF*/
void percentageMenu(SystemDisplayFlags* SDF){
	//Clear the screen
	LCDNokia_clear();
	//Write % de decre
	LCDNokia_gotoXY(7,1);
	LCDNokia_sendString( "% de decre");
	//Write percentage of decrement
	LCDNokia_gotoXY(28,2);
	LCDNokia_sendString( "%");
	LCDNokia_sendString(SDF->currentPerInc);
	LCDNokia_gotoXY(7,3);
	//Write the option buttons
	LCDNokia_sendString("(-)B1(+)B2");
	LCDNokia_gotoXY(21,4);
	LCDNokia_sendString("(OK)B3");
}

/*Print in the LCD the motor control menu, that takes in count the values in SDF*/
void motorControlMenu(SystemDisplayFlags* SDF){
	//Clear LCD
	LCDNokia_clear();
	LCDNokia_gotoXY(3,0);
	//Write autom or manual
	if(SDF->currentManual == MANUAL){
		LCDNokia_sendString("Ctrl manual");
	} else {
		LCDNokia_sendString("Ctrl autom");
	}
	//Write current speed percentage
	LCDNokia_gotoXY(30,1);
	LCDNokia_sendString("%");
	LCDNokia_sendString(SDF->currentSpeed);
	LCDNokia_gotoXY(0,2);
	//Write option buttons
	LCDNokia_sendString("ON)B1 OFF)B2");
	LCDNokia_gotoXY(21,3);
	LCDNokia_sendString("(OK)B3");
	LCDNokia_gotoXY(7,4);
	LCDNokia_sendString("(-)B4(+)B5");
}

/*Print in the LCD the frequency menu, that takes in count the values in SDF*/
void frequencyMenu(SystemDisplayFlags* SDF){
	//Clear LCD
	LCDNokia_clear();
	//Write the current frequency
	LCDNokia_gotoXY(7,1);
	LCDNokia_sendString("Frecuencia");
	LCDNokia_gotoXY(28,2);
	LCDNokia_sendString("(Hz)");
	LCDNokia_gotoXY(0,3);
	LCDNokia_sendString(SDF->currentFrec);
}

/*Print in the LCD the default menu, that takes in count the values in SDF*/
void defaultMenu(SystemDisplayFlags* SDF){
	//Clear LCD
	LCDNokia_clear();
	//Write the motor speed
	LCDNokia_gotoXY(10,0);
	LCDNokia_sendString("Velocidad");
	LCDNokia_gotoXY(31,1);
	LCDNokia_sendString(SDF->currentSpeed);
	//Write the current temperature
	LCDNokia_gotoXY(4,2);
	LCDNokia_sendString("Temperatura");
	LCDNokia_gotoXY(14,3);
	LCDNokia_sendString(SDF->currentTemperature);
	//Write 'C or 'F
	if(SDF->currentFormat == CELSIUS){
	LCDNokia_sendString("'C");
	} else {
	LCDNokia_sendString("'F");
	}

}

/*Print in the LCD the main menu*/
void mainMenu(SystemDisplayFlags* SDF){
	//Clear LCD
	LCDNokia_clear();
	//Write the menu options
	LCDNokia_gotoXY(0,0);
	LCDNokia_sendString("1)Alarma");
	LCDNokia_gotoXY(0,1);
	LCDNokia_sendString("2)Formato temp");
	LCDNokia_gotoXY(0,2);
	LCDNokia_sendString("3)% de inc");
	LCDNokia_gotoXY(0,3);
	LCDNokia_sendString("4)Ctrl manual");
	LCDNokia_gotoXY(0,4);
	LCDNokia_sendString("5)Frecuencia");
}

/*update the Display, according to the SDF current State*/
void update_Display(SystemDisplayFlags* SDF){
	/*Call the function that the function pointer ponits, accdoring to the current
	 * state*/
	stateDisplay[SDF->currentState].StateDisplay(SDF);
}


