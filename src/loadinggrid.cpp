#include "loadinggrid.h"

#include <fstream>

void LoadingGrid::loadData(const std::string &file, int n)
{
    this->n = n;
    nadawa = new double[n];
    std::string pomin;
    std::fstream plik;

    plik.open(file.c_str(), std::ios::in | std::ios::binary);
    if(plik.is_open()==true)
    {
        plik >> lp_prz;
        plik >> lokalizacja;
        //tabk[i].gdzie=tabk[i].lokalizacja/odcinek;
        for(int j=0; j < n; j++)
        {
            plik >> pomin;
            plik >> pomin;
            plik >> nadawa[j];
        }
        plik.close();
    }
    else std::cout << "nie mozna otworzyc pliku kr1.txt do odczytu";
}

LoadingGrid::~LoadingGrid()
{
    delete [] nadawa;
}
