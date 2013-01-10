// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
#ifndef TANK_H
#define TANK_H

#include <string>
#include <iostream>

class Tank
{
    friend std::istream& operator>>(std::istream& is, Tank& dt);
    friend std::ostream& operator<<(std::ostream& os, const Tank& conv);

public:
    Tank();
    Tank(std::istream& is);

    void setZapelnianie(int n);

    // Static methods
    static Tank *create(const std::string& str);

private:
    double wydajnosc;
    double stan_Tanka;

    double *zapelnianie;

private:
    double m_poj;	//pojemnosc zbiornika [t]
    int m_numerZbiornika; //wykorzystywany w lokalizacji numer przenosnika za lub przed zbiornikiem ??????porzadkowy???????
    std::string m_oddzial;
    std::string m_nazwa;
};

inline std::ostream& operator<<(std::ostream& os, const Tank& tank)
{
    os << tank.m_numerZbiornika << "\t";
    os << tank.m_oddzial << "\t";
    os << tank.m_nazwa << "\t";
    os << tank.m_poj << "\t";
    os << tank.wydajnosc << "\n";
    return os;
}

#endif // TANK_H
