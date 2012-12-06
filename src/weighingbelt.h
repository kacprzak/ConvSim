#ifndef WEIGHINGBELT_H
#define WEIGHINGBELT_H

#include "eventlistener.h"
#include "conveyor.h"
#include <iostream>

/**
 * Nasłuchuje zdarzeń związanych ze zmianami na przenośnikach tasmowych
 */
class WeighingBelt : public dtss::EventListener<IO_type>
{
    friend std::istream& operator>>(std::istream& is, WeighingBelt& dt);
    friend std::ostream& operator<<(std::ostream& os, const WeighingBelt& conv);

public:
    WeighingBelt() {}
    virtual ~WeighingBelt() {}

    void stateChanged(dtss::Atomic<IO_type> *model, unsigned int t);
    void outputEvent(const dtss::Event<IO_type>& /*e*/, unsigned int /*t*/)
    {
      // Nic nie robi
    }

    void setOdczyt(int n);

    // Static methods
    static WeighingBelt *create(const std::string& str);

private:
    int lp_prz; //< na ktorym instalujemy wage
    int lokalizacja; //< na ktorym metrze przenosnika znajduje sie waga
    int gdzie;
    double *odczyt;
};

inline std::ostream& operator<<(std::ostream& os, const WeighingBelt& wb)
{
    os << wb.lp_prz << "\t";
    os << wb.lokalizacja << "\n";
    return os;
}

#endif // WEIGHINGBELT_H
