#include <iostream>
#include <fstream>
#include <vector>

#include "event.h"
#include "conveyor.h"
#include "weighingbelt.h"
#include "tank.h"
#include "loadinggrid.h"
#include "simulator.h"

template <class T>
std::vector<T *> loadFromFile(const char *file)
{
    using namespace std;

    string tmp;
    vector<T *> v;
    ifstream f(file);

    if (f.is_open() == true) {
        while (f.good()) {
            getline(f, tmp);
            if (!tmp.empty())
                v.push_back(T::create(tmp));
        }
        f.close();
    } else {
        cerr << "Error: unable to open " << file << endl;
    }

    return v;
}


int main()
{
    using namespace std;

    cout << "Starting ConvSim ...\n";

    ////////////////////////////////////////////////////////////////////////////
    cout << "Loading Conveyors ... ";
    vector<Conveyor *> conveyors = loadFromFile<Conveyor>("przenosniki.txt");
    cout << conveyors.size() << "\n";

    ////////////////////////////////////////////////////////////////////////////
    cout << "Loading Tanks ... ";
    vector<Tank *> tanks = loadFromFile<Tank>("zbiorniki.txt");
    cout << tanks.size() << "\n";

    ////////////////////////////////////////////////////////////////////////////
    cout << "Loading Weighing Belts ... ";
    vector<WeighingBelt *> wbelts = loadFromFile<WeighingBelt>("wagi.txt");;
    cout << wbelts.size() << "\n";

    ////////////////////////////////////////////////////////////////////////////
    cout << "Loading Loading Grids ...\n";
    //stworzenie tablicy stuktur krat i wczytanie ich z pliku
    int n = 100; // ilosc cykli
    vector<LoadingGrid> grids;
    grids.push_back(LoadingGrid("kr1.txt", n));
    grids.push_back(LoadingGrid("kr2.txt", n));
    grids.push_back(LoadingGrid("kr3.txt", n));


    using namespace dtss;

    Conveyor *conv = conveyors[0];
    Simulator<double> sim(conv);
    WeighingBelt waga;
    sim.addEventListener(&waga);

    double input[] = {0.1, 0.2, 0.1, 0.3, 0.1, 0.5, 0.5,
                      0.1, 0.2, 0.1, 0.3, 0.1, 0.5, 0.5,
                      0.1, 0.2, 0.1, 0.3, 0.1, 0.5, 0.5,
                      0.1, 0.2, 0.1, 0.3, 0.1, 0.5, 0.5,
                      0.1, 0.2, 0.1, 0.3, 0.1, 0.5, 0.5,
                      0.1, 0.2, 0.1, 0.3, 0.1, 0.5, 0.5};
    int steps = sizeof(input)/sizeof(double);

    for (int n = 0; n < 300; ++n) {
        double material = 0.0;
        if (n < steps)
            material = input[n];
        set<Event<double> > in;
        in.insert(Event<double>(conv, material));

        sim.computeNextState(in);
        sim.computeOutput();
    }

    cout << "Czas symulacji: " << sim.getTime() << "\n";

#ifdef _MSC_VER
    system("pause");
#endif

    return 0;
}
