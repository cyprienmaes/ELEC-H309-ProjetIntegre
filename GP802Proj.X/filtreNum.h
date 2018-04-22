#ifndef FILTRENUM_H
#define	FILTRENUM_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* FILTRENUM_H */

typedef struct secondValues {
    long interval;
    long newval;
}secondValues;

secondValues sos(long coef[6], long *bridge, long input);

int filtre900(int input,long *bridge1, long *bridge2, long *bridge3, long *bridge4);

int filtre1100(int input,long *bridge1, long *bridge2, long *bridge3, long *bridge4);