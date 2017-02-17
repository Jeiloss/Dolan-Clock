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
struct color {
    unsigned short red;
    unsigned short green;
    unsigned short blue;
};
struct color colorarray[12]={
                              {255,0,0}, //Red
                              {255,255,0}, //Yellow
                              {0,255,0}, //Green
                              {0,255,255}, //Cyan
                              {0,0,255}, //Blue
                              {255,0,255}, //Magenta
                              {0,0,0}, //No color / Off / Black
                              {255,255,255} //White / ALL THE COLORS!!!
};

void ManageThyme() {
    if (qSec >= /*60*5**/4) {//change minute color!
        minute++; qSec=0;
        P2OUT ^= BIT1;
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
