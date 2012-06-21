#include "conveyor.h"

Conveyor::Conveyor()
    : m_v(0)
{
    // FIXME: tymczasowo 2,5 m/s
    m_v = 2.5;
}

void Conveyor::updateState(double x)
{
}

void Conveyor::updateOutput()
{
}

double Conveyor::output()
{
}

void Conveyor::printUrobek()
{
    std::cout << "z Conveyora " << m_number << " (waga) ";

    for(int i = 0; i < il_paczek; ++i)
    {
        std::cout << urobek[i] << " ";
    }
    std::cout << "\n";
}

void Conveyor::printIloscPaczek()
{
    std::cout << "ilos paczek Conveyora "
              << m_number <<" tyle paczek -> "
              << il_paczek << "\n";
}
