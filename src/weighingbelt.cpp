#include "weighingbelt.h"

WeighingBelt::WeighingBelt()
{
}

void WeighingBelt::setOdczyt(int n)
{
    odczyt = new double[n];

    for(int j = 0; j < n; ++j)
    {
        odczyt[j] = 0;
    }
}
