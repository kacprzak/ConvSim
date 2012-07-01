#ifndef TRANSPORTATIONSYSTEM_H
#define TRANSPORTATIONSYSTEM_H

#include "network.h"
#include "conveyor.h"
#include <set>
#include <map>

class TransportationSystem : public dtss::Network<double>
{
public:
    void getComponents(std::set<dtss::Model<double>*>& c) const;
    void route(const double& value, dtss::Model<double> *source, std::set<dtss::Event<double> >& r);

    void addConveyor(Conveyor *conv);
    void connect(Conveyor *out, Conveyor *in);

private:
    std::set<Conveyor *> m_conveyors;
    // TODO: Zmienić na multi_map
    std::map<Conveyor *, Conveyor *> m_routes;
};

#endif // TRANSPORTATIONSYSTEM_H
