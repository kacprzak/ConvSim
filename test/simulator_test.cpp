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

    Conveyor conv("C-1", 5, 2.5, 600);
    Simulator<IO_type> sim(&conv);
    WeighingBelt waga;
    sim.addEventListener(&waga);

    double input[] = {0.1, 0.2, 0.1, 0.3, 0.1, 0.5, 0.05, 0, 0, 0, 0};
    unsigned int steps = sizeof(input)/sizeof(double);

    for (unsigned int n = 0; n < steps; ++n) {
        std::set<Event<IO_type> > in;
        in.insert(Event<IO_type>(&conv, IO_type(1, Material::build(input[n], RUDNA_WEGLANOWA))));

        sim.computeNextState(in);
        sim.computeOutput();
    }

    if (sim.getTime() != steps)
        return EXIT_FAILURE;
    else
        return EXIT_SUCCESS;
}
