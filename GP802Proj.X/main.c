#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "init.h"
#include "adc.h"
#include <xc.h>
#include "filtreNum.h"
char bit900 = 0;
char bit1100 = 0;

void _ISR_T2Interrupt(void) {
    IFS0bits.AD1IF = 0;
    if (U1STAbits.UTXBF != 1){          //Transmit buffer is not full, at least one more character can be written
        IEC0bits.U1TXIE = 1;
        U1TXREG = bit900;   // Put the data in the transmit buffer
        //U1TXREG = bit1100;
    }
}
    
int main(void)
{   
    // Variable pour l'ADC.
    int voltage; 
    // Sortie des deux filtres
    int output900;
    int output1100;
    // echantillon sur une periode a 1100Hz = 14.5 et echantillon sur une periode
    // a 900Hz = 17.8. On prend donc un nombre d'echantillon egal a 20 pour avoir
    // une marge de securite et aussi une frequence entiere d'envoi a l'UART.
    char nbEchant = 20;
    
    // Definition Des seuils pour 1100Hz et 900Hz.
    int seuilMax1100 = 1010;
    int seuilMin1100 = 10;
    int seuilMax900 = 1024;
    int seuilMin900 = 0;
    
    // flag pour le dépassement des seuils
    char flagSeuil1100 = 0;
    char flagSeuil900 = 0;
    
    // flag pour l'incrementation d'un bit
    char flagBit = 0;

    
    oscillatorInit();
    adcTimerInit();
    // Configuration du Timer2 pour pour la frequence d'envoie à l'UART égale a 800Hz.
    PR2 = 5000; // => PR2 = 1/800 * 4 * 10^6
    T2CONbits.TON = 1;     
    // Configuration du Timer3 a 16 000 Hz pour echantillonage.
    PR3 = 250; // => PR3 = 1/16000 * 4 * 10^6
    T3CONbits.TON = 1;
    
    
    
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB6 = 0;
    
    U1BRG = 40000000/(16*115200) - 1;
    U1MODEbits.BRGH = 0;                    //High Baud Rate Enable bit (Standard mode)
    U1MODEbits.UARTEN = 1;                  //all UARTx pins are controlled by UARTx
    U1MODEbits.PDSEL = 0b00;                //8-bit data, no parity
    U1MODEbits.STSEL = 0;                   //Stop Bit Selection bit, one stop bit
    
    U1STAbits.UTXEN = 1;                    // Enable UART TX
    RPINR18bits.U1RXR = 0b00110;            // Assign UART1 input to RP6
    RPOR3bits.RP7R = 0b00011;               // Assign UART1 output to RP7
    
    INTCON1bits.NSTDIS = 1;                 //Interrupt Nesting Disable bit
    IFS0bits.AD1IF = 0;                     //ADC1 Conversion Complete Interrupt Flag Status bit
    IEC0bits.AD1IE = 1;                    //ADC1 Transmitter Interrupt Enable bit
    
    while(1) {
        
        if (IFS0bits.T2IF) {
            IFS0bits.T2IF = 0;
            adcPollingStart();
            while(!adcConversionFinished());
            voltage = adcRead();
            output900 = filtre900(voltage);
            output1100 = filtre1100(voltage);
            if (output900 <= seuilMin900 || output900 >= seuilMax900) {
                flagSeuil900 +=1;
            }
            else if (output1100 <= seuilMin1100 || output1100 >= seuilMax1100) {
                flagSeuil1100 +=1;
            }
            flagBit +=1;
        }
        if (flagBit == nbEchant - 1) {
            flagBit = 0;
            if (flagSeuil900 >= 2) bit900 = 1;
            else bit900 = 0;
            if (flagSeuil1100 >=2) bit1100 =1;
            else bit1100 = 0;
            flagSeuil900 = 0;
            flagSeuil1100 = 0;
        }
    }                                                                                                            
}
