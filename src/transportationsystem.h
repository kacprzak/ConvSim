#ifndef TRANSPORTATIONSYSTEM_H
#define TRANSPORTATIONSYSTEM_H

#include "network.h"
#include "conveyor.h"
#include <set>
//#include <map>

class TransportationSystem : public dtss::Network<IO_type>
{
    struct Connection {
        Conveyor *outConv;
        int outputNumber;
        Conveyor *inConv;
        int inputNumber;

        Connection() { outConv = 0; }
        bool isNull() { return outConv == 0; }

        // for STL
        bool operator<(const Connection& other) const
        {
            return outConv < other.outConv;
        }
    };

public:
    void getComponents(std::set<dtss::Model<IO_type>*>& c) const;
    void route(const IO_type& value, dtss::Model<IO_type> *source,
               std::set<dtss::Event<IO_type> >& r);

    void addConveyor(Conveyor *conv);
    void connect(Conveyor *outConv, Conveyor *inConv);
    void connect(Conveyor *outConv, int outNum, Conveyor *inConv, int inNum);

private:
    std::set<Conveyor *> m_conveyors;
    // TODO: Zmienić na multi_map
    std::set<Connection> m_connections;

    Connection findConnection(Conveyor *outConv, int outNum);
};

#endif // TRANSPORTATIONSYSTEM_H
