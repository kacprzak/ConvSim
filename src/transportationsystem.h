// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
#ifndef TRANSPORTATIONSYSTEM_H
#define TRANSPORTATIONSYSTEM_H

#include "connection.h"
#include "network.h"
#include "conveyor.h"
#include "tank.h"
#include <set>
//#include <map>

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
    void addTank(Tank *tank);

    void connect(dtss::Model<IO_type> *src, dtss::Model<IO_type> *dest);
    void connect(dtss::Model<IO_type> *src, int outNum,
                 dtss::Model<IO_type> *dest, int inNum);

    bool loadConnectionsFromFile(const std::string& filename);

private:
    std::set<Conveyor *> m_conveyors;
    std::set<Tank *>     m_tanks;

    // TODO: Zmienić na multi_map
    std::set<TSConnection *> m_connections;

    const TSConnection *findConnection(dtss::Model<IO_type> *src, int outNum);
};

#endif // TRANSPORTATIONSYSTEM_H
