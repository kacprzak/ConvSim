#ifndef TANK_H
#define TANK_H

#include <string>
#include <iostream>

class Tank
{
public:
    Tank();

//private:
    int numer_Tanka; //wykorzystywany w lokalizacji numer przenosnika za lub przed zbiornikiem ??????porzadkowy???????
    std::string odzial;
    std::string nazwa;
    double poj;	//pojemnosc zbiornika [t]
    double wydajnosc;
    double stan_Tanka;

    double *zapelnianie;
};

inline std::istream& operator>>(std::istream& is, Tank& tank)
{
    is >> tank.numer_Tanka;
    is >> tank.odzial;
    is >> tank.nazwa;
    is >> tank.poj;
    is >> tank.wydajnosc;
    return is;
}

inline std::ostream& operator<<(std::ostream& os, const Tank& tank)
{
    os << tank.numer_Tanka << "\t";
    os << tank.odzial << "\t";
    os << tank.nazwa << "\t";
    os << tank.poj << "\t";
    os << tank.wydajnosc << "\n";
    return os;
}

#endif // TANK_H
