// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
//#include <iostream>
//#include <fstream>
#include <vector>
//#include <utility>
#include <iomanip> // setw
#include <cstring> // strcmp

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
        Conveyor *c = *it;
        cout << setw(10) << c->name()
             << " (Q_we: " << setw(4) << c->m_chwilowaWydajnoscNaWejsciu << " t/h) ";
        (*it)->printMaterialDistribution(100); // Wyświetl odcinkami po 100 [m]
        cout << " (Q_wy: " << setw(4) << c->m_chwilowaWydajnoscNaWyjsciu << " t/h)\n";
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
    //vector<Conveyor *> conveyors = loadFromFile<Conveyor>("przenosniki.txt");
    vector<Conveyor *> conveyors = loadFromFile<Conveyor>("conveyors_test1.txt");
    cout << conveyors.size() << "\n";

    //-------------------------------------------------------------------------
    cout << "Loading Tanks ... ";
    //vector<Tank *> tanks = loadFromFile<Tank>("zbiorniki.txt");
    vector<Tank *> tanks = loadFromFile<Tank>("tanks_test1.txt");
    cout << tanks.size() << "\n";

    //-------------------------------------------------------------------------
    cout << "Loading Weighing Belts ... ";
    vector<WeighingBelt *> wbelts = loadFromFile<WeighingBelt>("wagi.txt");;
    cout << wbelts.size() << "\n";

    //-------------------------------------------------------------------------
    cout << "Loading Loading Grids ... ";
    vector<LoadingGrid *> grids;
    grids.push_back(new LoadingGrid("kr.txt"));
    //grids.push_back(new LoadingGrid("kr.txt"));
    //grids.push_back(new LoadingGrid("kr.txt"));
    //grids.push_back(new LoadingGrid("kr.txt"));
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

    // Wczytanie konfiguracji systemu transportowego
    //ts.loadConnectionsFromFile("connections.txt");
    ts.loadConnectionsFromFile("connections_test1.xml");

    pressAnyKey();

    // Czas jednego kroku symulacji [s]
    unsigned long dt = 4;
    dtss::Simulator<IO_type> sim(&ts, dt);

    // Obserwator symulacji
    Conveyor *p1 = findByName<Conveyor *>(conveyors, "P-1");
    WeighingBelt wagaP1(p1, "p1.dat");
    sim.addEventListener(&wagaP1);

    Conveyor *p2 = findByName<Conveyor *>(conveyors, "P-2");
    p2->addInput(500); // Wejście na 500 [m]
    WeighingBelt wagaP2(p2, "p2.dat");
    sim.addEventListener(&wagaP2);

    Conveyor *p3 = findByName<Conveyor *>(conveyors, "P-3");
    p3->addInput(400); // Wejście na 400 [m]
    WeighingBelt wagaP3(p3, "p3.dat");
    sim.addEventListener(&wagaP3);

    Conveyor *p4 = findByName<Conveyor *>(conveyors, "P-4");
    WeighingBelt wagaP4(p4, "p4.dat");
    sim.addEventListener(&wagaP4);

    Conveyor *p5 = findByName<Conveyor *>(conveyors, "P-5");
    WeighingBelt wagaP5(p5, "p5.dat");
    sim.addEventListener(&wagaP5);

    // Ilość kroków
    unsigned int max_step = 1800 / dt;
    // Pętla symulacji
    for (unsigned int step = 0; step < max_step; ++step) {
        // Pobierz materiał z kraty
        //if (step < 100)
        double mass = grids[0]->getNextValue(step * dt);
        // Treść zdarzenia: materiał na wejście nr 1 przenośnika
        IO_type material1(1, Material::build(mass, RUDNA_WEGLANOWA));
        IO_type material2(2, Material::build(mass, RUDNA_WEGLANOWA));
        IO_type material3(1, Material::build(mass, RUDNA_PIASKOWCOWA));
        IO_type material3_2(2, Material::build(mass, RUDNA_PIASKOWCOWA));

        // Zbiór zdarzeń wejściowych
        set<dtss::Event<IO_type> > in;
        // Jedno zdarzenie: przyjście materiału na przenośnik
        in.insert(dtss::Event<IO_type>(p1, material1));
        in.insert(dtss::Event<IO_type>(p2, material2));
        in.insert(dtss::Event<IO_type>(p3, material3));
        in.insert(dtss::Event<IO_type>(p3, material3_2));

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
            Sleep(10); // miliseconds
#else
            usleep(10000); // microseconds
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
