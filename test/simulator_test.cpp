#include <cstdlib>

#include "event.h"
#include "conveyor.h"
#include "weighingbelt.h"
#include "tank.h"
#include "loadinggrid.h"
#include "simulator.h"

/**
 * Test działania symulatora.
 */
int main()
{
    using namespace dtss;

    Conveyor conv("C-1", 10, 2.5, 600);
    Simulator<double> sim(&conv);
    WeighingBelt waga;
    sim.addEventListener(&waga);

    double input[] = {0.1, 0.2, 0.1, 0.3, 0.1, 0.5, 0.05};
    unsigned int steps = sizeof(input)/sizeof(double);

    for (unsigned int n = 0; n < steps; ++n) {
        std::set<Event<double> > in;
        in.insert(Event<double>(&conv, input[n]));

        sim.computeNextState(in);
        sim.computeOutput();
    }

    if (sim.getTime() != steps)
        return EXIT_FAILURE;
    else
        return EXIT_SUCCESS;
}
