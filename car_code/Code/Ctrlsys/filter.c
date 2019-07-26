#include "filter.h"

void filter1Reset(P_FILTER1_HANDLE phdl)
{
    phdl->last_output = 0;
}

double filter1Process(P_FILTER1_HANDLE phdl, double input)
{
    double output = phdl->a * phdl->last_output + (1 - phdl->a) * input;
    phdl->last_output = output;
    return output;
}

