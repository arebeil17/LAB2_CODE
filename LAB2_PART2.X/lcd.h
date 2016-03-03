/*
 * File:   lcd.h
 * Authors: Andres D. Rebeil
 *
 * Created on Feb 16, 2016
 */
#define Enable 1
#define Disable 0

#define LCD_DATA  
#define LCD_RS LATCbits.LATC4
#define LCD_E  LATCbits.LATC2

#define OUTPUT 0
#define INPUT 1

#define TRIS_D7 TRISEbits.TRISE1
#define TRIS_D6 TRISEbits.TRISE3
#define TRIS_D5 TRISEbits.TRISE5
#define TRIS_D4 TRISEbits.TRISE7
#define TRIS_RS TRISCbits.TRISC4
#define TRIS_E  TRISCbits.TRISC2

#define LCD_D7 LATEbits.LATE1
#define LCD_D6 LATEbits.LATE3
#define LCD_D5 LATEbits.LATE5
#define LCD_D4 LATEbits.LATE7

//LCD COMMANDS
#define display_On x0D   //0000 1101
#define display_Off x09  //0000 1001
#define home x02         //0000 0010
#define clear x01        //0000 0001

void writeLCD(unsigned char word, unsigned int commandType, unsigned int delay);
void writeFourBits(unsigned char word, unsigned int commandType, unsigned int delayAfter, unsigned int lower);
void initLCD(void);
void clearLCD(void);
void moveCursorLCD(unsigned char x, unsigned char y);
void printCharLCD(char c);
void printStringLCD(const char* s);
void testLCD();
void getString(int Rst, int i);
//void outputLCD(int stateNum, int i);