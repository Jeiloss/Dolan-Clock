#include "msp.h"
#include "portfunc.h"
#include "clock.h"

unsigned int qSec=0;
unsigned int minute=0;//PROPOSAL: change to track 5 minutes.
unsigned int hour=0;
unsigned short hourPatterns[20];
unsigned int hourPatternsPointer=0;

void InitLED2 (void){
    P2DIR|=(BIT0|BIT1|BIT2);
    SelectPortFunction(2,0,0,0);
//    SelectPortFunction(2,1,0,0);//need more than 1?
//    SelectPortFunction(2,2,0,0);//Shouldn't just 1 timer be fine?
    P2OUT &=~(BIT0|BIT1|BIT2);//turns light off.
}

void InitLED1 (void) {
    P1DIR |=BIT0;
    SelectPortFunction(1,0,0,0);
    P1OUT &= ~BIT0;
}

BlackLight(){
    P2OUT&=~(BIT0|BIT1|BIT2);
}
WhiteLight(){
    P2OUT|=(BIT0|BIT1|BIT2);
}
LightColor(int min/*maybe just 1/2 min?*/) {//No 1/2 min, because we can just x%5.
    switch(min%6){
    case 0:
        BlackLight();
        P2OUT|=BIT0;
        break;
    case 1:
        BlackLight();
        P2OUT|=(BIT1 | BIT0);
        break;
    case 2:
        BlackLight();
        P2OUT|=(BIT1);
        break;
    case 3:
        BlackLight();
        P2OUT|=(BIT2 | BIT1);
        break;
    case 4:
        BlackLight();
        P2OUT|=(BIT2);
        break;
    case 5:
        BlackLight();
        P2OUT|=(BIT2 | BIT0);
        break;
    }
}

int FlashLong(int i) {
    hourPatterns[i] = 1;i++;
    hourPatterns[i] = 1;i++;
    hourPatterns[i] = 0;i++;
//    hourPatterns[i] = 0;i++;

    return 3;
}
int FlashShort(int i) {
    hourPatterns[i] = 1;i++;
    hourPatterns[i] = 0;i++;
//    hourPatterns[i] = 0;i++;

    return 2;
}
void HourPattern(void) {
    short x = hour;
    int i = 0;

    while (x >0) {
        if (x>=6) {
            x-=6;
            i += FlashLong(i);
//            hourPatterns[i] = 1;i++;
//            hourPatterns[i] = 1;i++;
////            hourPatterns[i] = 0;i++;
//            hourPatterns[i] = 0;i++;
        } else {
            x--;
            i += FlashShort(i);
//            hourPatterns[i] = 1;i++;
////            hourPatterns[i] = 0;i++;
//            hourPatterns[i] = 0;i++;
        }
    }
    hourPatterns[i] = 0;i++;
    hourPatterns[i] = 0;i++;
    hourPatterns[i] = 0;i++;
    hourPatterns[i] = 0;i++;


    hourPatterns[i]=2;
}

void IncrHour(void) {
    hour++;
    HourPattern();
}

/**Note: qSec is being incremented before ManageThyme is called,
 *       so don't worry that it is not being implemented here.
 *
 * Idea: Change blinking speed to 1-sec. ~Kyle
 */
void ManageThyme(void){

//Handle Minute Color
    if (minute>5){
        if (qSec % 2 == 1){
                LightColor(minute);
            } else{
                WhiteLight();
            }
    } else{
        if (qSec % 2 == 1){
                LightColor(minute);
            } else{
                BlackLight();
            }
    }

//Handle Hour Color
    if (hourPatterns[hourPatternsPointer] == 2) {
        hourPatternsPointer=0;
    }
    if (hourPatterns[hourPatternsPointer] == 0) {
        P1OUT &= ~BIT0;
    } else if (hourPatterns[hourPatternsPointer] == 1) {
        P1OUT |= BIT0;
    }
    hourPatternsPointer++;

//Handle minute Inc.
//    if (qSec>=4){
//        minute++; qSec=0;
//    }
//Handle Hour Inc.
    if (qSec>=/*60*5**/4){//change minute color!
        minute++; qSec=0;
        if (minute>=11) {//change hour pattern!
            minute=0;
            IncrHour();
            if (hour>=24) {
                hour=0;
            }
        }
    }
}

void TimerA0Interrupt(void) {
    unsigned short intv = TA0IV;
    qSec++;
    ManageThyme();
}

void ConfigureTimer (void) {
    TA0CTL = 0x0100;
    TA0CCTL0 = 0x2000;
//    TA0CCR0 = 0x3e80;//half-sec
    TA0CCR0 = 0x1f40;//quar-sec
    TA0CTL = 0x0116;
}
void PortOneInterrupt (void) {

}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer
    NVIC_EnableIRQ(TA0_N_IRQn);
    InitLED2();
    InitLED1();
    ConfigureTimer();
    HourPattern();
    for(;;);
}
