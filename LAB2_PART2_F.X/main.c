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
    INITIAL, ENTER, SET_MODE, STATUS
} stateType;

//Volatile variable declarations
volatile stateType state = INITIAL;
volatile unsigned int pressed_C1 = 0, pressed_C2 = 0, pressed_C3 = 0;
volatile unsigned int toggle_C1 = 0, toggle_C2 = 0, toggle_C3 = 0, 
                      Toggle = 0, Hold_check = 0;
volatile unsigned int kp_cnt = 0, current_row = 0, current_col = 0, i = 0,
                      char_index = 0, mode = 0, star_count = 0, password = 0;
char current_char = 0;
// ******************************************************************************************* //

void resetToggle(int command);
void scanAllRows();
void resetRoutine();
int starCheck();
int checkDatabase(char new[], char database[][5]);
void multipleKP_Check();

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
    char new[5];
    char database[4][5];
    int k = 0;
    
    while(1)
    {        
        switch(state){
            case INITIAL:
                turnOnLED(0);
                enableRowScan(-1);
                moveCursorLCD(1,1);
                delaySec(2);
                mode = idle;
                state = ENTER;
                break;
            case ENTER:
                turnOnLED(1);
                if(char_index == 0) {
                    moveCursorLCD(1,1); 
                    printStringLCD("Enter"); moveCursorLCD(1,2); 
                } 
                scanAllRows();
                current_char = scanKeypad(current_row, current_col, kp_cnt);
                //check if a '*' was entered
                if(starCheck()) break;
                //check for 4 valid keys and check/compare with valid passwords
                if(checkDatabase(new, database)) break;
                //if a '#' is ever entered display bad and then restart
                if(current_char != 0){
                    if(current_char != '#'){ 
                        new[char_index] = current_char;
                        char_index++;
                    }else {
                       mode = bad; 
                       char_index = 0;
                       state = STATUS;
                       current_char = 0;
                       break;
                    }
                }
                //checks for multiple key presses
                multipleKP_Check();
                break;
            case STATUS: 
                delayMs(10);
                turnOnLED(2);
                kp_cnt = 0;
                if(mode == bad){
                    resetRoutine();
                    clearLCD(); 
                    moveCursorLCD(1,1);
                    printStringLCD("Bad"); delaySec(2);
                    state = ENTER; break;
                }else if(mode == good){
                    resetRoutine();
                    clearLCD(); 
                    moveCursorLCD(1,1); printStringLCD("Good");delaySec(2);
                    state = ENTER; break;
                }else if(mode == invalid){
                    resetRoutine();
                    clearLCD();
                    moveCursorLCD(1,1); printStringLCD("Invalid"); delaySec(2);
                    clearLCD(); state = ENTER; break;
                }else if(mode == valid){
                    char_index = 0; current_char = 0; resetToggle(0);
                    clearLCD(); 
                    moveCursorLCD(1,1); printStringLCD("Valid"); delaySec(2);
                    state = ENTER; break;
                }
                if(Toggle == 1) printCharLCD(current_char);
                resetToggle(0);
                state = ENTER;
                delayMs(10);
                break;
            case SET_MODE:
                if(char_index == 0) {
                    
                    moveCursorLCD(1,1); printStringLCD("Set Mode");
                    moveCursorLCD(1,2); 
                }
                turnOnLED(-1);
                scanAllRows();
                current_char = scanKeypad(current_row, current_col, kp_cnt);
                if(char_index == 4){
                    char_index = 0;
                    storePassword(new, &database, password);
                    if(password < 3) password++;
                    else password = 0;
                    mode = valid;
                    state = STATUS; break;                    
                }
                if((current_char == '*')||(current_char == '#')){
                    mode = invalid;
                    char_index = 0;
                    state = STATUS; break;
                }else if(current_char != 0){
                    new[char_index] = current_char;
                    char_index++;
                    printCharLCD(current_char);
                    current_char = 0;
                    kp_cnt = 0;
                    resetToggle(0);
                }
            break;
        }
    }
    return 0;
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt(void){
    //----------------------------------------
    int c1,c2,c3;
    
    if((state == ENTER) || (state == SET_MODE)){
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

void resetRoutine(){
    current_char = 0; 
    resetToggle(0); 
    kp_cnt = 0;
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
int starCheck(){
    if((current_char == '*') && (char_index == 0)){
        star_count = 1;
        return 0;
    }else if((star_count == 1)&&(current_char == '*')&&(char_index == 1)){
        star_count = 0;
        char_index = 0;
        resetRoutine();
        state = SET_MODE; clearLCD(); return 1;
    }else if((star_count == 1) && (current_char != 0)){
        star_count = 0;
        mode = bad;
        current_char = 0;
        state = STATUS; return 1;
    }else{
        return 0;
    }
}

int checkDatabase(char new[], char database[][5]){
    int k;
    if(char_index == 4){ 
            char_index = 0; 
            for(k = 0; k < 4; k++){
                if(passwordCheck(new, database, k )){
                    mode = good;
                    state = STATUS; return 1;
                }else if(k == 3){
                    mode = bad;
                    state = STATUS;
                }
            }
            return 1;
    }
    return 0;
}

void multipleKP_Check(){
    if(current_char != 0){
        if((kp_cnt == 1) && (Toggle == 1)) {
            mode = idle; state = STATUS; 
        }
    }else{
        if((kp_cnt > 1) && (Toggle > 1)) {
            resetToggle(0); kp_cnt = 0;
        }
        state = ENTER; 
    }
}


