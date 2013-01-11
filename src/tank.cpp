// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
#include "tank.h"

#include <sstream>
#include <iomanip>

inline std::istream& operator>>(std::istream& is, Tank& tank)
{
    is >> tank.m_numerZbiornika;
    is >> tank.m_oddzial;
    is >> tank.m_nazwa;
    is >> tank.m_pojemnosc;
    is >> tank.m_wydajnosc;
    return is;
}

//------------------------------------------------------------------------------

Tank::Tank()
    : stan_Tanka(0)
    , m_massOnOutput(0)
{
}

//------------------------------------------------------------------------------

Tank::Tank(std::istream &is)
    : stan_Tanka(0)
    , m_massOnOutput(0)
{
    is >> *this;
}

//------------------------------------------------------------------------------

void Tank::setZapelnianie(int n)
{
    zapelnianie = new double[n];
    for(int j = 0; j < n; ++j)
    {
        zapelnianie[j] = 0;
    }
}

//------------------------------------------------------------------------------

Tank *Tank::create(const std::string& str)
{
    std::stringstream is(str);

    Tank *t = new Tank;
    is >> *t;
    return t;
}

//------------------------------------------------------------------------------

void Tank::delta(unsigned long dt, const std::set<IO_type>& x)
{
    m_massOnOutput = 0;
    double massToRemove = m_wydajnosc * dt;

    while (!m_packages.empty() && m_massOnOutput < massToRemove) {
        double p = m_packages.front();
        m_massOnOutput += p;
        if (m_massOnOutput <= massToRemove) {
            m_packages.pop_front();
        } else {
            m_packages.front() = m_massOnOutput - massToRemove;
            m_massOnOutput = massToRemove;
        }
    }

    // Dodanie nowych paczek do zbiornika
    for (std::set<IO_type>::const_iterator it = x.cbegin(); it != x.cend(); ++it)
    {
        const IO_type& input = *it;
        addPackage(input.second);
    }
}

//------------------------------------------------------------------------------

void Tank::outputFunction(std::set<IO_type>& y) const
{
    // Wyście 1
    y.insert(IO_type(1, m_massOnOutput));
}

//------------------------------------------------------------------------------

void Tank::addPackage(double materialMass)
{
    m_packages.push_back(materialMass);
}

//------------------------------------------------------------------------------

double Tank::materialAmount() const
{
    double mass = 0;

    for (std::deque<double>::const_iterator it = m_packages.cbegin();
         it != m_packages.cend(); ++it)
    {
        mass += *it;
    }
    return mass;
}

//------------------------------------------------------------------------------

void Tank::printMaterialDistribution(int precision) const
{
    using namespace std;
    cout << "[" << setprecision(precision) << fixed << materialAmount() << "]\n";
}
