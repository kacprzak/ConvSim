#ifndef DTSS_NETWORK_H
#define DTSS_NETWORK_H

#include "model.h"
#include "event.h"
#include <set>

namespace dtss {

template <typename T>
class Network : public Model<T>
{
public:
    virtual void getComponents(std::set<Model<T>*>& c) const = 0;

    virtual void route(const T& value, Model<T> *source,
                       std::set<Event<T> >& r) = 0;

    Network<T> *typeIsNetwork() { return this; }

};

} // namespace dtss

#endif // DTSS_NETWORK_H
