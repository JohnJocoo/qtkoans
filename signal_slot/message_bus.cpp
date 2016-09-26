#include "message_bus.h"

#include <vector>
#include <mutex>

static void registerMessageReceiver(MessageReceiver* receiver);
static void removeMessageReceiver(MessageReceiver* receiver);

MessageReceiver::MessageReceiver()
{
    /**
     * Call registerMessageReceiver(MessageReceiver*)
     * for this object to receive messages later.
     */
}

MessageReceiver::~MessageReceiver()
{
    /**
     * Call removeMessageReceiver(MessageReceiver*)
     * for this object to notify message bus about deletion.
     */
}

void SimpleMessageReceiver::messageReceived(MessageBase* message)
{
    /**
     * Save message->message() to m_data.
     * Note that you don't have to delete message,
     * as it will be automatically deleted by bus.
     */
}

void sendHelloMessage()
{
    /**
     * Create message with "new" and
     * parameters "0" and "hello".
     * Then send it to the bus using
     * sendMessage(MessageBase*).
     */
}

void UserInputReceiver::messageReceived(MessageBase* message)
{
    /**
     * Do the same as in SimpleMessageReceiver::messageReceived(MessageBase*)
     * but before check that message->type() == UserInput.
     */
}

void MouseClickReceiver::messageReceived(MessageBase* message)
{
    /**
     * Save x and y coordinates, using dynamic_cast of message to
     * MouseClickMessage*, but before check that
     * message->type() == MouseClick and button clicked == 1.
     */
}


// Next goes task implementation detail,
// you don't need to overview them



MessageBase::MessageBase()
    : m_type(0)
{}

MessageBase::MessageBase(int type, QString message)
    : m_type(type)
    , m_message(message)
{}

MessageBase::~MessageBase()
{}

int MessageBase::type() const
{
    return m_type;
}

QString MessageBase::message() const
{
    return m_message;
}

MouseClickMessage::MouseClickMessage()
{}

MouseClickMessage::MouseClickMessage(int x, int y, int button)
    : MessageBase(MouseClick, QString())
    , m_x(x)
    , m_y(y)
    , m_button(button)
{}

MouseClickMessage::~MouseClickMessage()
{}

int MouseClickMessage::x() const
{
    return m_x;
}

int MouseClickMessage::y() const
{
    return m_y;
}

int MouseClickMessage::button() const
{
    return m_button;
}

MouseClickReceiver::MouseClickReceiver()
    : m_x(0)
    , m_y(0)
{}

std::vector<MessageReceiver*> __receivers;
std::vector<MessageBase*> __messages;
std::mutex __bus_mutex;
typedef std::lock_guard<std::mutex> guard;

void registerMessageReceiver(MessageReceiver* receiver)
{
    guard g(__bus_mutex);
    __receivers.push_back(receiver);
}

void removeMessageReceiver(MessageReceiver* receiver)
{
    guard g(__bus_mutex);
    auto i = std::find(__receivers.begin(), __receivers.end(), receiver);
    if (i != __receivers.end())
        __receivers.erase(i);
}

void sendMessage(MessageBase* message)
{
    guard g(__bus_mutex);
    __messages.push_back(message);
}

void deliverMessages()
{
    guard g(__bus_mutex);
    std::for_each(__messages.begin(), __messages.end(),
                  [] (MessageBase* message) -> void
    {
        std::for_each(__receivers.begin(), __receivers.end(),
                      [message] (MessageReceiver* receiver) -> void
        {
            receiver->messageReceived(message);
        });
        delete message;
    });
    __messages.clear();
}
