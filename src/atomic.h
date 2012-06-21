#ifndef ATOMIC_H
#define ATOMIC_H

class Atomic
{
public:
    Atomic();

    virtual void updateState(double x) = 0;
    virtual void updateOutput() = 0;
};

#endif // ATOMIC_H
