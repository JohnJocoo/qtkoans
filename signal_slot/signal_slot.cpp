#include "signal_slot.h"

void Counter::setValue(int value)
{
    /**
     * Set m_value and if m_value != value
     * emit valueChanged signal.
     */
}

Counter::Counter()
    : m_value(0)
{}

int Counter::value() const
{
    return m_value;
}

void connectExample(Counter* sender, Counter* receiver)
{
    /**
     * Connect sender's signal to receiver's slot.
     * Use QObject::connect().
     */
}

void connectExample2(Counter* sender, Counter* receiver1, Counter* receiver2)
{
    /**
     * Connect sender's signal to receiver1 and reciver2 slot.
     * Use QObject::connect().
     * Use Qt::DirectConnection flag now (you can read about it in qt docs).
     */
}

void connectExample3(Counter* sender1, Counter* sender2, Counter* receiver)
{
    /**
     * Connect sender1 and sender2 signal to receiver's slot.
     * Use QObject::connect().
     */
}

