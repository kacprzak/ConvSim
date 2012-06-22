#ifndef WEIGHINGBELT_H
#define WEIGHINGBELT_H

#include "eventlistener.h"
#include <iostream>

/** Nasłuchuje zdarzeń związanych ze zmianami na przenośnikach tasmowych */
class WeighingBelt : public dtss::EventListener<double>
{
    friend std::istream& operator>>(std::istream& is, WeighingBelt& dt);
    friend std::ostream& operator<<(std::ostream& os, const WeighingBelt& conv);

public:

    void stateChanged(dtss::Atomic<double> *model, unsigned int t);
    //void outputEvent(Atomic<double> *model, const double & output, unsigned int t);

    void setOdczyt(int n);

//private:
    int lp_prz; //< na ktorym instalujemy wage
    int lokalizacja; //< na ktorym metrze przenosnika znajduje sie waga
    int gdzie;
    double *odczyt;

private:
};

inline std::istream& operator>>(std::istream& is, WeighingBelt& wb)
{
    is >> wb.lp_prz;
    is >> wb.lokalizacja;
    return is;
}

inline std::ostream& operator<<(std::ostream& os, const WeighingBelt& wb)
{
    os << wb.lp_prz << "\t";
    os << wb.lokalizacja << "\n";
    return os;
}

#endif // WEIGHINGBELT_H
