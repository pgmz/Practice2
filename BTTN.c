/*
 * BTTN.c
 *
 *  Created on: 08/10/2016
 *      Author: Patricio Gomez
 */

#include "BTTN.h"
#include "NVIC.h"
#include "GlobalFunctions.h"

/*BTTN_MailBoxType to store the button pressed and the flag of data available*/
BTTN_MailBoxType BTTN_MailBox = {
		FALSE,
		0
};

/*Intialize the BTTN*/
void BTTN_init(){


	/*Enable the clock gating for PORTC*/
	GPIO_clockGating(GPIOC);

	/*Sets the configuration needed to receive an interruption when there is data available*/
	GPIO_pinControlRegisterType pinControlRegisterPORTC = GPIO_MUX1|INTR_RISING_EDGE;
	/*Sets PORT B pin 20, with the functionality of MUX_1  and the interruption actives with the rising edge*/

	GPIO_pinControlRegister(GPIOC,BIT5,&pinControlRegisterPORTC); //B0
	GPIO_pinControlRegister(GPIOC,BIT7,&pinControlRegisterPORTC); //B1
	GPIO_pinControlRegister(GPIOC,BIT0,&pinControlRegisterPORTC);//B2
	GPIO_pinControlRegister(GPIOC,BIT9,&pinControlRegisterPORTC);//B3
	GPIO_pinControlRegister(GPIOC,BIT8,&pinControlRegisterPORTC);//B4
	GPIO_pinControlRegister(GPIOC,BIT1,&pinControlRegisterPORTC);//B5

	GPIO_dataDirectionPIN(GPIOC,GPIO_INPUT,BIT5);//B0
	GPIO_dataDirectionPIN(GPIOC,GPIO_INPUT,BIT7);//B1
	GPIO_dataDirectionPIN(GPIOC,GPIO_INPUT,BIT0);//B2
	GPIO_dataDirectionPIN(GPIOC,GPIO_INPUT,BIT9);//B3
	GPIO_dataDirectionPIN(GPIOC,GPIO_INPUT,BIT8);//B4
	GPIO_dataDirectionPIN(GPIOC,GPIO_INPUT,BIT1);//B5

	/*Enable interruptions*/
	NVIC_enableInterruptAndPriority(PORTC_IRQ, PRIORITY_8);
}

void PORTC_IRQHandler(){
	/**/
	BTTN_MailBox.mailBoxData = GPIO_readPIN(GPIOC, BIT5) << 5| GPIO_readPIN(GPIOC, BIT7) << 4| GPIO_readPIN(GPIOC, BIT0) << 3
			|GPIO_readPIN(GPIOC, BIT9) << 2| GPIO_readPIN(GPIOC, BIT8) << 1| GPIO_readPIN(GPIOC, BIT1) ;

	BTTN_MailBox.flag = TRUE;
	/*Double check of the interruption*/
	/*Clear the interruption flags*/
	GPIO_clearInterrupt(GPIOC);
	/*Digital Debouncer*/
	delay(100);
}

/*return the flag*/
uint8 BTTN_mailBoxFlag(){

	return BTTN_MailBox.flag;
}

/*return the captured button*/
uint16 BTTN_mailBoxData(){

	/*clear flag*/
	BTTN_MailBox.flag = FALSE;

	/*return button*/
	switch(BTTN_MailBox.mailBoxData){
	case rawBUTTON_0:
		return BUTTON_0;

	case rawBUTTON_1:
		return BUTTON_1;

	case rawBUTTON_2:
		return BUTTON_2;

	case rawBUTTON_3:
		return BUTTON_3;

	case rawBUTTON_4:
		return BUTTON_4;

	case rawBUTTON_5:
		return BUTTON_5;

	default:
		return NULL_BUTTON;
	}


}
