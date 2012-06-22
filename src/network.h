#ifndef DTSS_NETWORK_H
#define DTSS_NETWORK_H

#include "atomic.h"
#include <set>

namespace dtss {

template <typename X>
class Network
{
public:
    Network() : m_resultant(0) {}

    void setResultant(Atomic<X> *resultant) { m_resultant = resultant; }
    Atomic<X> *resultant() const { return m_resultant; }

    virtual void getComponents(std::set<Atomic<X>*>& c) const;
    // TODO: Pomyśleć nad trzecim parametrem
    virtual void route(const X& value, Atomic<X> *model, X& r);

private:
    Atomic<X> *m_resultant;

};

} // namespace dtss

#endif // DTSS_NETWORK_H
