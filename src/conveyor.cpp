#include "conveyor.h"

Conveyor::Conveyor()
    : m_v(0)
    , m_massOnOutput(0)
{
    // FIXME: tymczasowo 2,5 m/s
    m_v = 2.5;
    il_paczek = 0;
}

void Conveyor::updateState(const double &x, unsigned int dt)
{
    m_massOnOutput = 0;
    // Przesunięcie paczek na przenośniku
    for(std::list<Package>::iterator it = m_packages.begin(); it != m_packages.end(); ++it)
    {
        it->position = it->position + m_v * dt;
    }

    // Dodanie nowej paczki na początek przenośnika
    m_packages.push_back(Package(x));
}

void Conveyor::updateOutput(double& y)
{
    // Paczki które są poza przenośnikiem są usuwane a ilośc wysypanego
    // materiału zapamiętana
    for(std::list<Package>::iterator it = m_packages.begin(); it != m_packages.end();)
    {
        if (it->position > m_length) {
            m_massOnOutput += it->mass;
            it = m_packages.erase(it);
        } else {
            ++it;
        }
    }
    y = m_massOnOutput;
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

void Conveyor::setIloscPaczek(int tr)
{
    float t_przejazdu = m_length / m_v;
    //float ile_pf=t_przejazdu/tr;
    int il_p = t_przejazdu / tr;
    il_paczek = t_przejazdu / tr;

    if(((t_przejazdu/tr)-il_p)>0) // tworzenie paczki jezeli na koncu nie wychodzi rowno
    {
        il_paczek++;
    }

    urobek = new float[il_paczek];
    for(int j = 0; j < il_paczek; ++j)
    {
        urobek[j] = 0;
    }
}
