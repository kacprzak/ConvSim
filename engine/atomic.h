#ifndef DTSS_ATOMIC_H
#define DTSS_ATOMIC_H

#include "model.h"

#include <set>

namespace dtss {

// forward declaration
template <typename T> class Simulator;

/**
 * Klasa bazowa dla dla modeli obiektów niepodzielnych.
 */
template <typename T>
class Atomic : public Model<T>
{
    template <typename TT> friend class Simulator;

public:
    virtual void delta(const std::set<T>& xb) = 0;
    virtual void outputFunction(std::set<T>& yb) const = 0;

    virtual void gcOutput(std::set<T>& /*gb*/) {}

    Atomic<T> *typeIsAtomic() { return this; }

private:
    std::set<T> m_input;
    std::set<T> m_output;
};

} // namespace dtss
#endif // DTSS_ATOMIC_H
