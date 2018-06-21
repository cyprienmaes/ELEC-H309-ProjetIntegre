#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "init.h"
#include "adc.h"
#include "FskDetector.h"
#include <xc.h>
#include "filtreNum.h"
char bit900 = '0';
char bit1100 = '0';
long output900 = 0;
long output1100 = 0;
long voltage = 0;
int message = 0;
int timerCount = 0;
char variable = 0;

typedef struct messageSplit {
    char message1;
    char message2;
}messageSplit;

messageSplit newMessage;

messageSplit separate_message(int m){
    messageSplit sendMessage;
    int bitsmessage1  = m & 0b0000001100000000;
    bitsmessage1 = bitsmessage1 >> 8;
    int bitsmessage2 = m & 0b0000000011111111;
    sendMessage.message1 = (char) bitsmessage1;
    sendMessage.message2 = (char) bitsmessage2; 
    return sendMessage;
}   

void _ISR _T2Interrupt(void) {
    timerCount = 1 + timerCount;
    IFS0bits.T2IF = 0;
    message = 0;
    if(timerCount == 10 || timerCount == 30){
        
        while(U1STAbits.UTXBF){}          //Transmit buffer is not full, at least one more character can be written
            message = 0b0000000000110010;
            //message = 50;
            newMessage = separate_message(message);
            variable = newMessage.message1;
            U1TXREG = newMessage.message1;
            //U1TXREG = separate_message(message).message1;   // Put the data in the transmit buffer
            //U1TXREG = bit900;
            //U1TXREG = 0b00;
            //U1TXREG = '7';
            //if (U1STAbits.UTXBF != 1){          //Transmit buffer is not full, at least one more character can be written
            
            while(U1STAbits.UTXBF){}
            U1TXREG = newMessage.message2;
            //U1TXREG = separate_message(message).message2;   // Put the data in the transmit buffer
            //U1TXREG = 50;
            //}
            // U1TXREG = bit1100;

        
    }
    
    if(timerCount == 20){
        message = 0b0000000110110100;
        newMessage = separate_message(message);
        while (U1STAbits.UTXBF ){}          //Transmit buffer is not full, at least one more character can be written
            U1TXREG = newMessage.message1;
            //U1TXREG = 0b01;
            //U1TXREG = separate_message(message).message1;   // Put the data in the transmit buffer

            while(U1STAbits.UTXBF){}
            U1TXREG = newMessage.message2;
            //U1TXREG = '90';    
            //U1TXREG = separate_message(message).message2;   // Put the data in the transmit buffer
                
    }
    
    if(timerCount == 40){
        timerCount = 0;
        message = 0b0000001010110100;
         newMessage = separate_message(message);
        while (U1STAbits.UTXBF ){}          //Transmit buffer is not full, at least one more character can be written
            U1TXREG = newMessage.message1;
            //U1TXREG = 0b10;
            //U1TXREG = separate_message(message).message1;   // Put the data in the transmit buffer

            while(U1STAbits.UTXBF){}
            U1TXREG = newMessage.message2;
            //U1TXREG = '90';        
            //U1TXREG = separate_message(message).message2;   // Put the data in the transmit buffer
               
    }
    
}


int main(void)
{   
    // Variable pour l'ADC.
    // echantillon sur une periode a 1100Hz = 14.5 et echantillon sur une periode
    // a 900Hz = 17.8. On prend donc un nombre d'echantillon egal a 20 pour avoir
    // une marge de securite et aussi une frequence entiere d'envoi a l'UART.
    char nbEchant = 20;
    // Definition Des seuils pour 1100Hz et 900Hz.
    long seuilMax1100 = 1200;
    long seuilMin1100 = -200;
    long seuilMax900 = 600;
    long seuilMin900 = 400;
    
    // flag pour le dépassement des seuils
    char flagSeuil1100 = 0;
    char flagSeuil900 = 0;
    
    // flag pour l'incrementation d'un bit
    char flagBit = 0;

    
    oscillatorInit();
    adcTimerInit();
    // Configuration du Timer2 pour pour la frequence d'envoie à l'UART égale a 800Hz.
    
    PR2 = 65535; // => PR2 = 1/800 * 4 * 10^6
    
    //PR2 = 5000; // => PR2 = 1/800 * 4 * 10^6
    T2CONbits.TCKPS = 0b11;  //Timer1 Input Clock Prescale Select bits 1:256
    T2CONbits.TON = 1;
    
    // Configuration du Timer3 a 16 000 Hz pour echantillonage.
    PR3 = 250; // => PR3 = 1/16000 * 4 * 10^6
    T3CONbits.TON = 1;
    
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB6 = 0;
    
    U1BRG = 40000000.0/(16*115200) - 1;
    U1MODEbits.BRGH = 0;                    //High Baud Rate Enable bit (Standard mode)
    U1MODEbits.UARTEN = 1;                  //all UARTx pins are controlled by UARTx
    U1MODEbits.PDSEL = 0b00;                //8-bit data, no parity
    U1MODEbits.STSEL = 0;                   //Stop Bit Selection bit, one stop bit
    
    U1STAbits.UTXEN = 1;                    // Enable UART TX
    RPINR18bits.U1RXR = 0b00110;            // Assign UART1 input to RP6
    RPOR3bits.RP7R = 0b00011;               // Assign UART1 output to RP7
    
    INTCON1bits.NSTDIS = 1;                 //Interrupt Nesting Disable bit
    IFS0bits.T3IF = 0;
    IFS0bits.T2IF = 0;                     //ADC1 Conversion Complete Interrupt Flag Status bit
    IEC0bits.T2IE = 1;                    //ADC1 Transmitter Interrupt Enable bit
    while(1) {
        
        /*
        if (IFS0bits.T3IF) {
            IFS0bits.T3IF = 0;
            adcPollingStart();
            while(!adcConversionFinished());
            voltage = adcRead();
            output900 = filtre900(voltage);
            output1100 = filtre1100(voltage);
            if (output900 <= seuilMin900) flagSeuil900 +=1;
            else if (output900 >= seuilMax900) flagSeuil900 +=1;
            else if (output1100 <= seuilMin1100) flagSeuil1100 += 1;
            if (output1100 >= seuilMax1100)flagSeuil1100 +=1;
            flagBit +=1;
        }
        if (flagBit == nbEchant) {
            flagBit = 0;
            if (flagSeuil900 >= 2) {
                bit900 = '1';
                message = fskDetector(1,0);
            }
            else bit900 = '0';
            if (flagSeuil1100 >= 2) {
                bit1100 = '1';
                message = fskDetector(0,1);
            }
            else {
                bit1100 = '0';
                message = fskDetector(0,0);
            }
            flagSeuil900 = 0;
            flagSeuil1100 = 0;
            if(message != 0){
                
            }

        }*/
    }                                                                                                            
}
