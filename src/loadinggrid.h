// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
#ifndef LOADINGGRID_H
#define LOADINGGRID_H

#include <string>
#include <iostream>
#include <vector>

class LoadingGrid
{
    friend std::ostream& operator<<(std::ostream& os, const LoadingGrid& lg);

public:
    LoadingGrid(const std::string& file);

    /// Następna porcja materiału [t]
    double getNextValue(unsigned int t)
    {
        unsigned int current_n = (t / 4) % m_nadawa.size();
        return m_nadawa[current_n] / 1000.0; // kg/1000
    }

private:
    void loadData(const std::string& file);

    int lp_prz;
    int lokalizacja; // na ktorym metrze znajduje sie krata
    //int gdzie;
    std::vector<double> m_nadawa; // [kg]
};

inline std::ostream& operator<<(std::ostream& os, const LoadingGrid& lg)
{
    //os << "Krata na przeniosniku nr: " << lg.lp_prz << " zlokalizowana na metrze" << lg.lokalizacja << "\n";
    for (size_t i = 0; i < lg.m_nadawa.size(); ++i)
    {
        os << lg.m_nadawa[i] << ' ';
    }
    os << "\n\n";
    return os;
}

#endif // LOADINGGRID_H
