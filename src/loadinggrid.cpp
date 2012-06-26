#include "loadinggrid.h"

#include <fstream>

LoadingGrid::LoadingGrid(const std::string &file, int n)
{
    loadData(file, n);
}

void LoadingGrid::loadData(const std::string &file, int n)
{
    this->n = n;
    std::string pomin;
    std::fstream plik;

    plik.open(file.c_str(), std::ios::in | std::ios::binary);
    if(plik.is_open()==true)
    {
        plik >> lp_prz;
        plik >> lokalizacja;

        double tmp;
		for (int i = 0; i < n; ++i)
        {
            plik >> pomin;
            plik >> pomin;
            plik >> tmp;
			m_nadawa.push_back(tmp);
        }
        plik.close();
    }
    else std::cout << "nie mozna otworzyc pliku kr1.txt do odczytu";
}

