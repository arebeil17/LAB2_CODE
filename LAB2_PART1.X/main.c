// ******************************************************************************************* //
//
// File:         lab1p2.c
// Date:         Feb 16, 2016
// Authors:      Andres D. Rebeil
//
// Description: 
// ******************************************************************************************* //

#include <xc.h>
#include <sys/attribs.h>
#include "lcd.h"
#include "timer.h"
#include "leds.h"
#include "keypad.h"
#include "config.h"
#include "interrupt.h"

typedef enum stateTypeEnum{
    INITIAL, SCAN, DISPLAY, WAIT
} stateType;

//Volatile variable declarations
volatile stateType state = INITIAL;
volatile unsigned int pressed_C1 = 0, pressed_C2 = 0, pressed_C3 = 0;
volatile unsigned int toggle_C1 = 0, toggle_C2 = 0, toggle_C3 = 0, 
                      Toggle = 0, Hold_check = 0;
volatile unsigned int kp_cnt = 0, current_row = 0, current_col = 0, i = 0;
// ******************************************************************************************* //
#define pressed 0
#define released 1

void resetToggle(int command);

void scanAllRows();

int main(void)
{
    SYSTEMConfigPerformance(10000000);
    enableInterrupts();
    
    //INITIALIZATION PROCESS
    initLEDs();
    initTimer1();
    initTimer2();
    initLCD();
    delayMs(10);
    initKeypad();
    //INITIALIZATION COMPLETE
    char current_char = 'c';
    
    while(1)
    {        
        switch(state){
            case INITIAL:
                turnOnLED(0);
                enableRowScan(-1);
                moveCursorLCD(1,1);
                delaySec(2);
                state = SCAN;
                break;
            case SCAN:
                turnOnLED(1);
                scanAllRows();
                current_char = scanKeypad(current_row, current_col, kp_cnt);
                if(current_char != 0){
                    if((kp_cnt == 1) && (Toggle == 1)) state = DISPLAY;
                }else{
                    if((kp_cnt > 1) && (Toggle > 1)) {
                        resetToggle(0);
                        kp_cnt = 0;
                    }
                    state = SCAN; 
                }
                break;
            case DISPLAY:
                delayMs(10);
                turnOnLED(2);
                if(Toggle == 1) printCharLCD(current_char); 
                resetToggle(0); kp_cnt = 0;
                state = WAIT;
                delayMs(10);
                break;
            case WAIT:
                turnOnLED(-1);
                delayMs(10);
                resetToggle(0);
                enableRowScan(-1);
                if((Toggle == 1) || (Hold_check != 0)) state = WAIT;
                else state = SCAN;
            break;
        }
    }
    return 0;
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(void){
    //----------------------------------------
    int c1,c2,c3;
    
    if((state == SCAN)){
        PORTD;
        PORTC;
        c1 = COL1;
        c2 = COL2;
        c3 = COL3;

        //COLUMN 1
        if(COL1 == pressed){
            current_row = i;
            pressed_C1 = 1;
            kp_cnt++;
        }else if((COL1 == released) && (pressed_C1 == 1)){
            pressed_C1 = 0;
            toggle_C1 = 1;
            current_col = 0;//with respect to array index    
        }
        //COLUMN 2
        if(COL2 == pressed){
            current_row = i;
            pressed_C2 = 1;
            kp_cnt++;
        }else if((COL2 == released) && (pressed_C2 == 1)){
            pressed_C2 = 0;
            toggle_C2 = 1;
            current_col = 1;//with respect to array index
        }
        //COLUMN 3
        if(COL3 == pressed){
            current_row = i;
            pressed_C3 = 1;
            kp_cnt++;
        }else if((COL3 == released) && (pressed_C3 == 1)){
            pressed_C3 = 0;
            toggle_C3 = 1;
            current_col = 2;//with respect to array index
        } 
    }  
        Toggle = toggle_C1 + toggle_C2 + toggle_C3;
        Hold_check = pressed_C1 + pressed_C2 + pressed_C3;
    //RESET FLAGS
    IFS1bits.CNDIF = 0; //FLAG DOWN
    IFS1bits.CNCIF = 0; //FLAG DOWN
}

void resetToggle(int command){
    if(command == 0){ //reset all column toggles
        toggle_C1 = 0;
        toggle_C2 = 0;
        toggle_C3 = 0;
        Toggle = 0;
    }else{ // reset master toggle only
        Toggle = 0;
    } 
}

void scanAllRows(){
    i = 0;
    if(Toggle == 0) { enableRowScan(0); delayMs(10);} 
    else{ enableRowScan(-1); return;}
    
    while(Hold_check != 0) delayMs(1);
    
    i = 1;
    if(Toggle == 0) { enableRowScan(1); delayMs(10);} 
    else{ enableRowScan(-1); return;}
    
    while(Hold_check != 0) delayMs(1);
    
    i = 2;
    if(Toggle == 0) { enableRowScan(2); delayMs(10);} 
    else{ enableRowScan(-1); return;}
    
    while(Hold_check != 0) delayMs(1);
    
    i = 3;
    if(Toggle == 0) { enableRowScan(3); delayMs(10);} 
    else{ enableRowScan(-1); return;}
    
    while(Hold_check != 0) delayMs(1);
 
    enableRowScan(-1);
}
