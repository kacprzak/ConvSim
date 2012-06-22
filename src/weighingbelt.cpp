#include "weighingbelt.h"

#include "conveyor.h"

void WeighingBelt::setOdczyt(int n)
{
    odczyt = new double[n];

    for(int j = 0; j < n; ++j)
    {
        odczyt[j] = 0;
    }
}

void WeighingBelt::stateChanged(dtss::Atomic<double> *model, unsigned int t)
{
    Conveyor *conv = static_cast<Conveyor*>(model);
    std::cout << "Na " << conv->name()
              << " jest " << conv->materialAmount() << " [t]\n";
}
