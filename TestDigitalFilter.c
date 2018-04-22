#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>

#define M_PI 3.14159265358979323846
#define n 1024
#define nsquare 1048576
#define gain 8

typedef struct secondValues {
    long interval;
    long newval;
}secondValues;

secondValues sos(long coef[6], long bridge[2], long input) {
    secondValues this;
    this.interval = input*n - (coef[4]*bridge[1]) - (coef[5]*bridge[0]);
    this.newval = (coef[0]*this.interval)/n + (coef[1]*bridge[1]) + (coef[2]*bridge[0]);
    return this;
}

int main(void)
{

    /*Coefficients des filtres du second ordre pour 1100Hz*/
    /*long coef1 [6] = {1024, 2048, 1024, 1024, -1843, 1009};
    long coef2 [6] = {1024, 2048, 1024, 1024, -1848, 1009};
    long coef3 [6] = {1024, -2048, 1024, 1024, -1847, 1018};
    long coef4 [6] = {1024, -2048, 1024, 1024, -1859, 1018};*/

    /*Coefficients des filtres du second ordre pour 900Hz*/
    long coef1 [6] = {1024, 2048, 1024, 1024, -1907, 1012};
    long coef2 [6] = {1024, 2048, 1024, 1024, -1911, 1012};
    long coef3 [6] = {1024, -2048, 1024, 1024, -1911, 1019};
    long coef4 [6] = {1024, -2048, 1024, 1024, -1920, 1019};

    /*Implementation des vecteurs necessaires du filtre (voltage => mots binaires en tension,
                                                         bridge => mots binaires des transitions entre les sos,
                                                         sortie => mots binaires des sorties et de la recursivite)*/
    long voltage = 0;
    long bridge1[2] = {0, 0};
    long output1 = 0;
    long bridge2[2] = {0, 0};
    long output2= 0;
    long bridge3[2] = {0, 0};
    long output3 = 0;
    long bridge4[2] = {0, 0};
    long output4 = 0;
    long exit = 0;
    int j = 0;
    int f = 930;
    int fe = 16000;

    FILE* fichier = NULL;
    fichier = fopen("nombres.txt", "w");

    if (fichier != NULL)
        {
        for(j=1;j<2001;j=j+1){
            voltage = round(512 + 512*sin(f*2*M_PI*j/fe));
            secondValues secondValues1 = sos(coef1, bridge1, voltage);
            bridge1[0] = bridge1[1];
            bridge1[1] = secondValues1.interval/n;
            output1 = (secondValues1.newval*gain)/nsquare;
            secondValues secondValues2 = sos(coef2, bridge2, output1);
            bridge2[0] = bridge2[1];
            bridge2[1] = secondValues2.interval/n;
            output2 = (secondValues2.newval*gain)/nsquare;
            secondValues secondValues3 = sos(coef3, bridge3, output2);
            bridge3[0] = bridge3[1];
            bridge3[1] = secondValues3.interval/n;
            output3 = (secondValues3.newval*gain)/nsquare;
            secondValues secondValues4 = sos(coef4, bridge4, output3);
            bridge4[0] = bridge4[1];
            bridge4[1] = secondValues4.interval/n;
            output4 = (secondValues4.newval*gain)/nsquare;
            exit = 512 + output4;
            fprintf(fichier,  "%ld\n", exit);
        }
        fclose(fichier);
    }
    else
    {
        printf("Impossible d'ouvrir le fichier test.txt");
    }
}
