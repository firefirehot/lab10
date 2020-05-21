/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif


        enum TL_States{TL_START,TL_LIGHT1,TL_LIGHT2,TL_LIGHT3}TL_state;
        enum BL_States{BL_START,BL_HIGH,BL_LOW}BL_state;
        enum CL_States{CL_START,CL_COMBINE}CL_state;
	
	unsigned char threeLEDs;
	unsigned char blinkingLED;

        void TL_Tick(){
	switch(TL_state){
		case TL_START:
			TL_state = TL_LIGHT1;
			break;
		case TL_LIGHT1:
			TL_state = TL_LIGHT2;
			break;
		case TL_LIGHT2:
			TL_state = TL_LIGHT3;
			break;
		case TL_LIGHT3:
			TL_state = TL_LIGHT1;
			break;
	}
	switch(TL_state){
                case TL_START:
			break;
                case TL_LIGHT1:
			threeLEDs = 0x01;
			break;
                case TL_LIGHT2:
			threeLEDs = 0x02;
			break;
                case TL_LIGHT3:
			threeLEDs = 0x04;
			break;
        }

	}
        void BL_Tick(){
	switch(BL_state){
		case BL_START:
			BL_state = BL_HIGH;
			break;
		case BL_HIGH:
			BL_state = BL_LOW;
			break;
		case BL_LOW:
			BL_state = BL_HIGH;
			break;
	}
	switch(BL_state){
                case BL_START:
			break;
                case BL_HIGH:
			blinkingLED = 0x08;
			break;
                case BL_LOW:
			blinkingLED = 0x00;
			break;
        }

	}
        void CL_Tick(){
	switch(CL_state){
                case CL_START:
			CL_state = CL_COMBINE;
                case CL_COMBINE:
			CL_state = CL_COMBINE;
        }
        switch(CL_state){
                case CL_START:
			break;
                case CL_COMBINE:
			PORTB = (blinkingLED | threeLEDs);
			break;

	}
	}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */

	unsigned long TL_elapsedTime = 0;
	unsigned long BL_elapsedTime = 0;
	unsigned long CL_elapsedTime = 0;
	const unsigned long timerPeriod = 100;

	TimerSet(timerPeriod);
	TimerOn();

	TL_state = TL_START;
	BL_state = BL_START;
	CL_state = CL_START;

    while (1) {
	if(TL_elapsedTime >= 300) {
	       	TL_Tick();
		TL_elapsedTime = 0;
	} if(BL_elapsedTime >= 1000) { 
	       	BL_Tick(); 
	       	BL_elapsedTime = 0; 
	} if(CL_elapsedTime >= 100){
		CL_Tick();
		CL_elapsedTime = 0;
	}
	while(!TimerFlag){}
	TimerFlag = 0;
	TL_elapsedTime += timerPeriod;
	BL_elapsedTime += timerPeriod;
	CL_elapsedTime += timerPeriod;
    }
    return 1;
}
