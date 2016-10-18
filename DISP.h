/*
 * DISP.h
 *
 *  Created on: 06/10/2016
 *      Author: Edson
 */

#ifndef SOURCES_MENU_H_
#define SOURCES_MENU_H_

#include "DataTypeDefinitions.h"
#include "GlobalFunctions.h"
#include "SYSUPD.h"

/**
 * Struct StateDisplay, will indicate, according to the currentState, what to display in
 * the LCD
 * **/
typedef struct{
	/*currentState is determined by the SDF received*/
	MenuStateType currentState;
	/*StateDisplay is a function pointer that receives the SDF, that indicates
	 * what to print in the LCD, taking in account SDF*/
	void (*StateDisplay)(SystemDisplayFlags*);
}StateDisplay;

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function displays in the LCD, the default Menu
 	 \param[in] SDF - Data to take account for displaying in the LCD
 	 \return void
 */
static void defaultMenu(SystemDisplayFlags* SDF);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function displays in the LCD, the main Menu
 	 \param[in] SDF - Data to take account for displaying in the LCD
 	 \return void
 */
static void mainMenu(SystemDisplayFlags* SDF);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function displays in the LCD, the alarm Menu
 	 \param[in] SDF - Data to take account for displaying in the LCD
 	 \return void
 */
static void alarmMenu(SystemDisplayFlags* SDF);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function displays in the LCD, the temperature Menu
 	 \param[in] SDF - Data to take account for displaying in the LCD
 	 \return void
 */
static void temperatureMenu(SystemDisplayFlags* SDF);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function displays in the LCD, the percentage Menu
 	 \param[in] SDF - Data to take account for displaying in the LCD
 	 \return void
 */
static void percentageMenu(SystemDisplayFlags* SDF);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function displays in the LCD, the motor Control Menu
 	 \param[in] SDF - Data to take account for displaying in the LCD
 	 \return void
 */
static void motorControlMenu(SystemDisplayFlags* SDF);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function displays in the LCD, the frequency Menu
 	 \param[in] SDF - Data to take account for displaying in the LCD
 	 \return void
 */
static void frequencyMenu(SystemDisplayFlags* SDF);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 This function according to SDF (currentState), chooses which menu to display
 	 in the LCD
 	 \param[in] SDF - Data to take account for displaying in the LCD
 	 \return void
 */
void update_Display(SystemDisplayFlags* SDF);

#endif /* SOURCES_DISP_H_ */
