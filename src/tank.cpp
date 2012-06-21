#include "tank.h"

Tank::Tank()
    : stan_Tanka(0)
{
}

void Tank::setZapelnianie(int n)
{
    zapelnianie = new double[n];
    for(int j = 0; j < n; ++j)
    {
        zapelnianie[j] = 0;
    }
}
