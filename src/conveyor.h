// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
#ifndef CONVEYOR_H
#define CONVEYOR_H

#include "atomic.h"
#include <string>
#include <iostream>
#include <list>
#include <set>
#include <vector>
#include <utility>

// Typ wejścia wyjścia
typedef std::pair<int, double> IO_type;

/**
 * @brief Model przenośnika taśmowego.
 *
 * Model ma domyślinie jedno wejście dla materiału
 * (początek przenosnika) i jedno wyjście (koniec przenośnika).
 *
 * W zbiorach wejściwych i wyjściowych są pary wartości <numer IO, wartość>.
 */
class Conveyor : public dtss::Atomic<IO_type>
{
    /**
     * Paczka materiału przesuwająca się po przenośniku.
     */
    struct Package
    {
        Package(double m, double p = 0.0)
            : mass(m), position(p) {}

        double mass;       ///< Masa urobku [t]
        double position;   ///< Pozycja [m] na przenośniku
    };

    //friend std::istream& operator>>(std::istream& is, Conveyor& conv);
    friend std::ostream& operator<<(std::ostream& os, const Conveyor& conv);

public:
    Conveyor(const std::string& name,
             double length,
             double beltSpeed,
             int beltWidth);

    // dtss:Atomic interface
    void delta(unsigned long dt, const std::set<IO_type>& x);
    void outputFunction(std::set<IO_type>& y) const;

    /** Długość przeniośnika */
    double length() const { return m_length; }

    void setDivision(const std::string& division) { m_division = division; }
    std::string division() const { return m_division; }

    void setNumber(int number) { m_number = number; }
    int number() const { return m_number; }

    /** Materiał na przenosniku [t] */
    double materialAmount(double start = 0, double end = 0) const;
    std::string name() const { return m_name; }

    void printMaterialDistribution(double l, int precision = 0) const;

    /** Ilość wejść */
    int numberOfInputs() const { return m_inputPositions.size(); }
    /** Dodaje wejście na określonej pozycji */
    int addInput(double position);

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

    std::vector<double> m_inputPositions; ///< pozycja wejść

    void addPackage(double materialMass, double position = 0.0);
    double inputPosition(int inputNumber) const;
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

#endif // CONVEYOR_H
