#include "init.h"
#include "adc.h"
#include"math.h"
#include <xc.h>

void timer2Init(){
    // Configuration of timer 2 for PWM use
    T2CONbits.TCKPS = 0b01; // Prescaler 8:1
    PR2 = 49999; // 49999 sets value T=10ms only possible with prescaler
    TMR2 = 0;
    T2CONbits.TON = 1;
}


void set_OCxRS_right(float dc){
    // Convertion du duty cycle (dc) désiré en commande OCxRS qui fixe le rapport 
    // cyclique du Output Compare --> qui actionne les moteurs
    
    if( dc <= -1 ){ OC2RS = 5000; }
    else if(dc >= 1) { OC2RS = 10000; } // on s'assure que dc ne devienne pas nég
    else { OC2RS = (dc*2500) + 7500; }
}

void set_OCxRS_left(float dc){
    // Convertion du duty cycle (dc) désiré en commande OCxRS qui fixe le rapport 
    // cyclique du Output Compare --> qui actionne les moteurs
    
    if( dc <= -1 ){ OC1RS = 10000; }
    else if(dc >= 1) { OC1RS = 5000; } // on s'assure que dc ne devienne pas nég
    else { OC1RS = (-(dc)*2500) + 7500; }
}

void motorsInit(){
    RPOR6bits.RP13R = 0b10010; // link left motor to OC1
    RPOR9bits.RP18R = 0b10011; // link right motor to OC2
    
    OC1CONbits.OCTSEL = 0; //select Timer2 for the PWM1
    OC2CONbits.OCTSEL = 0; //idem for PWM2
    
    // Set motors duty cycle; dc = (OC1RS)/(PR2+1)
    // Voltage = -7.2 + 0.00288*(OCxRS-5000)
    OC1RS = 7500; // left
    OC2RS = 7500; // right
    OC1R = 7500;
    OC2R = 7500;
    
    OC1CONbits.OCM = 0b110; // OC enabled in PWM mode withOUT fault protection
    OC2CONbits.OCM = 0b110; 
}

void encodersInit(){
    RPINR14bits.QEA1R = 19;//left encoder
    RPINR14bits.QEB1R = 20;
    
    RPINR16bits.QEA2R = 24;//right encoder
    RPINR16bits.QEB2R = 25;
    
    // Set QEI modes
    QEI1CONbits.QEIM = 0b111;
    QEI2CONbits.QEIM = 0b111;// x4 mode
    
    QEI1CONbits.SWPAB = 1; 
}

float l_ref(float t, float l_target){
    float l_ref;
    float a = 0.00025 ; // 0.5 m/(s^2) --> 0.0005 mm/(ms^2) 
    float t1, l1, v_ref;
    v_ref = 0.4; // mm/ms, v_ref est choisie dans la zone "linéaire" de la courbe v(dc) du robot
    t1= v_ref/a;
    l1 = (a*t1*t1)/2;
    
    if (l_target == 0) {l_ref = 0;}
    
    else if (l_target > l1){ // PROFILE TRAPEZOIDAL
        float l2 = l_target - l1;
        float t2 = ((l2 - l1)/v_ref) + t1 ;
        float t3 = t2 + t1; // si acceleration et deceleration ont la meme valeur, temps sont égaux
        
        if      (t <= t1){              l_ref = a*(t*t)/2 ;}
        else if (t >= t1 && t <= t2){   l_ref = l1 + ( v_ref*(t-t1) ) ;}
        else if (t >= t2 && t <= t3){   l_ref = l2 + ( v_ref*(t-t2) ) - (a*(t-t2)*(t-t2)/2) ;}
        else                        {   l_ref = l_target ;}
    }
    
    else if (l_target < l1){ // PROFILE TRIANGULAIRE
        t1 = sqrt(l_target/a);
        l1 = l_target/2;
        v_ref = a*t1;
        if      (t < t1)              {  l_ref = (a*(t*t)/2) ;}
        else if (t >= t1 && t < 2*t1) {  l_ref = (l_target/2) + v_ref*(t-t1) - a*(t-t1)*(t-t1)/2;}
        else if (t >= 2*t1)           {  l_ref = l_target;}
    }
        
    return l_ref; 
}

float l_mes(int pos_left, int pos_right){
    // Distance parcourue mesurée
    float Rsur2 = 50.8/2; // rayon roue divisé par 2 (mm)
    return ( (pos_right + pos_left)*(3.1416/180) )*Rsur2;
}

