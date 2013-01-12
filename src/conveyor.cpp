// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
#include "conveyor.h"

#include <sstream>
#include <cassert>
#include <iomanip>

Conveyor::Conveyor(const std::string& name, double length, double beltSpeed, int beltWidth)
    : m_name(name)
    , m_length(length)
    , m_beltSpeed(beltSpeed)
    , m_beltWidth(beltWidth)
    , m_number(0)
{
    addInput(0.0);
}

//------------------------------------------------------------------------------

Conveyor *Conveyor::create(const std::string& str)
{
    std::stringstream is(str);

    std::string division, name;
    double length;
    int beltWidth, number;

    is >> number;
    is >> division;
    is >> name;
    is >> length;
    is >> beltWidth;

    // FIXME: tymczasowo 2,5 m/s
    Conveyor *c = new Conveyor(name, length, 2.5, beltWidth);
    c->setDivision(division);
    c->setNumber(number);
    return c;
}

//------------------------------------------------------------------------------

void Conveyor::addPackage(Material material, double position)
{
    m_packages.push_back(Package(material, position));
}

//------------------------------------------------------------------------------

void Conveyor::delta(unsigned long dt, const std::set<IO_type>& x)
{
    m_materialOnOutput.clear();
    // Przesunięcie paczek na przenośniku
    for (std::list<Package>::iterator it = m_packages.begin(); it != m_packages.end(); ++it)
    {
        it->position += m_beltSpeed * dt;
    }

    // Paczki które są poza przenośnikiem są usuwane,
    // a ilośc wysypanego materiału zapamiętana
    for (std::list<Package>::iterator it = m_packages.begin(); it != m_packages.end();)
    {
        if (it->position > m_length) {
            m_materialOnOutput.push_back(it->material);
            it = m_packages.erase(it);
        } else {
            ++it;
        }
    }

    double massOnInput = 0.0;
    // Dodanie nowych paczek do przenośnika
    for (std::set<IO_type>::const_iterator it = x.cbegin(); it != x.cend(); ++it)
    {
        const IO_type& input = *it;
        double position = inputPosition(input.first);
        addPackage(input.second, position);
        massOnInput += input.second.mass();
    }

    m_chwilowaWydajnoscNaWejsciu = (massOnInput) / (dt / 3600.0) ; // kg*1000 / (s/3600)
    m_chwilowaWydajnoscNaWyjsciu = (Material::combine(m_materialOnOutput).mass()) / (dt / 3600.0) ; // kg*1000 / (s/3600)
}

//------------------------------------------------------------------------------

void Conveyor::outputFunction(std::set<IO_type>& y) const
{
    // Wyście 1 (bęben zrzutowy)
    y.insert(IO_type(1, Material::combine(m_materialOnOutput)));
}

//------------------------------------------------------------------------------
/**
 * Zwraca materiał znajdujący się na trasie pomiędzy znacznikami start oraz end.
 * Jeśli start i end są 0 to zwracany jest materiał z całej trasy.
 */
double Conveyor::materialAmount(double start, double end) const
{
    if (end == 0.0 || end > m_length)
        end = m_length;

    double mass = 0;

    for (std::list<Package>::const_iterator it = m_packages.cbegin();
         it != m_packages.cend(); ++it)
    {
        if (it->position >= start && it->position < end)
            mass += it->material.mass();
    }
    return mass;
}

//------------------------------------------------------------------------------
/**
 * Rozkład materiału na przenośniku.
 *
 * @param l         długość odcinków
 * @param precision ilość cyfr po przecinku
 */
void Conveyor::printMaterialDistribution(double l, int precision) const
{
    assert(l > 0.0);
    using namespace std;
    double pos = 0.0;

    cout << "[";
    while (pos < m_length) {
        cout << setprecision(precision) << fixed << setw(2) <<  materialAmount(pos, pos + l);
        pos += l;
        if (pos < m_length)
            cout << " ";
    }
    cout << "]";
}

//------------------------------------------------------------------------------

int Conveyor::addInput(double position)
{
    assert(position < m_length);
    m_inputPositions.push_back(position);
    return m_inputPositions.size();
}

//------------------------------------------------------------------------------

double Conveyor::inputPosition(int inputNumber) const
{
    return m_inputPositions[inputNumber - 1];
}
