#include "msp.h"
#include "portfunc.h"
#include "clock.h"

unsigned int qSec=0;
unsigned int minute=0;//PROPOSAL: change to track 5 minutes.
unsigned int hour=0;

void InitLED (void){
    P2DIR|=(BIT0|BIT1|BIT2);
    SelectPortFunction(2,0,0,0);
    SelectPortFunction(2,1,0,0);//need more than 1?
    SelectPortFunction(2,2,0,0);//Shouldn't just 1 timer be fine?
    P2OUT &=~(BIT0|BIT1|BIT2);//turns light off.
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

/**
 * Note: qSec is being incremented before ManageThyme is called,
 *       so don't worry that it is not being implemented here.
 *
 * Idea: Change blinking speed to 1-sec. ~Kyle
 */
void ManageThyme(){
    if (minute>=5){
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
    if (qSec>=4){
        minute++; qSec=0;
    }
    if (qSec>=/*60*5**/4){//change minute color!
        minute++; qSec=0;
//        if (minute >= 60) {
        if (minute>=6) {//change hour pattern!
            hour++; minute=0;
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
}

void ConfigureTimer (void) {
    TA0CTL = 0x0100;
    TA0CCTL0 = 0x2000;
    TA0CCR0 = 0x3e80;//half-sec
//    TA0CCR0 = 0x1f40;/quar-sec
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
