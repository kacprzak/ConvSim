#ifndef TANK_H
#define TANK_H

#include <string>

class Tank
{
public:
    Tank();

//private:
    int numer_Tanka; //wykorzystywany w lokalizacji numer przenosnika za lub przed zbiornikiem ??????porzadkowy???????
    std::string odzial;
    std::string nazwa;
    double poj;	//pojemnosc zbiornika [t]
    double wydajnosc;
    double stan_Tanka;

    double *zapelnianie;
};

#endif // TANK_H
