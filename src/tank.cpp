// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
#include "tank.h"

#include <sstream>
#include <iomanip>
#include <cassert>

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
{
}

//------------------------------------------------------------------------------

Tank::Tank(std::istream &is)
{
    is >> *this;
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
    m_materialOnOutput = Material();
    double massToRemove = (m_wydajnosc/3600.0) * dt; // t/s * s
 
    while (!m_packages.empty() && m_materialOnOutput.mass() < massToRemove) {
        Material p = m_packages.front();
        Material afterAdd = m_materialOnOutput + p;
        if (afterAdd.mass() <= massToRemove) {
            m_materialOnOutput += p;
            m_packages.pop_front();
        } else {
            // Brakująca masa
            double massToTake = massToRemove - m_materialOnOutput.mass();
            double ratio = massToTake / p.mass();
            assert(ratio < 1.0 && ratio > 0.0);
            Material toReclame = p * ratio;
            m_materialOnOutput += toReclame;
            m_packages.front() = p * (1.0 - ratio);
            break;
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
    y.insert(IO_type(1, m_materialOnOutput));
}

//------------------------------------------------------------------------------

void Tank::addPackage(Material material)
{
    m_packages.push_back(material);
}

//------------------------------------------------------------------------------

double Tank::materialAmount() const
{
    double mass = 0;

    for (auto it = m_packages.cbegin(); it != m_packages.cend(); ++it)
    {
        mass += it->mass();
    }
    return mass;
}

//------------------------------------------------------------------------------

void Tank::printMaterialDistribution(int precision) const
{
    using namespace std;
    cout << "[" << setprecision(precision) << fixed << materialAmount() 
         << " / " << m_pojemnosc << "]\n";
}
