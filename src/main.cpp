#include <iostream>
#include <fstream>
#include <vector>
#include <utility>

#include "event.h"
#include "conveyor.h"
#include "weighingbelt.h"
#include "tank.h"
#include "loadinggrid.h"
#include "simulator.h"

// Jeśli kompilujemy MSVC to na pewno jesteśmy na Windowsie
#ifdef _MSC_VER
  #define WINDOWS
#endif

#ifdef WINDOWS
  #include <windows.h> // Sleep
#else
  #include <unistd.h> // usleep
#endif

/**
 * Czyta plik linijka po linijce. Każdą przeczytaną linijkę tekstu
 * przekazuje do metody create, która tworzy obiekt typu T i zwraca
 * wskaźnik na niego. Funkcja zwaraca listę wskaźników na utworzone
 * obiekty.
 */
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

/**
 * Przyjmuje listę wskaźników. Następnie kasuje wszystkie
 * obiekty na które wskazują wskaźniki oraz czyści listę.
 */
template <class T> void freeClear(T& cntr)
{
    for (typename T::iterator it = cntr.begin();
         it != cntr.end(); ++it )
    {
        delete *it;
    }
    cntr.clear();
}

int main()
{
    using namespace std;

    cout << "Starting ConvSim ...\n";

    //-------------------------------------------------------------------------
    cout << "Loading Conveyors ... ";
    vector<Conveyor *> conveyors = loadFromFile<Conveyor>("przenosniki.txt");
    cout << conveyors.size() << "\n";

    //-------------------------------------------------------------------------
    cout << "Loading Tanks ... ";
    vector<Tank *> tanks = loadFromFile<Tank>("zbiorniki.txt");
    cout << tanks.size() << "\n";

    //-------------------------------------------------------------------------
    cout << "Loading Weighing Belts ... ";
    vector<WeighingBelt *> wbelts = loadFromFile<WeighingBelt>("wagi.txt");;
    cout << wbelts.size() << "\n";

    //-------------------------------------------------------------------------
    cout << "Loading Loading Grids ...\n";
    //stworzenie tablicy stuktur krat i wczytanie ich z pliku
    int n = 100; // ilosc cykli
    vector<LoadingGrid> grids;
    grids.push_back(LoadingGrid("kr1.txt", n));
    grids.push_back(LoadingGrid("kr2.txt", n));
    grids.push_back(LoadingGrid("kr3.txt", n));


    using namespace dtss;

    Conveyor *conv = conveyors[0];
    // Symulacja jednego przenośnika
    Simulator<IO_type> sim(conv);
    // Obserwator symulacji
    WeighingBelt waga;
    sim.addEventListener(&waga);

    // Strumień danych wejściowych do symulatora
    double input[] = {0.1, 0.2, 0.4, 0.3, 0.1, 0.5, 0.5,
                      0.5, 0.2, 0.1, 0.3, 0.3, 0.5, 0.1,
                      0.4, 0.3, 0.2, 0.5, 0.1, 0.3, 0.5,
                      0.3, 0.2, 0.1, 0.3, 0.2, 0.5, 0.7,
                      0.2, 0.7, 0.5, 0.8, 0.1, 0.3, 0.5,
                      0.1, 0.2, 0.1, 0.3, 0.2, 0.5, 0.1};
    int steps = sizeof(input)/sizeof(double);

    for (int n = 0; n < 300; ++n) {
        // Treść zdarzenia: materiał na wejście nr 1 przenośnika
        IO_type material(1, 0.0);
        if (n < steps)
            material.second = input[n];

        // Zbiór zdarzeń wejściowych
        set<Event<IO_type> > in;
        // Jedno zdarzenie: przyjście materiału na przenośnik
        in.insert(Event<IO_type>(conv, material));

        // Oblicza stan wszystkich obiektów symulacji
        sim.computeNextState(in);
        // Oblicz wyjścia wszystkich obiektów symulacji
        sim.computeOutput();

        // Spowalnia symulację
#ifdef WINDOWS
        Sleep(100); // 0.1 [s]
#else
        usleep(100000); // 0.1 [s]
#endif
    }

    cout << "Czas symulacji: " << sim.getTime() << "\n";

    // Zwolnienie pamięci
    freeClear<std::vector<Conveyor *> >(conveyors);
    freeClear<std::vector<Tank *> >(tanks);
    freeClear<std::vector<WeighingBelt *> >(wbelts);
    //freeClear<std::vector<LoadingGrid *> >(grids);

#ifdef WINDOWS
    system("pause");
#endif

    return 0;
}
