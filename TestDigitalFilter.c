#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>

#define M_PI 3.14159265358979323846

int32_t n = 512;
int32_t nsquare =  262144;

typedef struct secondValues {
    int32_t interval;
    int32_t newval;
}secondValues;

secondValues sos(int32_t coef[6], int32_t bridge[2], int32_t input) {
    secondValues this;
    this.interval = input*n - (coef[4]*bridge[1]) - (coef[5]*bridge[0]);
    this.interval = this.interval/n;
    this.newval = coef[0]*this.interval + (coef[1]*bridge[1]) + (coef[2]*bridge[0]);
    this.newval = this.newval/n;
    return this;
}

int main(void)
{
    /*
    int32_t coef1 [6] = {512, 1024, 512, 512, -922, 505};
    int32_t coef2 [6] = {512, 1024, 512, 512, -925, 505};
    int32_t coef3 [6] = {512, -1024, 512, 512, -924, 509};
    int32_t coef4 [6] = {512, -1024, 512, 512, -930, 509};
    int32_t gain1100 = 4;
    int32_t coef1 [6] = {512, 1024, 512, 512, -954, 506};
    int32_t coef2 [6] = {512, 1024, 512, 512, -956, 506};
    int32_t coef3 [6] = {512, -1024, 512, 512, -956, 509};
    int32_t coef4 [6] = {512, -1024, 512, 512, -960, 509};
    int gain900 = 3;*/ // Filtre pour 16000Hz
    /*
    int32_t coef1 [6] = {512, 1024, 512, 512, -916, 504};
    int32_t coef2 [6] = {512, 1024, 512, 512, -918, 504};
    int32_t coef3 [6] = {512, -1024, 512, 512, -918, 509};
    int32_t coef4 [6] = {512, -1024, 512, 512, -924, 509};
    int32_t gain1100 = 4;
    int32_t coef1 [6] = {512, 1024, 512, 512, -950, 506};
    int32_t coef2 [6] = {512, 1024, 512, 512, -952, 506};
    int32_t coef3 [6] = {512, -1024, 512, 512, -952, 509};
    int32_t coef4 [6] = {512, -1024, 512, 512, -956, 509};
    int gain900 = 3; */ // Filtre pour 15500Hz
    /*
    int32_t coef1 [6] = {512, 1024, 512, 512, -901, 504};
    int32_t coef2 [6] = {512, 1024, 512, 512, -904, 504};
    int32_t coef3 [6] = {512, -1024, 512, 512, -903, 509};
    int32_t coef4 [6] = {512, -1024, 512, 512, -911, 509};
    int32_t gain1100 = 4;
    int32_t coef1 [6] = {512, 1024, 512, 512, -940, 505};
    int32_t coef2 [6] = {512, 1024, 512, 512, -942, 505};
    int32_t coef3 [6] = {512, -1024, 512, 512, -942, 509};
    int32_t coef4 [6] = {512, -1024, 512, 512, -947, 509};
    int gain900 = 4;*/ // Filtre pour 14200Hz
    int32_t coef1 [6] = {512, 1024, 512, 512, -896, 504};
    int32_t coef2 [6] = {512, 1024, 512, 512, -899, 504};
    int32_t coef3 [6] = {512, -1024, 512, 512, -898, 508};
    int32_t coef4 [6] = {512, -1024, 512, 512, -906, 509};
    int32_t gain1100 = 5;
    /*
    int32_t coef1 [6] = {512, 1024, 512, 512, -936, 505};
    int32_t coef2 [6] = {512, 1024, 512, 512, -939, 505};
    int32_t coef3 [6] = {512, -1024, 512, 512, -939, 509};
    int32_t coef4 [6] = {512, -1024, 512, 512, -944, 509};
    int gain900 = 4;*/
    /*Implementation des vecteurs necessaires du filtre (voltage => mots binaires en tension,
                                                         bridge => mots binaires des transitions entre les sos,
                                                         sortie => mots binaires des sorties et de la recursivite)*/
    int32_t voltage = 0;
    int32_t bridge1[2] = {0, 0};
    int32_t output1 = 0;
    int32_t bridge2[2] = {0, 0};
    int32_t output2= 0;
    int32_t bridge3[2] = {0, 0};
    int32_t output3 = 0;
    int32_t bridge4[2] = {0, 0};
    int32_t output4 = 0;
    int32_t exit = 0;
    int j = 0;
    int i = 0;
    int nbEchant = 20;
    int seuil1100max = 860;
    int seuil1100min = 170;
    int seuil900max = 717;
    int seuil900min = 307;
    int flagSeuil900 = 0;
    int flagSeuil1100 = 0;
    char bit1100 = 0;
    char bit900 = 0;
    int f = 1100;
    int fe = 14200;

    FILE* fichier = NULL;
    fichier = fopen("nombres.txt", "w");

    if (fichier != NULL)
        {
        for(j=1;j<2001;j=j+1){
            voltage = round(512 + 512*sin(f*2*M_PI*j/fe));
            secondValues secondValues1 = sos(coef1, bridge1, voltage);
            bridge1[0] = bridge1[1];
            bridge1[1] = secondValues1.interval;
            output1 = secondValues1.newval;
            output1 = (output1*gain1100)/n;
            secondValues secondValues2 = sos(coef2, bridge2, output1);
            bridge2[0] = bridge2[1];
            bridge2[1] = secondValues2.interval;
            output2 = secondValues2.newval;
            output2 = (output2*gain1100)/n;
            secondValues secondValues3 = sos(coef3, bridge3, output2);
            bridge3[0] = bridge3[1];
            bridge3[1] = secondValues3.interval;
            output3 = secondValues3.newval;
            output3 = (output3*gain1100)/n;
            secondValues secondValues4 = sos(coef4, bridge4, output3);
            bridge4[0] = bridge4[1];
            bridge4[1] = secondValues4.interval;
            output4 = secondValues4.newval;
            output4 = (output4*gain1100)/n;
            exit = output4+512;
            fprintf(fichier,  "%" PRIi32 "\n", exit);
            /*
            if (exit <= seuil1100min) flagSeuil1100 +=1;
            else if (exit >= seuil1100max) flagSeuil1100 +=1;
            else flagSeuil1100 += 0;
            i += 1;
            if (i==nbEchant) {
                if (flagSeuil1100 >= 2) bit1100 = 1;
                else bit1100 = 0;
                fprintf(fichier,  "%" PRIi32 "\n", bit1100);
                i = 0;
                flagSeuil1100 = 0;
            }*//*
            if (exit <= seuil900min) flagSeuil900 +=1;
            else if (exit >= seuil900max) flagSeuil900 +=1;
            else flagSeuil900 += 0;
            i += 1;
            if (i==nbEchant) {
                if (flagSeuil900 >= 2) bit900 = 1;
                else bit900 = 0;
                fprintf(fichier,  "%" PRIi32 "\n", bit900);
                i = 0;
                flagSeuil900 = 0;
            }*/
        }
        fclose(fichier);
    }
    else
    {
        printf("Impossible d'ouvrir le fichier test.txt");
    }
}
