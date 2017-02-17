#include "msp.h"
#include "portfunc.h"
#include "clock.h"

unsigned int qSec=0;
unsigned int minute=0;//PROPOSAL: change to track 5 minutes.
unsigned int hour=0;

void InitLED (void) {
    P2DIR|=(BIT0|BIT1|BIT2);
    SelectPortFunction(2,0,0,0);
    SelectPortFunction(2,1,0,0);//need more than 1?
    SelectPortFunction(2,2,0,0);//Shouldn't just 1 timer be fine?
    P2OUT|=(BIT0|BIT1|BIT2);
    P2OUT&=BIT2;
}
//struct color {
//    unsigned short red;
//    unsigned short green;
//    unsigned short blue;
//};

struct colourPair {
    unsigned short thirty;
    unsigned short minuteCount;

};
struct colourPair colorarray[12]={
                              {000,001}, //Red
                              {000,011}, //Yellow
                              {000,010}, //Green
                              {000,110}, //Cyan
                              {000,100}, //Blue
                              {000,101}, //Magenta
                              {111,001}, //White & Red
                              {111,011}, //White & Yellow
                              {111,010}, //White & Green
                              {111,110}, //White & Cyan
                              {111,100}, //White & Blue
                              {111,101}, //White & Magenta
};



void ManageThyme() {
    if (qSec >= /*60*5**/4) {//change minute color!
        minute++; qSec=0;
        //P2OUT ^= BIT1;
        P2OUT ^= colourPair[2].minuteCount;
        //change minute's life.
//        if (minute >= 60) {
        if (minute >=12) {//change hour pattern!
            hour++; minute=0;
            //change hour's life.
            if (hour>=24) {
                hour=0;
            }
        }
    }
}
void IncrTime(void) {
    ManageThyme();
}

void TimerA0Interrupt(void) {
    unsigned short intv = TA0IV;
    qSec++;
    IncrTime();
//    if (intv == 0x0E) {
//        P2OUT ^= BIT1;
//        IncrTime();
//    }
}

void ConfigureTimer (void) {
    TA0CTL = 0x0100;
    TA0CCTL0 = 0x2000;
//    TA0CCR0 = 0x3e80;
    TA0CCR0 = 0x1f40;
    TA0CTL = 0x0116;
}
void PortOneInterrupt (void) {

}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer
    NVIC_EnableIRQ(TA0_N_IRQn);
    InitLED();
    ConfigureTimer();
    for(;;);
}
