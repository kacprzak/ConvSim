#ifndef DTSS_EVENTLISTENER_H
#define DTSS_EVENTLISTENER_H

#include "atomic.h"
#include "event.h"

namespace dtss {

template <class T>
class EventListener
{
public:
    EventListener();

    /**
     * Wywoływana przez symulator w razie zmiany stanu symulowanego modelu.
     *
     * @param model     model który zmienił stan
     * @param t         czas w którym nastąpiło zdarzenie
     */
    virtual void stateChanged(Atomic<T> */*model*/, unsigned int /*t*/) {}

    /**
     * Wywoływana przez symulator.
     *
     * @param e         zdarzenie
     * @param t         czas w którym nastąpiło zdarzenie
     */
    virtual void outputEvent(const Event<T>& /*e*/, unsigned int /*t*/) {}
};

template <class T>
EventListener<T>::EventListener()
{
}

} // namespace dtss

#endif // DTSS_EVENTLISTENER_H
