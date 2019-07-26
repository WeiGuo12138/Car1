
#ifndef __FILTER_H
#define __FILTER_H


// y(k + 1) = a * y(k) + (1 - a) * x(k)
typedef struct
{
    double a;
    double last_output;
} FILTER1_HANDLE, *P_FILTER1_HANDLE;

void filter1Reset(P_FILTER1_HANDLE phdl);
double filter1Process(P_FILTER1_HANDLE phdl, double input);


#endif
