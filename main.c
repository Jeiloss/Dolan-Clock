#include "msp.h"

unsigned int;

void TimerA0Interrupt(void) {

}

void ConfigureTimer (void) {
    TA0CTL = 0x0100;
    TA0CCTL0 = 0x2000;
    TA0CCR0 = 0x3e80;
    TA0CTL = 0x0116;
}
void PortOneInterrupt (void) {

}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    for(;;);
}
