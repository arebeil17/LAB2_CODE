/*
 * File:   lcd.c
 * Authors: Andres D. Rebeil
 *
 * Created on Feb 16, 2016
 */

/* TODO: Make define statements for each pin used in the LCD
 */
#include <xc.h>
#include "lcd.h"
#include "timer.h"
#include <math.h>

/* The pins that you will use for the lcd control will be
 * LCD_RS      RC4
 * LCD_E       RC2
 * LCD_D4      RE7
 * LCD_D5      RE5
 * LCD_D6      RE3
 * LCD_D7      RE1
 * Use these so that we will be able to test your code for grading and to
 * help you debug your implementation!
 */


/* This function should take in a two-byte word and writes either the lower or upper
 * byte to the last four bits of LATE. Additionally, according to the LCD data sheet
 * It should set LCD_RS and LCD_E with the appropriate values and delays.
 * After these commands are issued, there should be a delay.
 * The command type is a simplification. From the data sheet, the RS is '1'
 * when you are simply writing a character. Otherwise, RS is '0'.
 */
void writeFourBits(unsigned char word, unsigned int commandType, unsigned int delayAfter, unsigned int lower){
    //TODO:
     // set the commandType (RS value)
     LCD_RS = commandType;
    
     if(lower){
        LCD_D4 = word&0x01;
        LCD_D5 = (word&0x02)>>1;
        LCD_D6 = (word&0x04)>>2;
        LCD_D7 = (word&0x08)>>3;
     } else
     {
        LCD_D4 = (word&0x10)>>4;
        LCD_D5 = (word&0x20)>>5;
        LCD_D6 = (word&0x40)>>6;
        LCD_D7 = (word&0x80)>>7;
     }
    //enable
    LCD_E = Enable;
    
    //delay
    delayUs(5);
   
    //disable
    LCD_E = Disable;
    
    if((lower) && (delayAfter != 0)) 
    {   
        delayUs(delayAfter);
    }
}

/* Using writeFourBits, this function should write the two bytes of a character
 * to the LCD.
 */
void writeLCD(unsigned char word, unsigned int commandType, unsigned int delayAfter){
    //TODO:
    writeFourBits(word,commandType, delayAfter, 0);
    writeFourBits(word,commandType, delayAfter, 1);
}

/* Given a character, write it to the LCD. RS should be set to the appropriate value.
 */
void printCharLCD(char c) {
    //TODO:
    writeLCD(c,1,50);
}
/*Initialize the LCD
 */
void initLCD(void) {
    // Setup D, RS, and E to be outputs (0).
    TRIS_D4 = OUTPUT;
    TRIS_D5 = OUTPUT;
    TRIS_D6 = OUTPUT;
    TRIS_D7 = OUTPUT;
    TRIS_E = OUTPUT;
    TRIS_RS = OUTPUT;
    // Initialization sequence utilizes specific LCD commands before the general configuration
    // commands can be utilized. The first few initialization commands cannot be done using the
    // WriteLCD function. Additionally, the specific sequence and timing is very important.
     
      delayMs(20);
    // Enable 4-bit interface
      writeFourBits(0x03, 0, 4100, 1); //0000 0011
      
      writeFourBits(0x03, 0, 100, 1);  //0000 0011
      
      writeFourBits(0x03, 0, 40, 1);    //0000 0011
      
      //Function Set 4 bit
      writeFourBits(0x20, 0, 40, 0); //0000 0010
      //Function Set
      writeFourBits(0x20,0,0,0); // 0010 0000
      writeFourBits(0x08,0,40,1); //0000 1000
   
      // Turn Display (D) Off
      writeFourBits(0x00,0,100,0); // 0000 0000
      writeFourBits(0x08,0,40,1); // 0000 1000
      //Clear Display
      writeFourBits(0x00,0,0,0); // 0000 0000
      writeFourBits(0x01,0,1800,1); // 0000 0001
      //Entry Mode set
      writeFourBits(0x00,0,0,0); // 0000 0000
      writeFourBits(0x06,0,40,1); // 0000 0110
      //Display On/Off Control
      // Turn Display (D) On, Cursor (C) Off, and Blink(B) Off
      writeFourBits(0x00,0,0,0); // 0000 0000
      writeFourBits(0x0C,0,40,1); // 0000 1100
}

/*
 * You can use printCharLCD here. Note that every time you write a character
 * the cursor increments its position automatically.
 * Since a string is just a character array, try to be clever with your use of pointers.
 */
void printStringLCD(const char* s) {
    //TODO:
    int i;
    char c;
    unsigned int size = strlen(s);
    
        for(i = 0; i < size; i++)
        {   
            c = s[i];
            printCharLCD(c);
        } 
}

/*
 * Clear the display.
 */
void clearLCD(){
    
    //moveCursorLCD(1,1);
    writeFourBits(0x00, 0, 0 , 0);
    writeFourBits(0x01, 0, 1800 , 1);
}

/*
 Use the command for changing the DD RAM address to put the cursor somewhere.
 */
void moveCursorLCD(unsigned char x, unsigned char y){
    
    char command = 0x80 | (y - 1)*0x40 + x - 1; 
    writeLCD(command, 0, 40);
}

/*
 * This function is called in lab1p2.c for testing purposes.
 * If everything is working properly, you should get this to look like the video on D2L
 * However, it is suggested that you test more than just this one function.
 */
void testLCD(){
    initLCD();
    int i = 0;
    printCharLCD('c');
    for(i = 0; i < 1000; i++) delayMs(1);
    clearLCD();
    printStringLCD("Hello!");
    moveCursorLCD(1, 2);
    for(i = 0; i < 1000; i++) delayMs(1);
    printStringLCD("Hello!");
    for(i = 0; i < 1000; i++) delayMs(1);    
}

void getString(int Rst, int i){
    
    
    const char *T[9];
    unsigned int M2,M1,S2,S1,Ms1,Ms2;
    
    
    if(Rst) printStringLCD("00:00:00");
    else{
        T[0] = M2 = floor(i/600000);
        T[0] = T[0] + '0';
        printCharLCD(T[0]);
        T[1] = M1 = floor( (i - M2*600000)/60000);
        T[1] = T[1] + '0';
        printCharLCD(T[1]);
        T[2] = ':';
        printCharLCD(T[2]);
        T[3] = S2 = floor((i - M2*600000 - M1*60000)/10000);
        T[3] = T[3] + '0';
        printCharLCD(T[3]);
        T[4] = S1 = floor((i - M2*600000 - M1*60000 - S2*10000)/1000);
        T[4] = T[4] + '0';
        printCharLCD(T[4]);
        T[5] = ':';
        printCharLCD(T[5]);
        T[6] = Ms2 = floor((i - M2*600000 - M1*60000 - S2*10000 - S1*1000)/100);
        T[6] = T[6] + '0';
        printCharLCD(T[6]);
        T[7] = Ms1 = floor((i - M2*600000 - M1*60000 - S2*10000 - S1*1000 - Ms2*100)/10);
        T[7] = T[7] + '0';
        printCharLCD(T[7]);
     
   
    }
}
