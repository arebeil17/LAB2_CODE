/* 
 * File:   keypad.h
 * Author: Andres D. Rebeil
 *
 * Created on February 5, 2015, 11:42 AM
 */

#ifndef KEYPAD_H
#define	KEYPAD_H

/* Initialize the rows as ODC outputs and the columns as inputs with pull-up
 * resistors. Don't forget about other considerations...
 */
//ODC OUTPUTS FOR KEYPAD ROWS
 #define TRIS_ROW1 TRISEbits.TRISE6  // RE6  PIN 6   KEYPAD ROW 2
 #define TRIS_ROW2 TRISEbits.TRISE4  // RE4  PIN 8   KEYPAD ROW 7
 #define TRIS_ROW3 TRISEbits.TRISE2  // RE2  PIN 10  KEYPAD ROW 6
 #define TRIS_ROW4 TRISEbits.TRISE0  // RE0  PIN 12  KEYPAD ROW 4
//INPUTS WITH PICK-UP RESISTORS FOR KEYPAD COLUMNS
 #define TRIS_COL1 TRISDbits.TRISD4  // RD4  PIN 14  KEYPAD COLUMN 3
 #define TRIS_COL2 TRISDbits.TRISD10 // RD10 PIN 16  KEYPAD COLUMN 1
 #define TRIS_COL3 TRISCbits.TRISC1  // RC1  PIN 18  KEYPAD COLUMN 5
//FOR SETTING TRISTATE REGISTERS
 #define INPUT 1
 #define OUTPUT 0
 #define ENABLED 1
 #define DISABLED 0
//OUTPUT LATCHES FOR ROWS: ENABLED FOR ROW SCAN
#define ROW1 LATEbits.LATE6
#define ROW2 LATEbits.LATE4
#define ROW3 LATEbits.LATE2
#define ROW4 LATEbits.LATE0
//INPUT PORTS FOR COLUMNS
#define COL1 PORTDbits.RD4
#define COL2 PORTDbits.RD10
#define COL3 PORTCbits.RC1



void initKeypad(void);

void enableRowScan(unsigned int scan);

char scanKeypad(int row, int col, int presses);

#endif	/* KEYPAD_H */

