#include <cstdlib>

#include "event.h"
#include "conveyor.h"
#include "weighingbelt.h"
#include "tank.h"
#include "loadinggrid.h"
#include "transportationsystem.h"
#include "simulator.h"

/**
 * Test działania systemu transportowego.
 */
int main()
{
    using namespace dtss;

    // Utworzenie dwóch przenośników
    Conveyor conv1("C-1", 5, 2.5, 600);
    Conveyor conv2("C-2", 25, 3, 600);
    // System transportowy
    TransportationSystem ts;
    // Zarejestrowanie przenośników w systemie
    ts.addConveyor(&conv1);
    ts.addConveyor(&conv2);

    std::set<dtss::Model<IO_type>*> components;
    ts.getComponents(components);
    if (components.size() != 2)
        return EXIT_FAILURE;

    // Połączenie wyjścia C-1 z wejściem C-2
    ts.connect(&conv1, &conv2);

    Simulator<IO_type> sim(&ts);
    //WeighingBelt waga;
    //sim.addEventListener(&waga);

    double input[] = {0.2, 0.2, 0.0, 0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0};
    unsigned int steps = sizeof(input)/sizeof(double);

    for (unsigned int n = 0; n < steps; ++n) {
        std::set<Event<IO_type> > in;
        in.insert(Event<IO_type>(&conv1, IO_type(1, input[n])));

        sim.computeNextState(in);
        std::cout << conv1.materialAmount() << " " << conv2.materialAmount() << std::endl;
    }

    if (sim.getTime() != steps)
        return EXIT_FAILURE;
    else
        return EXIT_SUCCESS;
}
