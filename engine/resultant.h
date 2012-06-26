#ifndef DTSS_RESULTANT_H
#define DTSS_RESULTANT_H

#include "atomic.h"
#include "network.h"
#include <set>
#include <map>

namespace dtss {


/**
 * Klasa opakowująca sieć (Network). Umożliwia operowanie siecią tak jakby
 * była modelem prostego obiektu (Atomic).
 */
template <typename X>
class Resultant : public dtss::Atomic<X>
{
public:
    Resultant(Network<X> *network);
    ~Resultant() { delete m_network; }

    void delta(const X &x, unsigned int dt);
    void outputFunction(X &y) const;

private:
    void route(const X& value, Atomic<X> *source);

    Network<X> *m_network;              ///< osłaniana sieć
    std::set<Atomic<X> *> m_c;          ///< komponenty sieci
    std::map<Atomic<X> *, X> m_input;   ///< stan na wejściach komponentów
    std::map<Atomic<X> *, X> m_output;  ///< stan na wyjściach komponentów
};

/**
 * Konstruktor. Obiekt przejmuje odpowiedzialność za usunięcie obiektu network.
 *
 * @param network   sieć która ma być osłonięta
 */
template <typename X>
Resultant<X>::Resultant(Network<X> *network)
    : Atomic<X>()
    , m_network(network)
{
    m_network->setResultant(this);
    m_network->getComponents(m_c);
}

template <typename X>
void Resultant<X>::delta(const X &x, unsigned int dt)
{
    // Wyślij input sieci do odpowiednich komponentów tej sieci
    route(x, this);

    // Update stanu dla wszystkich komponentów sieci
    for (typename std::set<Atomic<X>*>::iterator it = m_c.begin;
         it != m_c.end(); ++it)
    {
        (*it)->updateState(m_input[*it], dt);
    }
}

template <typename X>
void Resultant<X>::outputFunction(X &y) const
{
    // Obliczenie wyjść dla wszystkich komponentów sieci
    for (typename std::set<Atomic<X>*>::iterator it = m_c.begin;
         it != m_c.end(); ++it)
    {
        X& itY = m_output[*it]; // Zapisuje stan wyjść w tabeli
        (*it)->outputFunction(&itY);

        route(itY, *it);
    }

    // Wyjście całej sieci
    X& netY = m_output[this];
    y = netY;
}

/**
 * Przepisanie wartości z wyjść komponentów sieci do wejść odpowiednich
 * komponentów.
 *
 * @param value     wartość wyjścia na koponencie source
 * @param source    koponent wyjściowy
 */
template <typename X>
void Resultant<X>::route(const X& value, Atomic<X> *source)
{
    std::set<Atomic<X>*> influenced;
    m_network->route(value, source, influenced);

    for (typename std::set<Atomic<X>*>::iterator it = influenced.begin;
         it != influenced.end(); ++it)
    {
        if ((*it) == this) {
            // Jeśli ścieżka prowadzi do wyjścia z sieci
            m_output[this] = value;
        } else {
            // Ścieżka prowadzi do wejścia innego komponentu sieci
            m_input[*it] = value;
        }
    }
}

} // namespace dtss

#endif // DTSS_RESULTANT_H
