#include "tank.h"

inline std::istream& operator>>(std::istream& is, Tank& tank)
{
    is >> tank.m_numerZbiornika;
    is >> tank.m_oddzial;
    is >> tank.m_nazwa;
    is >> tank.m_poj;
    is >> tank.wydajnosc;
    return is;
}

Tank::Tank()
    : stan_Tanka(0)
{
}

Tank::Tank(std::istream &is)
    : stan_Tanka(0)
{
    is >> *this;
}

void Tank::setZapelnianie(int n)
{
    zapelnianie = new double[n];
    for(int j = 0; j < n; ++j)
    {
        zapelnianie[j] = 0;
    }
}
