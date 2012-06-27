#ifndef DTSS_CONVEYOR_H
#define DTSS_CONVEYOR_H

#include "atomic.h"
#include <string>
#include <iostream>
#include <list>
#include <set>

class Conveyor : public dtss::Atomic<double>
{
    struct Package
    {
        Package(double m, double p = 0)
            : mass(m), position(p) {}

        double mass;       ///< Masa urobku [t]
        double position;   ///< Pozycja [m] na przenośniku
    };

    friend std::istream& operator>>(std::istream& is, Conveyor& conv);
    friend std::ostream& operator<<(std::ostream& os, const Conveyor& conv);

public:
    Conveyor(const std::string& name,
             double length,
             double beltSpeed,
             int beltWidth);

    void delta(const std::set<double>& x);
    void outputFunction(std::set<double>& y) const;

    /** Długość przeniośnika */
    double length() const { return m_length; }

    void setDivision(const std::string& division) { m_division = division; }
    std::string division() const { return m_division; }

    void setNumber(int number) { m_number = number; }
    int number() const { return m_number; }

    /** Materiał na przenosniku [t] */
    double materialAmount() const;
    std::string name() const { return m_name; }

    // Static methods
    static Conveyor *create(const std::string& str);

private:
    std::string m_name;     ///< nazwa przenośnika
    double m_length;        ///< dlugość przenośnika
    double m_beltSpeed;     ///< prędkość taśmy
    double m_beltWidth;     ///< szerokość taśmy
    int m_number;           ///< wykorzystywany w lokalizacji
    std::string m_division; ///< oddział

    std::list<Package> m_packages;    ///< przyjęte paczki materiału
    double m_massOnOutput;            ///< materiał na wyjściu
};

inline std::ostream& operator<<(std::ostream& os, const Conveyor& conv)
{
    os << conv.m_number << "\t\t";
    os << conv.m_division << "\t\t";
    os << conv.m_name << "\t\t";
    os << conv.m_length << "\t\t";
    os << conv.m_beltWidth << "\n";
    return os;
}

#endif // DTSS_CONVEYOR_H
