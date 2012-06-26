#ifndef DTSS_MODEL_H
#define DTSS_MODEL_H

namespace dtss {

// forward declaration
template <typename T> class Atomic;
template <typename T> class Network;

/**
 * Klasa bazowa dla klas Atomic oraz Network.
 */
template <typename T>
class Model
{
public:
    virtual ~Model() {}

    virtual Atomic<T> *typeIsAtomic() { return 0; }
    virtual Network<T> *typeIsNetwork() { return 0; }

    void setParent(Network<T> *parent) { m_parent = parent; }
    Network<T> *parent() const { return m_parent; }

protected:
    Model() {}
    //Model(const Model<T>& other);              // non construction-copyable
private:
    //Model& operator=(const Model<T>& other);   // non copyable

    Network<T> *m_parent; ///< Sieć do której należy model
};

} // namespace dtss

#endif // DTSS_MODEL_H
