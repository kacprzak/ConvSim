// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
#include "weighingbelt.h"

#include <sstream>

inline std::istream& operator>>(std::istream& is, WeighingBelt& wb)
{
    is >> wb.lp_prz;
    is >> wb.lokalizacja;
    return is;
}

WeighingBelt *WeighingBelt::create(const std::string &str)
{
    std::stringstream is(str);
    WeighingBelt *wb = new WeighingBelt;
    is >> *wb;
    return wb;
}

void WeighingBelt::stateChanged(dtss::Atomic<IO_type> *model, unsigned int t)
{
    //    std::cout << t;
    //Conveyor *conv = static_cast<Conveyor*>(model);
    //std::cout << conv->name() << " [" << t << "]\t";
    //conv->printMaterialDistribution(70);
    //m_output << t;
}

void WeighingBelt::outputEvent(const dtss::Event<IO_type>& e, unsigned int t)
{
    if (e.model() == m_conveyor) {
        Material m = e.value().second;
        m_output << t << '\t' 
                 << m.weglanowej << '\t'
                 << m.piaskowcowej << '\t'
                 << m.lupkowej << '\t'
                 << std::endl;
     }
}
