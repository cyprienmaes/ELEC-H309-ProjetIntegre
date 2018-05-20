#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define n 512
#define gain900 3
#define gain1100 4

long coef9001 [6] = {512, 1024, 512, 512, -954, 506};
long coef9002 [6] = {512, 1024, 512, 512, -956, 506};
long coef9003 [6] = {512, -1024, 512, 512, -956, 509};
long coef9004 [6] = {512, -1024, 512, 512, -960, 509};

long coef11001 [6] = {512, 1024, 512, 512, -922, 505};
long coef11002 [6] = {512, 1024, 512, 512, -925, 505};
long coef11003 [6] = {512, -1024, 512, 512, -924, 509};
long coef11004 [6] = {512, -1024, 512, 512, -930, 509};

typedef struct secondValues {
    long interval;
    long newval;
}secondValues;

secondValues sos(long coef[6], long bridge[2], long input) {
    secondValues this;
    this.interval = input*n - (coef[4]*bridge[1]) - (coef[5]*bridge[0]);
    this.interval = this.interval/n;
    this.newval = coef[0]*this.interval + (coef[1]*bridge[1]) + (coef[2]*bridge[0]);
    this.newval = this.newval/n;
    return this;
}

long filtre900(long input)
{
    /*Coefficients des filtres du second ordre pour 900Hz*/
    static long bridge1[2] = {0,0};
    static long bridge2[2] = {0,0};
    static long bridge3[2] = {0,0};
    static long bridge4[2] = {0,0};
    long output1 = 0;
    long output2= 0;
    long output3 = 0;
    long output4 = 0;
    long exit = 0;

    secondValues secondValues1 = sos(coef9001, bridge1, input);
    bridge1[0] = bridge1[1];
    bridge1[1] = secondValues1.interval;
    output1 = secondValues1.newval;
    output1 = (output1*gain900)/n;
    secondValues secondValues2 = sos(coef9002, bridge2, output1);
    bridge2[0] = bridge2[1];
    bridge2[1] = secondValues2.interval;
    output2 = secondValues2.newval;
    output2 = (output2*gain900)/n;
    secondValues secondValues3 = sos(coef9003, bridge3, output2);
    bridge3[0] = bridge3[1];
    bridge3[1] = secondValues3.interval;
    output3 = secondValues3.newval;
    output3 = (output3*gain900)/n;
    secondValues secondValues4 = sos(coef9004, bridge4, output3);
    bridge4[0] = bridge4[1];
    bridge4[1] = secondValues4.interval;
    output4 = secondValues4.newval;
    output4 = (output4*gain900)/n;
    exit = 512 + output4;
    return exit;
}

long filtre1100(long input)
{
    /*Coefficients des filtres du second ordre pour 1100Hz*/
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
    bridge5[1] = secondValues1.interval;
    output1 = secondValues1.newval;
    output1 = (output1*gain1100)/n;
    secondValues secondValues2 = sos(coef11002, bridge6, output1);
    bridge6[0] = bridge6[1];
    bridge6[1] = secondValues2.interval;
    output2 = secondValues2.newval;
    output2 = (output2*gain1100)/n;
    secondValues secondValues3 = sos(coef11003, bridge7, output2);
    bridge7[0] = bridge7[1];
    bridge7[1] = secondValues3.interval;
    output3 = secondValues3.newval;
    output3 = (output3*gain1100)/n;
    secondValues secondValues4 = sos(coef11004, bridge8, output3);
    bridge8[0] = bridge8[1];
    bridge8[1] = secondValues4.interval;
    output4 = secondValues4.newval;
    output4 = (output4*gain1100)/n;
    exit = 512 + output4;
    return exit;
}
