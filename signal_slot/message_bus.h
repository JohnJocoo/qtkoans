#ifndef MESSAGE_BUS_H
#define MESSAGE_BUS_H

#include <QString>

class SignalSlotKoan;

/**
 * Message bus is a concept where you have
 * one global carrier that receives all notifications
 * from objects all over the application and then
 * delivers them to all objects that can receive notifications.
 * It can be left to receivers to decide if they want to
 * handle this notification or not, or message bus can itself
 * decide who from receivers will receive some type of notification
 * and who not.
 * There also could be non-global implementations, allowing you to
 * have multiple message buses in one application.
 * We'll create simple global message bus that will deliver every message to
 * every receiver, as our attempt is to grasp what message bus is, and
 * not to create production quality code.
 * (Especially when Qt already has message bus implementation).
 */

/**
 * @brief The MessageBase class
 * base class for any message.
 */
class MessageBase
{
public:
    MessageBase();
    MessageBase(int type, QString message = QString());
    virtual ~MessageBase();

    /**
     * @brief type of message
     * @return
     */
    int type() const;
    /**
     * @brief message some common information to send
     * @return
     */
    QString message() const;

private:
    int m_type;
    QString m_message;
};

/**
 * @brief sendMessage function that sends message through
 * message bus.
 * @param message to send. Note that you don't need
 * to delete message later, as it will be deleted
 * by message bus when delivered.
 */
void sendMessage(MessageBase* message);

/**
 * @brief The MessageReceiver class
 * is a base class for all objects that
 * want to receive bus messages.
 * It's automatically gets registered
 * in a bus and removed from the bus
 * then deleted.
 */
class MessageReceiver
{
public:
    /**
     * @brief MessageReceiver in constructor we
     * register instance of receiver class in message bus
     * so it can receive messages later.
     * Go to message_bus.cpp and implement it.
     */
    MessageReceiver();

    /**
     * @brief ~MessageReceiver in destructor we
     * remove instance of receiver class from message bug
     * so it wouldn't be any hanging pointers left.
     */
    virtual ~MessageReceiver();

    /**
     * @brief messageReceived every class that want to
     * receive callbacks need to inherit from MessageReceiver
     * and implement this method.
     * @param message
     */
    virtual void messageReceived(MessageBase* message) = 0;
};

/**
 * @brief The SimpleMessageReceiver class
 * now we create a simple message receiver
 * that will receive message and store a string
 * from it to m_data member.
 * Also note that while we inherit from MessageReceiver
 * and implement messageReceived(MessageBase*) method,
 * we don't write any code that register this object
 * in message bus or removes it. All this work is already
 * done in MessageReceiver class.
 */
class SimpleMessageReceiver : public MessageReceiver
{
public:
    /**
     * @brief messageReceived go to .cpp file
     * and store string from message.
     * @param message
     */
    void messageReceived(MessageBase* message);

private:
    QString m_data;

    friend class SignalSlotKoan;
};

/**
 * @brief sendHelloMessage here you
 * have to send simple message to the bus.
 * Go to the .cpp.
 */
void sendHelloMessage();

/**
 * At this stage SignalSlotKoan::busSimple test showld pass.
 * For now you see that all you need is to inherit from
 * MessageReceiver, implement void messageReceived(MessageBase*),
 * and you will receive all messages sent anywhere in application.
 * Even more if you send message from another thread,
 * you'll always receive message in main thread, because
 * message bus is synchronized.
 * Now lets see how things go if we want different receivers receive
 * different type of messages.
 */

/**
 * @brief The MessageType enum defines
 * type of message we received.
 * Because we always receive pointer to MessageBase
 * in real world we would like first to check
 * if message->type() is the type we want.
 */
enum MessageType
{
    Undefined = 0,
    /**
     * We received MessageBase with
     * message->message() being user input.
     */
    UserInput,
    /**
     * We received MouseClickMessage,
     * we need to cast our MessageBase* message
     * to MouseClickMessage* first.
     */
    MouseClick
};

/**
 * @brief The MouseClickMessage class
 * is a class that describes where mouse
 * click happened.
 */
class MouseClickMessage : public MessageBase
{
public:
    MouseClickMessage();
    MouseClickMessage(int x, int y, int button);
    ~MouseClickMessage();

    int x() const;
    int y() const;
    int button() const;

private:
    int m_x;
    int m_y;
    int m_button;
};

/**
 * @brief The UserInputReceiver class
 * should handle only messages with type == UserInput.
 */
class UserInputReceiver : public MessageReceiver
{
public:
    /**
     * @brief messageReceived you must filter
     * out message of UserInput type and store a string from it.
     * Go to .cpp.
     * @param message
     */
    void messageReceived(MessageBase* message);

private:
    QString m_data;

    friend class SignalSlotKoan;
};

/**
 * @brief The MouseClickReceiver class
 * should handle only messages with type == MouseClick.
 */
class MouseClickReceiver : public MessageReceiver
{
public:
    MouseClickReceiver();
    /**
     * @brief messageReceived you must filter
     * out message of MouseClick type and store x and y from it,
     * if button == 1.
     * Go to .cpp.
     * @param message
     */
    void messageReceived(MessageBase* message);

private:
    int m_x;
    int m_y;

    friend class SignalSlotKoan;
};

/**
 * So now SignalSlotKoan::busDifferent test should pass.
 * As I said before there could be many message bus
 * implementation, but all of them suffer from the same problems:
 * 1 - you can't easily send message from one object to
 * another concrete object, some other objects will receive
 * it as well.
 * 2 - you should create new Message type class if you want
 * to transfer some new type of data, and you have to cast
 * to it from basic class (MessageBase*) in receiver.
 *
 * Now answear following questions and then go to signal_slot.h:
 * 1 - How many new Message classes you have to create if you
 * have 10 different kinds of notifications? What should you
 * do in every receiver class to ensure you don't process
 * wrong message?
 * 2 - Suppose message bus is implemented as synchronized
 * message queue and a thread that delivers messages from queue
 * to receivers. Compare now what is more effective -
 * callback or message bus?
 * 3 - Summarize all pros and cons of using message bus.
 */

#endif // MESSAGE_BUS_H
