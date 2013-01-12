// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
#ifndef TANK_H
#define TANK_H

#include "atomic.h"
#include "io_type.h"
#include <string>
#include <iostream>
#include <set>
#include <deque>

class Tank : public dtss::Atomic<IO_type>
{
    friend std::istream& operator>>(std::istream& is, Tank& tank);
    friend std::ostream& operator<<(std::ostream& os, const Tank& tank);

public:
    Tank();
    Tank(std::istream& is);

    // dtss:Atomic interface
    void delta(unsigned long dt, const std::set<IO_type>& x);
    void outputFunction(std::set<IO_type>& y) const;
    // end dtss:Atomic interface

    //void setZapelnianie(int n);

    /** Materiał w zbiorniku [t] */
    double materialAmount() const;
    std::string name() const { return m_nazwa; }

    void printMaterialDistribution(int precision = 0) const;

    // Static methods
    static Tank *create(const std::string& str);

private:
    void addPackage(Material material);

    double m_wydajnosc; // wydajność na wyjściu [t/h]
    //double stan_Tanka;

    //double *zapelnianie;

    double m_pojemnosc;	//pojemnosc zbiornika [t]

    //wykorzystywany w lokalizacji numer przenosnika za lub przed zbiornikiem ?porzadkowy?
    int m_numerZbiornika;
    std::string m_oddzial;
    std::string m_nazwa;
    std::deque<Material> m_packages; ///< Zakolejkowany materiał w zbiorniku
    Material m_materialOnOutput;     ///< Materiał na wyjściu
};

inline std::ostream& operator<<(std::ostream& os, const Tank& tank)
{
    os << tank.m_numerZbiornika << "\t";
    os << tank.m_oddzial << "\t";
    os << tank.m_nazwa << "\t";
    os << tank.m_pojemnosc << "\t";
    os << tank.m_wydajnosc << "\n";
    return os;
}

#endif // TANK_H
