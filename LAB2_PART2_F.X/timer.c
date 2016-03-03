/*
 * File:   timer.c
 * Authors: Andres D. Rebeil
 *
 * Created on Feb 16, 2016
 */

#include <xc.h>
#include "timer.h"
#include <math.h>
#define prescalar_8 3
#define prescalar_256 3

void initTimer1(){
    TMR1 = 0;
    T1CONbits.ON = 0;   //TURN TIMER ON
    T1CONbits.TCKPS = prescalar_256;
    T1CONbits.TCS = 0;
    IFS0bits.T1IF = 0;
}

void delaySec(unsigned int delay){   
    TMR1 = 0;                  //RESET TIME
    PR1 = delay*19530;         //set PR    
    IFS0bits.T1IF = 0;         // SETS FLAG
    T1CONbits.ON = 1;          //ENABLE CN
    while(IFS0bits.T1IF == 0); //DELAY TILL FLAG IS RAISED
    T1CONbits.ON = 0;          //DISABLE CN
}

void initTimer2(){
    TMR2 = 0;
    T2CONbits.ON = 0;   //TURN TIMER ON
    T2CONbits.TCKPS = prescalar_8;
    T2CONbits.TCS = 0;
    IFS0bits.T2IF = 0;
}

void delayMs(unsigned int delay){
    IFS0bits.T2IF = 0;         // SETS FLAG
    TMR2 = 0;                  //RESET TIME
    PR2 = delay*624;           
    
    T2CONbits.ON = 1;          //ENABLE CN
    while(IFS0bits.T2IF == 0); //DELAY TILL FLAG IS RAISED
    T2CONbits.ON = 0;          //DISABLE CN
}

void delayUs(unsigned int delay){

    //TODO: Create a delay for "delay" micro seconds using timer 2
    IFS0bits.T2IF = 0;         // SETS FLAG
    TMR2 = 0;                  //RESET TIME
    PR2 = delay;           //SET PRESCALAR
   
    T2CONbits.ON = 1;          //ENABLE CN
    while(IFS0bits.T2IF == 0); //DELAY TILL FLAG IS RAISED
    T2CONbits.ON = 0;          //DISABLE CN
}