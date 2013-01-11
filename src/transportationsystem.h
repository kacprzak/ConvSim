// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
#ifndef TRANSPORTATIONSYSTEM_H
#define TRANSPORTATIONSYSTEM_H

#include "connection.h"
#include "network.h"
#include "conveyor.h"
#include "tank.h"
#include <set>
//#include <map>

//------------------------------------------------------------------------------

class TransportationSystem : public dtss::Network<IO_type>
{

public:
    ~TransportationSystem();

    // dtss::Network interface
    void getComponents(std::set<dtss::Model<IO_type>*>& c) const;
    void route(const IO_type& value, dtss::Model<IO_type> *source,
               std::set<dtss::Event<IO_type> >& r);
    // end dtss::Network interface

    void addConveyor(Conveyor *conv);
    void connect(Conveyor *outConv, Conveyor *inConv);
    void connect(Conveyor *outConv, int outNum, Conveyor *inConv, int inNum);

private:
    std::set<Conveyor *> m_conveyors;
    // TODO: Zmienić na multi_map
    std::set<Connection *> m_connections;

    const ConvConvConnection *findConnection(Conveyor *outConv, int outNum);
};

#endif // TRANSPORTATIONSYSTEM_H
