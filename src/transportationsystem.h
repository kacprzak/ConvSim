// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
#ifndef TRANSPORTATIONSYSTEM_H
#define TRANSPORTATIONSYSTEM_H

#include "network.h"
#include "conveyor.h"
#include "tank.h"
#include <set>
//#include <map>

//------------------------------------------------------------------------------

template <typename S, typename D>
class _Connection {
 public:
    _Connection()
        : m_src(nullptr)
        , m_outNumber(0)
        , m_dest(nullptr)
        , m_inNumber(0)
    {}

    _Connection(S *src, int outNum, D *dest, int inNum)
        : m_src(src)
        , m_outNumber(outNum)
        , m_dest(dest)
        , m_inNumber(inNum)
    {}

    bool isNull() const { return m_src == nullptr; }
        
    // for STL
    bool operator==(const _Connection& other) const
    {
        return m_src == other.m_src
        && m_outNumber == other.m_outNumber
        && m_dest == other.m_dest
        && m_inNumber == other.m_inNumber;
    }
    
    bool operator<(const _Connection& other) const
    {
        return m_src < other.m_src;
    }
    
    S *source() const      { return m_src; }
    int srcOut() const     { return m_outNumber; }
    D *destination() const { return m_dest; }
    int destIn() const     { return m_inNumber; }

    S   *m_src;
    int  m_outNumber;
    D   *m_dest;
    int  m_inNumber;
};

//------------------------------------------------------------------------------

typedef _Connection<Conveyor, Conveyor> Connection;

class TransportationSystem : public dtss::Network<IO_type>
{

public:
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
    std::set<Connection> m_connections;

    const Connection findConnection(Conveyor *outConv, int outNum);
};

#endif // TRANSPORTATIONSYSTEM_H
