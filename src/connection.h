// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
#ifndef CONNECTION_H
#define CONNECTION_H

#include "conveyor.h"
#include "tank.h"

/** Rodzaje połączeń */
enum ConnType { CONV_CONV, CONV_TANK, TANK_CONV, MODEL_MODEL };

/**
 * Połączenia pomiędzy modelami w systemie transportowym.
 */
template <typename S, typename D>
class Connection {
 public:
    Connection(S *src, int outNum, D *dest, int inNum)
        : m_src(src)
        , m_outNumber(outNum)
        , m_dest(dest)
        , m_inNumber(inNum)
    {}

    virtual ~Connection() {}

    virtual ConnType connType() const = 0;

    // for STL
    bool operator==(const Connection& other) const
    {
        return m_src == other.m_src
        && m_outNumber == other.m_outNumber
        && m_dest == other.m_dest
        && m_inNumber == other.m_inNumber;
    }
    
    bool operator<(const Connection& other) const
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

class TSConnection : public Connection<dtss::Model<IO_type>, dtss::Model<IO_type> >
{
 public:
    TSConnection(dtss::Model<IO_type> *src, int outNum, dtss::Model<IO_type> *dest, int inNum)
        : Connection(src, outNum, dest, inNum)
    {}

    ConnType connType() const { return MODEL_MODEL; }
};

//------------------------------------------------------------------------------

#endif // CONNECTION_H
