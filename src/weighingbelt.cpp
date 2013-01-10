// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
#include "weighingbelt.h"

#include <sstream>

inline std::istream& operator>>(std::istream& is, WeighingBelt& wb)
{
    is >> wb.lp_prz;
    is >> wb.lokalizacja;
    return is;
}


//void WeighingBelt::setOdczyt(int n)
//{
//    odczyt = new double[n];
//
//    for(int j = 0; j < n; ++j)
//    {
//        odczyt[j] = 0;
//    }
//}


WeighingBelt *WeighingBelt::create(const std::string &str)
{
    std::stringstream is(str);
    WeighingBelt *wb = new WeighingBelt;
    is >> *wb;
    return wb;
}


void WeighingBelt::stateChanged(dtss::Atomic<IO_type> *model, unsigned int t)
{
    //Conveyor *conv = static_cast<Conveyor*>(model);
    //std::cout << conv->name() << " [" << t << "]\t";
    //conv->printMaterialDistribution(70);
}
