#ifndef DTSS_RESULTANT_H
#define DTSS_RESULTANT_H

#include "atomic.h"
#include "network.h"
#include <set>
#include <map>

namespace dtss {

template <typename X>
class Resultant : public dtss::Atomic<X>
{
public:
    Resultant(Network<X> *model);
    ~Resultant() { delete m_model; }

    void updateState(const X &x, unsigned int dt);
    void outputFunction(X &y) const;

private:
    void route(const X& value, Atomic<X> *source);

    Network<X> *m_model;
    std::set<Atomic<X> *> m_c;
    std::map<Atomic<X> *, X> m_input;
    std::map<Atomic<X> *, X> m_output;
};

template <typename X>
Resultant<X>::Resultant(Network<X> *model)
    : Atomic<X>()
    , m_model(model)
{
    m_model->setResultant(this);
    m_model->getComponents(m_c);
}

template <typename X>
void Resultant<X>::updateState(const X &x, unsigned int dt)
{
    // Wyślij input do odpowiednich komponentów
    route(x, this);

    // Update stanu dla wszystkich komponentów
    for (typename std::set<Atomic<X>*>::iterator it = m_c.begin;
         it != m_c.end(); ++it)
    {
        (*it)->updateState(x, dt);
    }
}

template <typename X>
void Resultant<X>::outputFunction(X &y) const
{
    // Obliczenie wyjść dla wszystkich komponentów
    for (typename std::set<Atomic<X>*>::iterator it = m_c.begin;
         it != m_c.end(); ++it)
    {
        X& ity = m_output[*it];
        (*it)->outputFunction(&ity);

        route(ity, *it);
    }

    // Wyjście całej sieci
    X& ny = m_output[this];
    y = ny;
}

template <typename X>
void Resultant<X>::route(const X& value, Atomic<X> *source)
{
    // TODO
}

} // namespace dtss

#endif // DTSS_RESULTANT_H
