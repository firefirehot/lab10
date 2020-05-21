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
	enum SF_State{SF_START,SF_HIGH, SF_LOW}SF_state;
	enum BP_State{BP_START,BP_BASE,BP_B0,BP_B1}BP_state;
	
	unsigned char threeLEDs;
	unsigned char blinkingLED;
	unsigned char speakerFrequency;
	unsigned char speakerSlide = 0;
		
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
			//if((!PINA & 0X04) == 1)
				blinkingLED = 0x08;
			//else
			//	blinkingLED = 0x00;
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
			PORTB = ((blinkingLED | threeLEDs) | speakerFrequency);
			break;

	}
	}
	void BP_Tick(){
	switch(BP_state){
	       case BP_START:
		       BP_state = BP_BASE;
		break;
	       case BP_BASE:
			if((~PINA & 0x01) == 0x01){
				BP_state = BP_B0;
				if(speakerSlide > -1)
				speakerSlide--;
			}
			else if((~PINA & 0x02) == 0x02){
				BP_state = BP_B1;
				speakerSlide++;
			}
			else
				BP_state = BP_BASE;
		break;
	       case BP_B0:
			if((~PINA & 0x01) == 0)
				BP_state = BP_BASE;
			else
				BP_state = BP_B0;
		break;
	       case BP_B1:
                        if((~PINA & 0x02) == 0)
                                BP_state = BP_BASE;
                        else
                                BP_state = BP_B1;
		break;
	}
        switch(BP_state){
               case BP_START:
                break;
               case BP_BASE:
                break;
               case BP_B0:
                break;
               case BP_B1:
                break;
        }

	}



	void SF_Tick(){
        switch(SF_state){
                case SF_START:
                        SF_state = SF_HIGH;
                        break;
                case SF_HIGH:
                        SF_state = SF_LOW;
                        break;
                case SF_LOW:
                        SF_state = SF_HIGH;
                        break;
        }
        switch(SF_state){
                case SF_START:
                        break;
                case SF_HIGH:
			if((~PINA & 0x04) == 0)
                        	speakerFrequency = 0x00;
			else
				speakerFrequency = 0x10;
                        break;
                case SF_LOW:
                        speakerFrequency = 0x00;
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
	unsigned long SF_elapsedTime = 0;
	unsigned long BP_elapsedTime = 0;

	const unsigned long timerPeriod = 2;
	

	TimerSet(timerPeriod);
	TimerOn();

	TL_state = TL_START;
	BL_state = BL_START;
	CL_state = CL_START;
	SF_state = SF_START;
	BP_state = BP_START;
	speakerSlide = 0;

    while (1) {
	if(TL_elapsedTime >= 300) {
	       	TL_Tick();
		TL_elapsedTime = 0;
	} if(BL_elapsedTime >= 1000) { 
	       	BL_Tick(); 
	       	BL_elapsedTime = 0; 
	}if(SF_elapsedTime >= (2+speakerSlide)){
                SF_Tick();
                SF_elapsedTime = 0;
        }if(BP_elapsedTime >= 100){
                BP_Tick();
                BP_elapsedTime = 0;
        }if(CL_elapsedTime >= 1){
		CL_Tick();
		CL_elapsedTime = 0;
	}
	while(!TimerFlag){}
	TimerFlag = 0;
	TL_elapsedTime += timerPeriod;
	BL_elapsedTime += timerPeriod;
	CL_elapsedTime += timerPeriod;
	SF_elapsedTime += timerPeriod;
	BP_elapsedTime += timerPeriod;
    }
    return 1;
}
