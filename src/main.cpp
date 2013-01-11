// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
#include <iostream>
#include <fstream>
#include <vector>
//#include <utility>
#include <iomanip> // setw
#include <cstring> // strcmp
#include <cstdlib> // atoi

#include "utils.h"
#include "event.h"
#include "conveyor.h"
#include "weighingbelt.h"
#include "tank.h"
#include "loadinggrid.h"
#include "simulator.h"
#include "transportationsystem.h"

/**
 * Wyświetla informacje na ekranie.
 */
void printUI(unsigned int step,
             const std::vector<Conveyor *>& conveyors,
             const std::vector<Tank *>& tanks)
{
    using namespace std;
    clearScreen();
    cout << "Krok symulacji: " << step << '\n';

    // Wyświetl kilka przenosników
    cout << "Przenośniki:\n";
    int num = 15;
    for (auto it = conveyors.cbegin(); it != conveyors.cend() && num > 0; ++it) {
        cout << setw(10) << (*it)->name() << ' ';
        (*it)->printMaterialDistribution(100); // Wyświetl odcinkami po 100 [m]
        --num;
    }

    cout << endl;
    // Wyświetl kilka zbiorników
    cout << "Zbiorniki:\n";
    num = 5;
    for (auto it = tanks.cbegin(); it != tanks.cend() && num > 0; ++it) {
        cout << setw(10) << (*it)->name() << ' ';
        (*it)->printMaterialDistribution();
        --num;
    }
}

/**
 * Opcje:
 *   -s    spowolnienie symulacji
 */
int main(int argc, char **argv)
{
    bool slow = false;
    if (argc > 1 && strcmp(argv[1], "-s") == 0)
        slow = true;

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
    cout << "Loading Loading Grids ... ";
    vector<LoadingGrid *> grids;
    grids.push_back(new LoadingGrid("kr.txt"));
    grids.push_back(new LoadingGrid("kr.txt"));
    grids.push_back(new LoadingGrid("kr.txt"));
    cout << grids.size() << "\n";

    // System transportowy
    TransportationSystem ts;
    // Dodanie wszystkich przenosników do systemu
    for (auto it = conveyors.begin(); it != conveyors.end(); ++it) {
        ts.addConveyor(*it);
    }

    // Dodanie wszystkich zbiorników do systemu
    for (auto it = tanks.begin(); it != tanks.end(); ++it) {
        ts.addTank(*it);
    }

#if 1
    string filename = "connections.txt";
    char sep = ';';
    ifstream f(filename);

    if (f.is_open() == true) {
        while (f.good()) {
            string conn_type;
            string src;
            string out;
            string dst;
            string in;

            std::getline(f, conn_type, sep);
            std::getline(f, src, sep);
            std::getline(f, out, sep);
            std::getline(f, dst, sep);
            std::getline(f, in);

            if (conn_type.empty() || src.empty())
                break;

            int so = atoi(out.c_str());
            int di = atoi(in.c_str());

            cout << conn_type << ' '
                 << src << ' '
                 << so << ' '
                 << dst << ' '
                 << di << '\n';
            Conveyor *srcp = findByName(conveyors, src);
            Conveyor *dstp = findByName(conveyors, dst);
            ts.connect(srcp, so, dstp, di);
        }
        f.close();
    } else {
        cerr << "Error: unable to open " << filename << endl;
    }
#else
    ts.connect(conveyors[0], tanks[0]);
    ts.connect(tanks[0], conveyors[1]);

    // Połącz przenośniki szeregowo
    for (auto it = (conveyors.begin() + 1); it != conveyors.end();) {
        Conveyor *c1 = *it;
        ++it;
        if (it != conveyors.end()) {
            Conveyor *c2 = *it;
            ts.connect(c1, c2);
        }
    }
#endif

    pressAnyKey();

    // Czas jednego kroku symulacji [s]
    unsigned long dt = 1;
    dtss::Simulator<IO_type> sim(&ts, dt);

    // Obserwator symulacji
    //WeighingBelt waga;
    //sim.addEventListener(&waga);

    // Ilość kroków
    unsigned int max_step = 2000;
    // Pętla symulacji
    for (unsigned int step = 0; step < max_step; ++step) {
        // Treść zdarzenia: materiał na wejście nr 1 przenośnika
        IO_type material(1, 0.0);

        // Pobierz materiał z kraty
        //if (step < 100)
        material.second = grids[0]->getNextValue();

        // Zbiór zdarzeń wejściowych
        set<dtss::Event<IO_type> > in;
        // Jedno zdarzenie: przyjście materiału na przenośnik
        in.insert(dtss::Event<IO_type>(conveyors[0], material));

        // Oblicza stan wszystkich obiektów symulacji
        sim.computeNextState(in);
        // Oblicz wyjścia wszystkich obiektów symulacji
        sim.computeOutput();

        // Wyświetla informacje na ekranie
        if (step % 5 == 0) // co kilka klatek
            printUI(step, conveyors, tanks);

        if (slow) {
            // Spowalnia symulację
#ifdef WINDOWS
            Sleep(10);
#else
            usleep(10000);
#endif
        }
    }

    cout << "Czas symulacji: " << sim.getTime() << " [s]\n";

    // Zwolnienie pamięci
    freeClear<std::vector<Conveyor *> >(conveyors);
    freeClear<std::vector<Tank *> >(tanks);
    freeClear<std::vector<WeighingBelt *> >(wbelts);
    freeClear<std::vector<LoadingGrid *> >(grids);

    pressAnyKey();

    return 0;
}
