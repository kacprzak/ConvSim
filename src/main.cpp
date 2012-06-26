#include <iostream>
#include <fstream>
#include <vector>

#include "conveyor.h"
#include "weighingbelt.h"
#include "tank.h"
#include "loadinggrid.h"
#include "simulator.h"

template <class T>
void loadObjects(std::vector<T> *v, int size, const char *file)
{
    using namespace std;

    fstream plik;
    plik.open(file, ios::in | ios::binary);
    if (plik.is_open() == true) {
        for(int i = 0; i < size; ++i) {
            v->push_back(T(plik));
        }
        plik.close();
    } else
        cout << "plik" << file << "nie zostal otwarty" << "\n";
}


int main()
{
    using namespace std;

    cout << "Starting ConvSim ...\n";

    ////////////////////////////////////////////////////////////////////////////
    cout << "Loading Conveyors ...\n";
    // wczytanie Conveyorow z pliku do struktury
    int ile_conveyorow = 64;
    vector<Conveyor> conveyors;
    loadObjects<Conveyor>(&conveyors, ile_conveyorow, "przenosniki.txt");


    ////////////////////////////////////////////////////////////////////////////
    cout << "Loading Tanks ...\n";
    //stworzenie tablicy struktur Tanków i wczytanie z pliku
    int ile_tankow = 16;
    vector<Tank> tanks;
    loadObjects<Tank>(&tanks, ile_tankow, "zbiorniki.txt");

    ////////////////////////////////////////////////////////////////////////////
    cout << "Loading Weighing Belts ...\n";
    //stworzenie tablicy struktur wag i wczytanie ich z pliku
    int ile_wszystkich_wag = 6;
    vector<WeighingBelt> wbelts;
    loadObjects<WeighingBelt>(&wbelts, ile_wszystkich_wag, "wagi.txt");

    ////////////////////////////////////////////////////////////////////////////
    cout << "Loading Loading Grids ...\n";
    //stworzenie tablicy stuktur krat i wczytanie ich z pliku
    int n = 100; // ilosc cykli
    vector<LoadingGrid> grids;
    grids.push_back(LoadingGrid("kr1.txt", n));
    grids.push_back(LoadingGrid("kr2.txt", n));
    grids.push_back(LoadingGrid("kr3.txt", n));


    using namespace dtss;

    Simulator<double> sim(&conveyors[0]);
    WeighingBelt waga;
    sim.addEventListener(&waga);

    double input[] = {0.1, 0.2, 0.1, 0.3, 0.1, 0.5, 0.05};
    int steps = sizeof(input)/sizeof(double);

    for (int n = 0; n < steps; ++n) {
        std::set<double> in;
        in.insert(input[n]);
        sim.computeNextState(in);
        sim.computeOutput();
    }

    cout << "Czas symulacji: " << sim.getTime() << "\n";

#ifdef _MSC_VER
    system("pause");
#endif

    return 0;
}
