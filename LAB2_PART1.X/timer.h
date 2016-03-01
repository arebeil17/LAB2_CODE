/* 
 * File:   timer.h
 * Authors: Andres D. Rebeil
 *
 * Created on Feb 16, 2016
 */

#ifndef INITTIMER_H
#define	INITTIMER_H

//Used for long delays in seconds
void initTimer1();

//User for short delays micro and milli-seconds
void initTimer2();

//Maximum delay of 3 sec. per call
void delaySec(unsigned int delay);

//Maximum of 100 ms per call
void delayMs(unsigned int delay);

//Maximum of 100 ms per call
void delayUs(unsigned int delay);


#endif	/* INITTIMER_H */

