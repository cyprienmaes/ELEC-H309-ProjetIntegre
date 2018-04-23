#include "init.h"
#include "adc.h"
#include <xc.h>


void _ISR _ADC1Interrupt(void) {
    
    IFS0bits.AD1IF = 0;
    
    if (U1STAbits.UTXBF != 1){          //Transmit buffer is not full, at least one more character can be written
        IEC0bits.U1TXIE = 1;
        unsigned int a = adcRead();
        int_to_int(a);
        U1TXREG = adcRead();                         // Put the data in the transmit buffer
    }
    
}

unsigned int int_to_int(unsigned int k) {
    return (k == 0 || k == 1 ? k : ((k % 2) + 10 * int_to_int(k / 2)));
}


int main(void)
{
	oscillatorInit();
    adcTimerInit();
    PR3 = 2665;    // T3=1/15ms=(PR3+1)*25ns => PR3=2665
    T3CONbits.TON = 1;
    
    //TRISPbits.TRISP6 = 1; //entree
    //TRISPbits.TRISP7 = 0; // sortie
    //uart
    //assign pin 14 to the UART1 RX input register
    //RX RP14 (input)
    //assign UART1 TX function to the pin 15 output register
    //TX RP15 (output)

    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB6 = 0;
    
                                            // Fcy = 40 MHz
                                            //OperatingFc = OscillatorFc/2
    
	U1BRG = 40000000/(16*115200) - 1;
    U1MODEbits.BRGH = 0;                    //High Baud Rate Enable bit (Standard mode)
    U1MODEbits.UARTEN = 1;                  //all UARTx pins are controlled by UARTx
    U1MODEbits.PDSEL = 0b00;                //8-bit data, no parity
    U1MODEbits.STSEL = 0;                   //Stop Bit Selection bit, one stop bit
    
    //U1STAbits.UTXISEL1 = 00;                //Transmission Interrupt Mode Selection bits, Interrupt when a character is transferred to the Transmit Shift Register
    //U1STAbits.UTXEN = 1;                    //Transmit Enable bit, Transmit enabled, UxTX pin controlled by UARTx
    //U1STAbits.URXISEL1 = 11;                // Receive Interrupt Mode Selection bits, Interrupt is set on UxRSR transfer making the receive buffer full
    //U1STAbits.PERR = 1;
    //U1STAbits.FERR = 1;
    //U1STAbits.OERR = 1;
    
    //U1STAbits.UTXISEL0 = 0;                  // Interrupt after one TX character is transmitted
    //IEC0bits.U1TXIE = 1;                     // Enable UART TX interrupt
                   
    U1STAbits.UTXEN = 1;                    // Enable UART TX
    RPINR18bits.U1RXR = 0b00110;            // Assign UART1 input to RP6
    RPOR3bits.RP7R = 0b00011;               // Assign UART1 output to RP7
    
    //------------ADC------------------
    /*
    AD1CON1bits.ADON = 1;                   //ADC Operating Mode bit
    AD1CON1bits.AD12B = 0;                  //10-Bit Operation Mode bit
    AD1CON1bits.FORM = 0b00;                //Integer (D OUT = 0000 00dd dddd dddd)
    AD1CON1bits.SSRC = 0b111;               //Internal counter ends sampling and starts conversion (auto-convert)
    AD1CON1bits.ASAM = 0;                   //Sampling begins when SAMP bit is set
    AD1CON1bits.SAMP = 1;                   //ADC sample/hold amplifiers are sampling
    AD1CON1bits.DONE;                       //ADC Conversion Status bit
    
     */ 
    //--------INTERUPTION--------------
    
    INTCON1bits.NSTDIS = 1;                 //Interrupt Nesting Disable bit
    IFS0bits.AD1IF = 0;                     //ADC1 Conversion Complete Interrupt Flag Status bit
    IEC0bits.AD1IE = 1;                    //ADC1 Transmitter Interrupt Enable bit
      
    
	while(1) {	
  
        
    }    
}
