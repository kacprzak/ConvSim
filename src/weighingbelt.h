#ifndef WEIGHINGBELT_H
#define WEIGHINGBELT_H

class WeighingBelt
{
public:
    WeighingBelt();

//private:
    int lp_prz; //< na ktorym instalujemy wage
    int lokalizacja; //< na ktorym metrze przenosnika znajduje sie waga
    int gdzie;
    double *odczyt;
};

#endif // WEIGHINGBELT_H
