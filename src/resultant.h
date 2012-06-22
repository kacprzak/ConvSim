#ifndef DTSS_RESULTANT_H
#define DTSS_RESULTANT_H

#include "atomic.h"

namespace dtss {

template <typename X>
class Resultant : public dtss::Atomic<X>
{
public:
    Resultant();
};

} // namespace dtss

#endif // DTSS_RESULTANT_H
