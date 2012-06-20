#include "conveyor.h"

Conveyor::Conveyor()
{
}

void Conveyor::printUrobek()
{
    std::cout << "z Conveyora " << lp_prz << " (waga) ";

    for(int i = 0; i < il_paczek; ++i)
    {
        std::cout << urobek[i] << " ";
    }
    std::cout << "\n";
}

void Conveyor::printIloscPaczek()
{
    std::cout << "ilos paczek Conveyora "
              << lp_prz <<" tyle paczek -> "
              << il_paczek << "\n";
}
