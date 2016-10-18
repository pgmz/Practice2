/*
 * BTTN.h
 *
 *  Created on: 08/10/2016
 *      Author: Patricio Gomez
 */

#ifndef SOURCES_BTTN_H_
#define SOURCES_BTTN_H_

#include "GPIO.h"
#include "NVIC.h"

/*Replace BUTTON_0 with a 0, this represent the pushbutton BO for state machines*/
#define BUTTON_0	0
/*Replace BUTTON_1 with a 1, this represent the pushbutton B1 for state machines*/
#define BUTTON_1	1
/*Replace BUTTON_2 with a 2, this represent the pushbutton B2 for state machines*/
#define BUTTON_2	2
/*Replace BUTTON_3 with a 3, this represent the pushbutton B3 for state machines*/
#define BUTTON_3	3
/*Replace BUTTON_4 with a 4, this represent the pushbutton B4 for state machines*/
#define BUTTON_4	4
/*Replace BUTTON_5 with a 5, this represent the pushbutton B5 for state machines*/
#define BUTTON_5	5
/*Replace NULL_BUTTON with a 6, this represent noise*/
#define NULL_BUTTON 6

/*rawBUTTON_0 is 32, is the value that is readed from the portC when this button is pressed*/
#define rawBUTTON_0 32
/*rawBUTTON_1 is 16, is the value that is readed from the portC when this button is pressed*/
#define rawBUTTON_1 16
/*rawBUTTON_2 is 8, is the value that is readed from the portC when this button is pressed*/
#define rawBUTTON_2 8
/*rawBUTTON_3 is 4, is the value that is readed from the portC when this button is pressed*/
#define rawBUTTON_3 4
/*rawBUTTON_4 is 2, is the value that is readed from the portC when this button is pressed*/
#define rawBUTTON_4 2
/*rawBUTTON_5 is 1, is the value that is readed from the portC when this button is pressed*/
#define rawBUTTON_5 1


/*Type of data that represent a binary semaphore with two variables that represent a status flag and information*/
typedef struct{
	uint8 flag :1;
	uint16 mailBoxData;
}BTTN_MailBoxType;
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	This function configures the port C and PINs 0,1,5,7,8 and 9 as GPIO input, also
 	 enable the interruption in the port C, set a level of priority 8 and enable the interruption
 	 \return void
 */
void BTTN_init();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	This function return the value of the flag of struct BTTN_MailBOxType
 	 \return This Function return 0 if the value of flag is FALSE or 1 if the value of the flag is TRUE
 */
uint8 BTTN_mailBoxFlag();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	This function set the value of flag in false and clean the signal received from the pushbuttons from
 	  	  	the port C
 	 \return This Function return 0 if the value of mailBoxdata is 32, return 1 if the value of mailBoxData is 16,
 	 	 	 return 3 if the value of mailBoxData is 4, return 4 if the value of mailBoxData is 2, return 5 if the
 	 	 	 value of mailBoxData is 1 or return 6 if the value of mailBoxData has a different value than ones
 	 	 	 already mentiones


 */
uint16 BTTN_mailBoxData();


#endif /* SOURCES_BTTN_H_ */
