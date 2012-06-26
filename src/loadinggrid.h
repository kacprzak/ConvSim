#ifndef LOADINGGRID_H
#define LOADINGGRID_H

#include <string>
#include <iostream>
#include <vector>

class LoadingGrid
{
    friend std::ostream& operator<<(std::ostream& os, const LoadingGrid& lg);

public:
    LoadingGrid(const std::string& file, int n);

private:
    void loadData(const std::string& file, int n);

    int lp_prz;
    int lokalizacja; // na ktorym metrze znajduje sie krata
    //int gdzie;
    std::vector<double> m_nadawa;

    int n;
};

inline std::ostream& operator<<(std::ostream& os, const LoadingGrid& lg)
{
    os << "Krata na przeniosniku nr: " << lg.lp_prz << " zlokalizowana na metrze" << lg.lokalizacja << "\n";
    for (int j = 0; j < lg.n; ++j)
    {
        //os << lg.nadawa[j] <<" ";
    }
    os << "\n\n";

    return os;
}

#endif // LOADINGGRID_H
