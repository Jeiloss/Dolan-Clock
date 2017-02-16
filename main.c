#include "msp.h"

void TimerA0Interrupt(void) {

}

void ConfigureTimer (void) {
    TA0CTL = 0x0100;
    TA0CCTL0 = 0x2000;
    TA0CCR0 = 0xFA00;
    TA0CTL = 0x0136;
}
void PortOneInterrupt (void) {

}

void main(void) {
    WDTCTL = WDTPW | WDTHOLD;           // Stop watchdog timer

    for(;;);
}
