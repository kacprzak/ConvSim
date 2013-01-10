#ifndef DTSS_EVENTLISTENER_H
#define DTSS_EVENTLISTENER_H

#include "atomic.h"
#include "event.h"

namespace dtss {

/**
 * Klasy obiektów obserwujących przebieg symulacji powinny dziedziczyć po
 * tej klasie (po tym interfejsie).
 */
template <class T>
class EventListener
{
public:
    /**
     * Wywoływana przez symulator w razie zmiany stanu symulowanego modelu.
     *
     * @param model     model który zmienił stan
     * @param t         czas w którym nastąpiło zdarzenie
     */
    virtual void stateChanged(Atomic<T> *model, unsigned int t) = 0;

    /**
     * Wywoływana przez symulator.
     *
     * @param e         zdarzenie
     * @param t         czas w którym nastąpiło zdarzenie
     */
    virtual void outputEvent(const Event<T>& e, unsigned int t) = 0;
};

} // namespace dtss

#endif // DTSS_EVENTLISTENER_H
