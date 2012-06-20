#ifndef CONVEYOR_H
#define CONVEYOR_H

#include <string>
#include <iostream>

class Conveyor
{
    friend std::istream& operator>>(std::istream& is, Conveyor& dt);
    friend std::ostream& operator<<(std::ostream& os, const Conveyor& conv);

public:
    Conveyor();

//private:
    int lp_prz; //< wykorzystywany w lokalizacji
    std::string oddzial;
    std::string nazwa;
    double L; //< dlugosc
    double B; //< chyba szerokość taśmy

    float v; //< prędkość taśmy

    int il_paczek; // ile paczek ma przenosnik
    float *urobek;

    int ile_wag;
};

inline std::istream& operator>>(std::istream& is, Conveyor& conv)
{
    is >> conv.lp_prz;
    is >> conv.oddzial;
    is >> conv.nazwa;
    is >> conv.L;
    is >> conv.B;
    return is;
}

inline std::ostream& operator<<(std::ostream& os, const Conveyor& conv)
{
    os << conv.lp_prz << "\t\t";
    os << conv.oddzial << "\t\t";
    os << conv.nazwa << "\t\t";
    os << conv.L << "\t\t";
    os << conv.B << "\n";
    return os;
}

#endif // CONVEYOR_H
