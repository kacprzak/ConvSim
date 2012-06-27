#ifndef DTSS_NETWORK_H
#define DTSS_NETWORK_H

#include "model.h"
#include "event.h"
#include <set>

namespace dtss {

/**
 * Klasa bazowa dla sieci.
 * Sieć agreguje modele typu atomic oraz inne sieci. Sieć umożliwia
 * odczytanie połączeń jakie występują pomiędzy modelami które są
 * koponentami tej sieci. Dzięki temu symulator wie jak połączyć wyjścia
 * z wejściami.
 */
template <typename T>
class Network : public Model<T>
{
public:

    /**
     * Dodaje do zbioru c komponenty sieci.
     */
    virtual void getComponents(std::set<Model<T>*>& c) const = 0;

    /**
     * Na podstawie wartości na wyjściu jednego z modeli należących do sieci
     * generuje zdarzenie wejściowe do innego modelu.
     *
     * @param value     wartość na wyjściu modelu source
     * @param source    model który zmienił wartość na wyjściu
     * @param r         zbiór do którego zostaną dodane zdarzenia wejściowe
     *                  spowodowane wartością value na obiekcie source
     */
    virtual void route(const T& value, Model<T> *source,
                       std::set<Event<T> >& r) = 0;

    Network<T> *typeIsNetwork() { return this; }

};

} // namespace dtss

#endif // DTSS_NETWORK_H
