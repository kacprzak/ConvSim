#ifndef DTSS_CONVEYOR_H
#define DTSS_CONVEYOR_H

#include "atomic.h"
#include <string>
#include <iostream>
#include <list>

class Conveyor : public dtss::Atomic<double>
{
    struct Package
    {
        Package(double m, double p = 0)
            : mass(m), position(p) {}

        double mass; ///< Masa urobku [t]
        double position;   ///< Pozycja [m] na przenośniku
    };

    friend std::istream& operator>>(std::istream& is, Conveyor& conv);
    friend std::ostream& operator<<(std::ostream& os, const Conveyor& conv);

public:
    Conveyor();
    Conveyor(std::istream& is);

    void updateState(const double& x, unsigned int dt);
    void outputFunction(double& y) const;

    /** Długość przeniośnika */
    double length() const { return m_length; }

    /** Materiał na przenosniku [t] */
    double materialAmount() const;
    std::string name() const { return m_nazwa; }

private:
    double m_v;             ///< prędkość taśmy
    double m_length;        ///< dlugość przenośnika
    double m_beltWidth;     ///< szerokość taśmy
    int m_number;           ///< wykorzystywany w lokalizacji
    std::string m_oddzial;
    std::string m_nazwa;

    std::list<Package> m_packages;    ///< przyjęte paczki materiału
    double m_massOnOutput;            ///< materiał na wyjściu
};

inline std::ostream& operator<<(std::ostream& os, const Conveyor& conv)
{
    os << conv.m_number << "\t\t";
    os << conv.m_oddzial << "\t\t";
    os << conv.m_nazwa << "\t\t";
    os << conv.m_length << "\t\t";
    os << conv.m_beltWidth << "\n";
    return os;
}

#endif // DTSS_CONVEYOR_H
