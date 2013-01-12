// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
#ifndef WEIGHINGBELT_H
#define WEIGHINGBELT_H

#include "eventlistener.h"
#include "conveyor.h"
#include <iostream>
#include <fstream>

/**
 * Nasłuchuje zdarzeń związanych ze zmianami na przenośnikach tasmowych
 */
class WeighingBelt : public dtss::EventListener<IO_type>
{
    friend std::istream& operator>>(std::istream& is, WeighingBelt& dt);
    friend std::ostream& operator<<(std::ostream& os, const WeighingBelt& conv);

public:
    WeighingBelt()
        : m_conveyor(nullptr)
    {
        //m_output.open("output.txt");
    }
    
    WeighingBelt(Conveyor *conv)
        : m_conveyor(conv)
    {
        m_output.open("output.txt");
    }

    virtual ~WeighingBelt()
    {
        m_output.close();
    }

    void stateChanged(dtss::Atomic<IO_type> *model, unsigned int t);
    void outputEvent(const dtss::Event<IO_type>& e, unsigned int t);
//    void setOdczyt(int n);

    // Static methods
    static WeighingBelt *create(const std::string& str);

private:
    int lp_prz; //< na ktorym instalujemy wage
    int lokalizacja; //< na ktorym metrze przenosnika znajduje sie waga
    int gdzie;
//    double *odczyt;
    Conveyor *m_conveyor; // Obserwowany przenośnik
    std::ofstream m_output;
};

inline std::ostream& operator<<(std::ostream& os, const WeighingBelt& wb)
{
    os << wb.lp_prz << "\t";
    os << wb.lokalizacja << "\n";
    return os;
}

#endif // WEIGHINGBELT_H
