#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <QString>

class SignalSlotKoan;

/**
 * At first before starting with Qt's signal/slot
 * system we'll talk about other communication mechanisms
 * in programming. We'll discuss callbacks and message bus
 * to grasp a common understanding of how you can make
 * different parts of your programm talk to each other.
 * I hope that it'll help you to understang signal/slot
 * mechanism later.
 *
 * So heading on to callbacks.
 * Callback might be the simpliest mechanism for understanding
 * (though simpliest doesn't mean easiest to use).
 * Suppose you have object A that takes an user input,
 * and object B that must be notified than there is an
 * input avaliable. So you set some function from B to
 * A as callback. So you have:
 * 1 - user input data to A
 * 2 - A than sends it with callback
 * 3 - B receives it
 * Thats pretty much all theory, so now you need to
 * implement a callback in C++. We'll use object oriented
 * programming to declare an abstract class with callback
 * function.
 */

/**
 * @brief The Callback class is our callback.
 * Every class that want to receive input data
 * must inherit from "Callback" class,
 * and every class that can provide input data
 * must have a possibility to set and store reference
 * to instance of receiver. In our case we'll have
 * setCallback function and store pointer to receiver
 * as class member.
 */
class Callback
{
public:
    virtual ~Callback();
    virtual void inputReady(QString data) = 0;
};

/**
 * @brief The InputDataReceiver class
 * will receive input data
 * (if it was registered as callback ofc).
 * Follow directions in comment to
 * methods to implement this koan.
 */
class InputDataReceiver : public Callback
{
public:
    /**
     * @brief inputReady implement it to
     * set m_data memeber to data parameter
     * in callbacks.cpp
     * @param data
     */
    void inputReady(QString data);

private:
    QString m_data;

    friend class SignalSlotKoan;
};

/**
 * @brief The InputDataProvider class
 * is a class that sends input data to
 * callback.
 */
class InputDataProvider
{
public:
    InputDataProvider();

    /**
     * @brief setCallback remember callback
     * as m_callback member.
     * @param callback
     */
    void setCallback(Callback* callback);
    /**
     * @brief updateData suppose our
     * class get name and surname
     * from user and concartinates them
     * with space between.
     * Ex. "John", "Smith" -> "John Smith".
     * Implement concaternation, store
     * result to m_data member, call callback
     * if it's not NULL.
     * @param name
     * @param surname
     */
    void updateData(QString name, QString surname);

private:
    QString m_data;
    Callback* m_callback;

    friend class SignalSlotKoan;
};

/**
 * So for now everything works fine,
 * and SignalSlotKoan::basicCallback
 * and SignalSlotKoan::callCallback tests should pass.
 * But where are some pitfalls with using callback
 * and I'll tell about two of the most common.
 * Pitfall #1 is deletion of receiver and provider.
 * If provider gets deleted, probably nothing bad will happen now,
 * but if you delete receiver, boom - you get hanging pointer,
 * and this is one of the things programmers fear most!
 * Pointers to deleted objects are a nightmare to debug.
 * So lets fix our example.
 */

/**
 * @brief The ProviderInterface class
 * declares functions to set and remove callback,
 * so we can be safe when receiver is deleted before
 * provider.
 */
class ProviderInterface
{
public:
    virtual void setCallback(Callback* callback) = 0;
    virtual void removeCallback(Callback* callback) = 0;
};

/**
 * @brief The InputDataProvider2 class
 * is a class that sends input data to
 * callback, and also tracks if callback
 * deleted.
 * Implement this class now.
 */
class InputDataProvider2 : public ProviderInterface
{
public:
    InputDataProvider2();

    /**
     * @brief setCallback remember callback
     * as m_callback member.
     * @param callback
     */
    void setCallback(Callback* callback);
    /**
     * @brief removeCallback remove callback.
     * @param callback
     */
    void removeCallback(Callback* callback);
    /**
     * @brief updateData suppose our
     * class get name and surname
     * from user and concartinates them
     * with space between.
     * Ex. "John", "Smith" -> "John Smith".
     * Implement concaternation, store
     * result to m_data member, call callback
     * if it's not NULL.
     * @param name
     * @param surname
     */
    void updateData(QString name, QString surname);

private:
    QString m_data;
    Callback* m_callback;

    friend class SignalSlotKoan;
};

/**
 * @brief The InputDataReceiver2 class
 * will receive input data from receiver
 * which was registered to it.
 * Follow directions in comment to
 * methods to implement this koan.
 */
class InputDataReceiver2 : public Callback
{
public:
    InputDataReceiver2();

    /**
     * Here you'll need to notify provider
     * that object is being deleted.
     */
    ~InputDataReceiver2();

    /**
     * @brief inputReady implement it to
     * set m_data memeber to data parameter
     * in callbacks.cpp
     * @param data
     */
    void inputReady(QString data);
    /**
     * @brief setProvider register data provider.
     * Here you need to store pointer to provider
     * to notify it then object is deleted.
     * Then you'll need to register InputDataReceiver2
     * instance as callback in provider to receive data.
     * @param provider
     */
    void setProvider(ProviderInterface* provider);

private:
    QString m_data;
    ProviderInterface* m_provider;

    friend class SignalSlotKoan;
};

/**
 * We fixed deletion of callback when receiver gets deleted,
 * so now SignalSlotKoan::deleteCallback should pass.
 * Though you can notice that now we have the same problem
 * with deletion of provider, we can fix it the same way
 * as with receiver, so we'll have cross reference for deletion
 * notification. The concept that was simple at the beginning
 * now became pretty complicated. Of course monitoring of
 * callback lifetime is usually handeled by framework, but still
 * you'll probably need to keep that in mind.
 *
 * Pitfall #2 is threading. Developing for real world, you'll
 * face objects living in different threads and communicating
 * with each other (Ex. worker in background thread need to receive data
 * to process from main thread, and then work is done send result
 * back to main thread). Callback in such a situation can introduce
 * synchronization problems to your code. We wouldn't discuss strategies
 * to solve such problems, as it is out of scope of this koan, just
 * mention that any solution that leads to programmer using
 * synchronization primitives isn't a good solution, as programmer's
 * task in this case is just to send data, receive it in other place and
 * process it. Sadly when using direct callbacks (like in this task),
 * you'll have to use synchronization primitives.
 *
 * Now answear following questions and then go to message_bus.h:
 * 1 - How can you modify provider, so multiple receivers
 * could receive data?
 * 2 - How can you modify receiver, so it could receive data
 * from multiple providers?
 * 3 - What should you do if your receiver want to receive different
 * kind of notifications (each kind has it's own callback class)?
 * Count the number of classes you need to inherit from if your class
 * want to receive 10 different kinds of notifications.
 * 4 - Assuming all above summarize all pros and cons of
 * using callbacks.
 */

#endif // CALLBACKS_H
