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

    void setZapelnianie(int n);
//private:


    double wydajnosc;
    double stan_Tanka;

    double *zapelnianie;

private:
    double m_poj;	//pojemnosc zbiornika [t]
    int m_numerZbiornika; //wykorzystywany w lokalizacji numer przenosnika za lub przed zbiornikiem ??????porzadkowy???????
    std::string m_oddzial;
    std::string m_nazwa;
};

inline std::istream& operator>>(std::istream& is, Tank& tank)
{
    is >> tank.m_numerZbiornika;
    is >> tank.m_oddzial;
    is >> tank.m_nazwa;
    is >> tank.m_poj;
    is >> tank.wydajnosc;
    return is;
}

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
