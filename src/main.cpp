#include <iostream>
#include <fstream>

#include "conveyor.h"
#include "weighingbelt.h"
#include "tank.h"
#include "loadinggrid.h"
#include "simulator.h"

#define VERBOSE 0

template <class T>
void loadObjects(T *tab, int size, const char *file)
{
    using namespace std;

    fstream plik;
    plik.open(file, ios::in | ios::binary);
    if(plik.is_open() == true) //sprawdz czy plik zostal otwarty
    {
        for(int i=0; i < size; i++)
        {
            plik >> tab[i];
        }
        plik.close();
        //delete []wydajnosc;
    }
    else cout << "plik" << file << "nie zostal otwarty" << "\n";

#if VERBOSE
    // niepotrzebne sprawdzenie czy sie wczytalo
    for (int i=0; i < size; i++)
    {
        cout << tab[i];
    }
    cout << "\n\n";
#endif
}


int main()
{
    using namespace std;

    cout << "Starting ConvSim ...\n";

    ////////////////////////////////////////////////////////////////////////////
    cout << "Loading Conveyors ...\n";
    // wczytanie Conveyorow z pliku do struktury
    int ile_Conveyorow=64;// ilosc Conveyoraow
    Conveyor *tabp = new Conveyor[ile_Conveyorow];
    loadObjects<Conveyor>(tabp, ile_Conveyorow, "przenosniki.txt");


    ////////////////////////////////////////////////////////////////////////////
    cout << "Loading Tanks ...\n";
    //stworzenie tablicy struktur Tanków i wczytanie z pliku
    int ile_Tankow=16;
    Tank *tabz=new Tank[ile_Tankow]; //wskaznik na tablice w ktorej sa zapisywane struktury Tanka
    loadObjects<Tank>(tabz, ile_Tankow, "zbiorniki.txt");

    ////////////////////////////////////////////////////////////////////////////
    cout << "Loading Weighing Belts ...\n";
    //stworzenie tablicy struktur wag i wczytanie ich z pliku
    int ile_wszystkich_wag=6;
    WeighingBelt *tabw= new WeighingBelt[ile_wszystkich_wag];
    loadObjects<WeighingBelt>(tabw, ile_wszystkich_wag, "wagi.txt");

    ////////////////////////////////////////////////////////////////////////////
    cout << "Loading Loading Grids ...\n";
    //stworzenie tablicy stuktur krat i wczytanie ich z pliku
    int n = 100; // ilosc cykli
    int ile_krat=3;
    LoadingGrid *tabk = new LoadingGrid[ile_krat];
    tabk[0].loadData("kr1.txt", n);
    tabk[1].loadData("kr2.txt", n);
    tabk[2].loadData("kr3.txt", n);


    using namespace dtss;

    Simulator<double> sim(&tabp[0]);
    WeighingBelt waga;
    sim.addEventListener(&waga);

    double input[] = {0.1, 0.2, 0.1, 0.3, 0.1, 0.5, 0.05};
    int steps = sizeof(input)/sizeof(double);

    for (int n = 0; n < steps; ++n) {
        sim.computeNextState(input[n]);
        sim.computeOutput();
    }

    cout << "Czas symulacji: " << sim.getTime() << "\n";

#ifdef _MSC_VER
	system("pause");
#endif

    return 0;
}
