#ifndef SIGNAL_SLOT_H
#define SIGNAL_SLOT_H

#include <QString>
#include <QObject>

class SignalSlotKoan;

/**
 * Signals and slots one of the driving features of Qt.
 * They are effective, easy to use and quite understandable
 * if you dig into it's implementation. Even more concept
 * of signals and slots became so groundbraking that now
 * similar features are included to other C++ libraries (ex. boost).
 * So now lets grasp what signals and slots is.
 * If some Qt object want to notify others about something,
 * it emits signal. If some object wants to be notified about something,
 * it declares slot, to receive notification there.
 * When you connect signal and slot (using QObject::connect()),
 * you create the link (similar to what we had with callback)
 * between two objects. So now object1 want to notify about
 * some event, so it emits signal (using "emit signalName()").
 * This event travels though link that we established earlier,
 * and ends up calling object2 slot.
 * Slots actually are simple C++ methods that can be called
 * from anywhere, they just have some additional meta information
 * so signal could be connected.
 * You can connect signal to multiple slots, and slot can receive
 * multiple signals. Signal slot connection is thread save
 * (exept if you don't want it to be unsafe), as slot is always
 * called in thread to which object belongs to.
 * Read more about signals and slots http://doc.qt.io/qt-5/signalsandslots.html
 */

/**
 * @brief The Counter class
 * is an example from qt docs.
 * Every object that want to use signals
 * and slots must inherit from QObject
 * and have Q_OBJECT macro declared
 * inside.
 */
class Counter : public QObject
{
    Q_OBJECT

public:
    Counter();

    int value() const;

public slots:
    /**
     * @brief setValue go to signal_slot.cpp
     * and implement it.
     * @param value
     */
    void setValue(int value);

signals:
    void valueChanged(int newValue);

private:
    int m_value;
};

/**
 * Now go to .cpp and connect objects.
 */
void connectExample(Counter* sender, Counter* receiver);
void connectExample2(Counter* sender, Counter* receiver1, Counter* receiver2);
void connectExample3(Counter* sender1, Counter* sender2, Counter* receiver);

#endif // SIGNAL_SLOT_H
