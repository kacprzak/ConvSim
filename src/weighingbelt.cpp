#include "weighingbelt.h"

#include "conveyor.h"

inline std::istream& operator>>(std::istream& is, WeighingBelt& wb)
{
    is >> wb.lp_prz;
    is >> wb.lokalizacja;
    return is;
}

WeighingBelt::WeighingBelt(std::istream &is)
{
    is >> *this;
}

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
              << " jest " << conv->materialAmount() << " [t]"
              << " w czasie " << t << "\n";
}
