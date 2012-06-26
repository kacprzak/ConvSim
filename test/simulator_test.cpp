#include <cstdlib>

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

    Conveyor conv;
    Simulator<double> sim(&conv);
    WeighingBelt waga;
    sim.addEventListener(&waga);

    double input[] = {0.1, 0.2, 0.1, 0.3, 0.1, 0.5, 0.05};
    unsigned int steps = sizeof(input)/sizeof(double);

    for (unsigned int n = 0; n < steps; ++n) {
        sim.computeNextState(input[n]);
        sim.computeOutput();
    }

    if (sim.getTime() != steps)
        return EXIT_FAILURE;
    else
        return EXIT_SUCCESS;
}