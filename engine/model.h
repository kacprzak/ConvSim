#ifndef DTSS_MODEL_H
#define DTSS_MODEL_H

namespace dtss {

// forward declaration
template <typename T> class Atomic;
template <typename T> class Network;

/**
 * Klasa bazowa dla klas Atomic oraz Network. Przy tworzeniu własnych modeli
 * należy skorzystać z klas Atomic lub Network. Dziedziczenie po tej klasie
 * nie ma uzasadnienia.
 */
template <typename T>
class Model
{
public:
    virtual ~Model() {}

    virtual Atomic<T> *typeIsAtomic() { return 0; }
    virtual Network<T> *typeIsNetwork() { return 0; }

    /**
     * Przypisuje model do sieci.
     */
    void setParent(Network<T> *parent) { m_parent = parent; }

    /**
     * Zwraca sieć do której należy model.
     */
    Network<T> *parent() const { return m_parent; }

protected:
    Model() : m_parent(0) {}
    //Model(const Model<T>& other);              // non construction-copyable
private:
    //Model& operator=(const Model<T>& other);   // non copyable

    Network<T> *m_parent; ///< Sieć do której należy model
};

} // namespace dtss

#endif // DTSS_MODEL_H
