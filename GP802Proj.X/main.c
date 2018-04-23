#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "init.h"
#include "adc.h"
#include <xc.h>
#include "filtreNum.h"

int main(void)
{   
    int voltage;
    int output900;
    int output1100;
    
    oscillatorInit();
    adcTimerInit();
    // Configuration du Timer2 a 16 000 Hz 
    PR3 = 250; // => PR2 = 1/16000 * 4 * 10^6
    T3CONbits.TON = 1;
    
    while(1) {
        
        if (IFS0bits.T2IF) {
            IFS0bits.T2IF = 0;
            adcPollingStart();
            while(!adcConversionFinished());
            voltage = adcRead();
            output900 = filtre900(voltage);
            output1100 = filtre1100(voltage);
        }
    }                                                                                                            
}
