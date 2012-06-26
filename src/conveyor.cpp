#include "conveyor.h"

inline std::istream& operator>>(std::istream& is, Conveyor& conv)
{
    is >> conv.m_number;
    is >> conv.m_oddzial;
    is >> conv.m_nazwa;
    is >> conv.m_length;
    is >> conv.m_beltWidth;
    return is;
}

Conveyor::Conveyor()
    : m_v(0)
    , m_massOnOutput(0)
{
    // FIXME: tymczasowo 2,5 m/s
    m_v = 2.5;
}

Conveyor::Conveyor(std::istream &is)
    : m_v(0)
    , m_massOnOutput(0)
{
    // FIXME: tymczasowo 2,5 m/s
    m_v = 2.5;
    is >> *this;
}

void Conveyor::updateState(const double &x, unsigned int dt)
{
    m_massOnOutput = 0;
    // Przesunięcie paczek na przenośniku
    for(std::list<Package>::iterator it = m_packages.begin(); it != m_packages.end(); ++it)
    {
        it->position = it->position + m_v * dt;
    }

    // Paczki które są poza przenośnikiem są usuwane,
    // a ilośc wysypanego materiału zapamiętana
    for(std::list<Package>::iterator it = m_packages.begin(); it != m_packages.end();)
    {
        if (it->position > m_length) {
            m_massOnOutput += it->mass;
            it = m_packages.erase(it);
        } else {
            ++it;
        }
    }

    // Dodanie nowej paczki na początek przenośnika
    m_packages.push_back(Package(x));
}

void Conveyor::outputFunction(double& y) const
{
    y = m_massOnOutput;
}

double Conveyor::materialAmount() const
{
    double mass = 0;

    for(std::list<Package>::const_iterator it = m_packages.begin(); it != m_packages.end(); ++it)
    {
        mass += it->mass;
    }
    return mass;
}
