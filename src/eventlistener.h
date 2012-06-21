#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

#include "atomic.h"

class EventListener
{
public:
    EventListener();

    virtual void stateChanged(Atomic *model, unsigned int t) {}
    virtual void outputEvent(Atomic *model, double output, unsigned int t) {}
};

#endif // EVENTLISTENER_H
