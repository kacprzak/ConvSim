#ifndef CONVEYOR_H
#define CONVEYOR_H

#include <string>

class Conveyor
{
public:
    Conveyor();

//private:
    int lp_prz; //< wykorzystywany w lokalizacji
    std::string oddzial;
    std::string nazwa;
    double L; //< dlugosc
    double B; //< chyba szerokość taśmy

    int il_paczek; // ile paczek ma przenosnik
    float *urobek;

    int ile_wag;
};

#endif // CONVEYOR_H
