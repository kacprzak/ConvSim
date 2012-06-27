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
    , m_massOnOutput(0.0)
{
}


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


void Conveyor::delta(const std::set<double>& x)
{
    m_massOnOutput = 0;
    // Przesunięcie paczek na przenośniku
    for(std::list<Package>::iterator it = m_packages.begin(); it != m_packages.end(); ++it)
    {
        int dt = 1; // [s]
        it->position = it->position + m_beltSpeed * dt;
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
    m_packages.push_back(Package(*(x.begin())));
}


void Conveyor::outputFunction(std::set<double>& y) const
{
    y.insert(m_massOnOutput);
}

/**
 * Zwraca materiał znajdujący się na trasie pomiędzy znacznikami start oraz end.
 * Jeśli start i end są 0 to zwracany jest materiał z całej trasy.
 */
double Conveyor::materialAmount(double start, double end) const
{
	if (end == 0.0 || end > m_length)
		end = m_length;

	double mass = 0;

    for(std::list<Package>::const_iterator it = m_packages.begin(); it != m_packages.end(); ++it)
    {
		if (it->position >= start && it->position < end)
			mass += it->mass;
    }
    return mass;
}


void Conveyor::printMaterialDistribution(double l) const
{
	assert(l > 0.0);
	using namespace std;
	double pos = 0.0;

	cout << "[";
	while (pos < m_length) {
		cout << setprecision(1) << fixed << materialAmount(pos, pos + l) << "  ";
		pos += l;
	}
    cout << "]\n";
}