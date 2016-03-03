#include <xc.h>
#include "keypad.h"
#include "timer.h"
#include <stdlib.h>

#define ENABLE_ODC 0
#define DISABLE_ODC 1

void initKeypad(void){
    
    TRIS_ROW1 = OUTPUT;
    TRIS_ROW2 = OUTPUT;
    TRIS_ROW3 = OUTPUT;
    TRIS_ROW4 = OUTPUT;
    ODCEbits.ODCE6 = ENABLED;
    ODCEbits.ODCE4 = ENABLED;
    ODCEbits.ODCE2 = ENABLED;
    ODCEbits.ODCE0 = ENABLED;
    
    TRIS_COL1 = INPUT;                  //Configure RD4 as input 
    CNCONDbits.ON = ENABLED;            // Enable overall interrupt
    CNENDbits.CNIED4 = ENABLED;         // Enable pin CN
    CNPUDbits.CNPUD4 = ENABLED;         // Enable pull-up resistor
    IFS1bits.CNDIF = 0;                 // Put down the flag
    IPC8bits.CNIP = 7;                  // Configure interrupt priority
    IPC8bits.CNIS = 3;                  // Configure the interrupt sub-priority
    IEC1bits.CNDIE = ENABLED;           // Enable interrupt for D pins
    
    TRIS_COL2 = INPUT;                  //Configure RD10 as input
    CNCONDbits.ON = 1;                  // Enable overall interrupt
    CNENDbits.CNIED10 = ENABLED;        // Enable pin CN
    CNPUDbits.CNPUD10 = ENABLED;        // Enable pull-up resistor
    IFS1bits.CNDIF = 0;                 // Put down the flag
    IPC8bits.CNIP = 7;                  // Configure interrupt priority
    IPC8bits.CNIS = 3;                  // Configure the interrupt sub-priority
    IEC1bits.CNDIE = ENABLED;           // Enable interrupt for D pins
    
    TRIS_COL3 = INPUT;                  //Configure RC1 as input
    CNCONCbits.ON = ENABLED;            // Enable overall interrupt
    CNENCbits.CNIEC1 = ENABLED;         // Enable pin CN
    CNPUCbits.CNPUC1 = ENABLED;         // Enable pull-up resistor
    IFS1bits.CNCIF = 0;                 // Put down the flag
    IPC8bits.CNIP = 7;                  // Configure interrupt priority
    IPC8bits.CNIS = 3;                  // Configure the interrupt sub-priority
    IEC1bits.CNCIE = ENABLED;           // Enable interrupt for C pins
    
}

void enableRowScan(unsigned int scan){
    if(scan == 0){
        ROW1 = ENABLE_ODC;
        ROW2 = DISABLE_ODC;
        ROW3 = DISABLE_ODC;
        ROW4 = DISABLE_ODC;
    }else if(scan == 1){
        ROW1 = DISABLE_ODC;
        ROW2 = ENABLE_ODC;
        ROW3 = DISABLE_ODC;
        ROW4 = DISABLE_ODC;
    }else if(scan == 2){
        ROW1 = DISABLE_ODC;
        ROW2 = DISABLE_ODC;
        ROW3 = ENABLE_ODC;
        ROW4 = DISABLE_ODC;  
    }else if(scan == 3){
        ROW1 = DISABLE_ODC; 
        ROW2 = DISABLE_ODC;
        ROW3 = DISABLE_ODC; 
        ROW4 = ENABLE_ODC;
    }else{
        ROW1 = DISABLE_ODC; 
        ROW2 = DISABLE_ODC;
        ROW3 = DISABLE_ODC; 
        ROW4 = DISABLE_ODC;  
    } 
}

/* This function will be called AFTER you have determined that someone pressed
 * SOME key. This function is to figure out WHICH key has been pressed.
 * This function should return -1 if more than one key is pressed or if
 * no key is pressed at all. Otherwise, it should return the ASCII character of
 * the key that is pressed. The ascii character c programmatically is just 'c'
 */
char scanKeypad(int row, int col, int presses){
    
    int ROWS = 4; //four rows
    int COLS = 3; //three columns
    
    char keypad[ROWS][COLS];
    keypad[0][0] = '1'; keypad[0][1] = '2'; keypad[0][2] = '3';
    keypad[1][0] = '4'; keypad[1][1] = '5'; keypad[1][2] = '6';
    keypad[2][0] = '7'; keypad[2][1] = '8'; keypad[2][2] = '9';
    keypad[3][0] = '*'; keypad[3][1] = '0'; keypad[3][2] = '#';
    
    if(presses != 1) return 0; // multiple key presses return 0
    else return keypad[row][col];
}
int passwordCheck(char suspect[], char stored[][5], int row){ 
    int k = 0;
    
    for(k = 0; k < 4; k++){
        if(suspect[k] != stored[row][k]) return no_match;
    }
    return match;
}

void storePassword(char set[], char (*database)[5], int row){
    int k = 0;
    
    for(k = 0; k < 4; k++){
        database[row][k] = set[k];
    }
}
