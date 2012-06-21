#ifndef CONVEYOR_H
#define CONVEYOR_H

#include "atomic.h"
#include <string>
#include <iostream>

class Conveyor : public Atomic<double>
{
    friend std::istream& operator>>(std::istream& is, Conveyor& dt);
    friend std::ostream& operator<<(std::ostream& os, const Conveyor& conv);

public:
    Conveyor();

    void updateState(double x, unsigned int dt);
    void updateOutput();
    double output();

    /** Długość przeniośnika */
    double length() { return m_length; }

    void printUrobek();
    void printIloscPaczek();
    void setIloscPaczek(int tr);

//private:
    int il_paczek; ///< ile paczek ma przenosnik
    float *urobek;
    int ile_wag;

private:
    double m_v;             ///< prędkość taśmy
    double m_length;        ///< dlugość przenośnika
    double m_beltWidth;     ///< szerokość taśmy
    int m_number;           ///< wykorzystywany w lokalizacji
    std::string m_oddzial;
    std::string m_nazwa;
};

inline std::istream& operator>>(std::istream& is, Conveyor& conv)
{
    is >> conv.m_number;
    is >> conv.m_oddzial;
    is >> conv.m_nazwa;
    is >> conv.m_length;
    is >> conv.m_beltWidth;
    return is;
}

inline std::ostream& operator<<(std::ostream& os, const Conveyor& conv)
{
    os << conv.m_number << "\t\t";
    os << conv.m_oddzial << "\t\t";
    os << conv.m_nazwa << "\t\t";
    os << conv.m_length << "\t\t";
    os << conv.m_beltWidth << "\n";
    return os;
}

#endif // CONVEYOR_H