float ang_mes(int pos_left, int pos_right){
    // Angle effectué mesuré
    // ATTENTION POS1CNT= right et POS2CNT = left
    float RsurE = 50.8/215; // rayon roues divisé par empattement (E = 215mm)
    float ang = (pos_right - pos_left)* RsurE*(3.1416/180);
    
    return ang;
}

float ang_ref(float t, float ang_target){
    float ang_ref;
    float ang_speed = 1.57/1000; //rad/ms vitesse de reference
    //float ang_a = 0.00000157; //rad
    if( ang_target < 0 ){ang_speed = -ang_speed;}
    float ti = ang_target/ang_speed;
    
    //float t1 = ang_speed/ang_a;
    //float ang1 = ang_a*t1*t1/2;
    
    //float ang2 = ang_target - t1;
    //float t2 = ((ang2 - ang1)/ang_speed) + t1;
    //float t3 = t1 + t2;
    
    if (ang_target == 0){ang_ref = 0;}
    else if( t <= ti){ang_ref = ang_speed*t;}
    else if(t > ti){ang_ref = ang_target;}
    
    //else if(t <= t1){           ang_ref = ang_a*t*t/2 ;}
    //else if(t > t1 && t <= t2){ ang_ref = ang1 + ( ang_speed*(t-t1) ) ;}
    //else if(t > t2 && t <= t3){ ang_ref = ang2 + ( ang_speed*(t-t2) ) - (ang_a*(t-t2)*(t-t2)/2);}
    //else if(t > t3 ){           ang_ref = ang_target ;}
    
    return ang_ref;
}

int main(void)
{
	
   oscillatorInit();
   OC1CONbits.OCM = 0b000; // OC disabled (plus propre de faire comme ca...)
   OC2CONbits.OCM = 0b000; // OC disabled (plus propre de faire comme ca...)
  
   timer2Init();
   motorsInit();
   encodersInit(); 
   
   //----------UART--------------
   
   
    U1BRG = 40000000/(16*115200) - 1;
    U1MODEbits.BRGH = 0;                    //High Baud Rate Enable bit (Standard mode)
    U1MODEbits.UARTEN = 1;                  //all UARTx pins are controlled by UARTx
    U1MODEbits.PDSEL = 0b00;                //8-bit data, no parity
    U1MODEbits.STSEL = 0;                   //Stop Bit Selection bit, one stop bit
    
    U1STAbits.UTXEN = 1;                    // Enable UART TX
    RPINR18bits.U1RXR = 0b00110;            // Assign UART1 input to RP6
    RPOR3bits.RP7R = 0b00011;               // Assign UART1 output to RP7
   
   
   
   //----------------------------
    //Variables
    
    //int dist1 = POS1CNT;
    //int dist2 = POS2CNT;
    float t = 0; //ms
    //float kp_dist = 0.00849; // 8.49 /m --> 0.00849 /mm;
    //float kp_ang = 0.075;
    float kp_fois_e_dist=0, kp_fois_e_ang=0;
    int pos_right, pos_left;
    
    //set_OCxRS_right(0);
    //set_OCxRS_left(0);
    
    int POS0 = 32768;
    POS1CNT = POS0;
    POS2CNT = POS0;
    
    while(1) {	
        
        /*
         * We check if there is any data in the receive buffer. 
         * The message will contain the command the motor will execute.
         */
        
        if(U1STA.URXDA){                   //Receive Buffer Data Available bit 
        
            if(U1RXREG == 1){
            }
        
        }
        
        
        if (IFS0bits.T2IF) {
            IFS0bits.T2IF = 0;
            t += 10;
        
            pos_right = POS1CNT - POS0;
            pos_left = POS2CNT - POS0;
        
            if( ang_mes(pos_left, pos_right) > -3.1416*2){
                kp_fois_e_dist = ( l_ref(t, 0) - l_mes(pos_left, pos_right) )*0.00846;
                kp_fois_e_ang = ( ang_ref(t,-3.1416*2) - ang_mes(pos_left, pos_right))*0.5;
        
                set_OCxRS_right(kp_fois_e_dist + kp_fois_e_ang);
                set_OCxRS_left(kp_fois_e_dist - kp_fois_e_ang);
            }
            
            else {
                set_OCxRS_right(0);
                set_OCxRS_left(0);
            }
        }
        
    }                                                                                                                  
}