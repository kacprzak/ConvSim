// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
#ifndef CONNECTION_H
#define CONNECTION_H

#include "conveyor.h"
#include "tank.h"

/** Rodzaje połączeń */
enum ConnType { CONV_CONV, CONV_TANK, TANK_CONV, MODEL_MODEL };

/**
 * Połączenia pomiędzy przenośnikami i zbiornikami w systemie
 * transportowym.
 */
class Connection {
 public:
    virtual ~Connection() {}

    virtual ConnType connType() const = 0;
};

template <typename S, typename D>
class GenericConnection : public Connection {
 public:
    GenericConnection(S *src, int outNum, D *dest, int inNum)
        : m_src(src)
        , m_outNumber(outNum)
        , m_dest(dest)
        , m_inNumber(inNum)
    {}

    // for STL
    bool operator==(const GenericConnection& other) const
    {
        return m_src == other.m_src
        && m_outNumber == other.m_outNumber
        && m_dest == other.m_dest
        && m_inNumber == other.m_inNumber;
    }
    
    bool operator<(const GenericConnection& other) const
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

class ConvConvConnection : public GenericConnection<Conveyor, Conveyor>
{
 public:
    ConvConvConnection(Conveyor *src, int outNum, Conveyor *dest, int inNum)
        : GenericConnection(src, outNum, dest, inNum)
    {}


    ConnType connType() const { return CONV_CONV; }
};

#endif // CONNECTION_H
