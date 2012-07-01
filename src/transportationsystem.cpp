#include "transportationsystem.h"

void TransportationSystem::getComponents(std::set<dtss::Model<double>*>& c) const
{
    c.insert(m_conveyors.begin(), m_conveyors.end());
}

void TransportationSystem::route(const double& value, dtss::Model<double> *source, std::set<dtss::Event<double> >& r)
{
    // Źródło
    Conveyor *outConv = static_cast<Conveyor *>(source);
    // Model docelowy
    if (m_routes.find(outConv) != m_routes.end()) {
        Conveyor *inConv = m_routes[outConv];
        dtss::Event<double> e(inConv, value);
        r.insert(e);
    }
}

void TransportationSystem::addConveyor(Conveyor *conv)
{
    m_conveyors.insert(conv);
    conv->setParent(this);
}

void TransportationSystem::connect(Conveyor *out, Conveyor *in)
{
    m_routes[out] = in;
}
