#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define M_PI 3.14159265358979323846
#define n 1024
#define nsquare 1048576
#define gain 8

typedef struct secondValues {
    long interval;
    long newval;
}secondValues;

secondValues sos(int coef[6], long bridge[2], long input) {
    secondValues this;
    this.interval = input*n - (coef[4]*bridge[1]) - (coef[5]*bridge[0]);
    this.newval = (coef[0]*this.interval)/n + (coef[1]*bridge[1]) + (coef[2]*bridge[0]);
    return this;
}

int filtre900(int input)
{
    /*Coefficients des filtres du second ordre pour 900Hz*/
    int coef9001[6] = {1024, 2048, 1024, 1024, -1907, 1012};
    int coef9002[6] = {1024, 2048, 1024, 1024, -1911, 1012};
    int coef9003[6] = {1024, -2048, 1024, 1024, -1911, 1019};
    int coef9004[6] =  {1024, -2048, 1024, 1024, -1920, 1019};
    static long bridge1[2] = {0,0};
    static long bridge2[2] = {0,0};
    static long bridge3[2] = {0,0};
    static long bridge4[2] = {0,0};
    long output1 = 0;
    long output2= 0;
    long output3 = 0;
    long output4 = 0;
    int exit = 0;

    secondValues secondValues1 = sos(coef9001, bridge1, input);
    bridge1[0] = bridge1[1];
    bridge1[1] = secondValues1.interval/n;
    output1 = (secondValues1.newval*gain)/nsquare;
    secondValues secondValues2 = sos(coef9002, bridge2, output1);
    bridge2[0] = bridge2[1];
    bridge2[1] = secondValues2.interval/n;
    output2 = (secondValues2.newval*gain)/nsquare;
    secondValues secondValues3 = sos(coef9003, bridge3, output2);
    bridge3[0] = bridge3[1];
    bridge3[1] = secondValues3.interval/n;
    output3 = (secondValues3.newval*gain)/nsquare;
    secondValues secondValues4 = sos(coef9004, bridge4, output3);
    bridge4[0] = bridge4[1];
    bridge4[1] = secondValues4.interval/n;
    output4 = (secondValues4.newval*gain)/nsquare;
    return exit = 512 + output4;
}

int filtre1100(int input)
{
    /*Coefficients des filtres du second ordre pour 1100Hz*/
    int coef11001[6] = {1024, 2048, 1024, 1024, -1843, 1009};
    int coef11002[6] = {1024, 2048, 1024, 1024, -1848, 1009};
    int coef11003[6] = {1024, -2048, 1024, 1024, -1847, 1018};
    int coef11004[6] = {1024, -2048, 1024, 1024, -1859, 1018};
    static long bridge5[2] = {0,0};
    static long bridge6[2] = {0,0};
    static long bridge7[2] = {0,0};
    static long bridge8[2] = {0,0};
    long output1 = 0;
    long output2= 0;
    long output3 = 0;
    long output4 = 0;
    long exit = 0;

    secondValues secondValues1 = sos(coef11001, bridge5, input);
    bridge5[0] = bridge5[1];
    bridge5[1] = secondValues1.interval/n;
    output1 = (secondValues1.newval*gain)/nsquare;
    secondValues secondValues2 = sos(coef11002, bridge6, output1);
    bridge6[0] = bridge6[1];
    bridge6[1] = secondValues2.interval/n;
    output2 = (secondValues2.newval*gain)/nsquare;
    secondValues secondValues3 = sos(coef11003, bridge7, output2);
    bridge7[0] = bridge7[1];
    bridge7[1] = secondValues3.interval/n;
    output3 = (secondValues3.newval*gain)/nsquare;
    secondValues secondValues4 = sos(coef11004, bridge8, output3);
    bridge8[0] = bridge8[1];
    bridge8[1] = secondValues4.interval/n;
    output4 = (secondValues4.newval*gain)/nsquare;
    return exit = 512 + output4;
}
