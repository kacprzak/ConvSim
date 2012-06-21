#ifndef WEIGHINGBELT_H
#define WEIGHINGBELT_H

#include <iostream>

class WeighingBelt
{
    friend std::istream& operator>>(std::istream& is, WeighingBelt& dt);
    friend std::ostream& operator<<(std::ostream& os, const WeighingBelt& conv);

public:
    WeighingBelt();

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
