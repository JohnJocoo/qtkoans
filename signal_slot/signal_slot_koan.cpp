#include <QObject>
#include <QTest>
#include <QScopedPointer>

#include "callbacks.h"
#include "message_bus.h"

class SignalSlotKoan : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void cleanup();

    void basicCallback();
    void callCallback();
    void deleteCallback();

    void busSimple();
    void busDifferent();
};

void SignalSlotKoan::initTestCase()
{

}

void SignalSlotKoan::cleanupTestCase()
{

}

void SignalSlotKoan::basicCallback()
{
    InputDataReceiver receiver;
    QCOMPARE(receiver.m_data, QString());
    receiver.inputReady(QString::fromLatin1("new data"));
    QCOMPARE(receiver.m_data, QString::fromLatin1("new data"));
}

void SignalSlotKoan::callCallback()
{
    InputDataReceiver receiver;
    InputDataProvider provider;

    QCOMPARE(receiver.m_data, QString());
    QCOMPARE(provider.m_data, QString());
    QCOMPARE(provider.m_callback, reinterpret_cast<Callback*>(NULL));

    provider.updateData(QString::fromLatin1("Name"),
                        QString::fromLatin1("Surname"));
    QCOMPARE(provider.m_data, QString::fromLatin1("Name Surname"));
    QCOMPARE(provider.m_callback, reinterpret_cast<Callback*>(NULL));
    QCOMPARE(receiver.m_data, QString());

    provider.setCallback(&receiver);
    QCOMPARE(provider.m_callback, &receiver);

    provider.updateData(QString::fromLatin1("John"),
                        QString::fromLatin1("Jocoo"));
    QCOMPARE(provider.m_data, QString::fromLatin1("John Jocoo"));
    QCOMPARE(receiver.m_data, QString::fromLatin1("John Jocoo"));
}

void SignalSlotKoan::deleteCallback()
{
    QScopedPointer<InputDataReceiver2> receiver(new InputDataReceiver2());
    InputDataProvider2 provider;

    QCOMPARE(receiver->m_data, QString());
    QCOMPARE(provider.m_data, QString());
    QCOMPARE(receiver->m_provider, reinterpret_cast<ProviderInterface*>(NULL));
    QCOMPARE(provider.m_callback, reinterpret_cast<Callback*>(NULL));

    provider.updateData(QString::fromLatin1("Name"),
                        QString::fromLatin1("Surname"));
    QCOMPARE(provider.m_data, QString::fromLatin1("Name Surname"));
    QCOMPARE(provider.m_callback, reinterpret_cast<Callback*>(NULL));
    QCOMPARE(receiver->m_provider, reinterpret_cast<ProviderInterface*>(NULL));
    QCOMPARE(receiver->m_data, QString());

    receiver->setProvider(&provider);
    QCOMPARE(receiver->m_provider, &provider);
    QCOMPARE(provider.m_callback, receiver.data());

    provider.updateData(QString::fromLatin1("John"),
                        QString::fromLatin1("Jocoo"));
    QCOMPARE(provider.m_data, QString::fromLatin1("John Jocoo"));
    QCOMPARE(receiver->m_data, QString::fromLatin1("John Jocoo"));

    receiver.reset();
    QCOMPARE(provider.m_callback, reinterpret_cast<Callback*>(NULL));

    provider.updateData(QString::fromLatin1("Name"),
                        QString::fromLatin1("Surname"));
    QCOMPARE(provider.m_data, QString::fromLatin1("Name Surname"));
    QCOMPARE(provider.m_callback, reinterpret_cast<Callback*>(NULL));
}

extern void deliverMessages();
extern std::vector<MessageReceiver*> __receivers;
extern std::vector<MessageBase*> __messages;

void SignalSlotKoan::cleanup()
{
    __receivers.clear();
    __messages.clear();
}

void SignalSlotKoan::busSimple()
{
    QCOMPARE(__receivers.size(), (size_t)0);
    QCOMPARE(__messages.size(), (size_t)0);

    {
        SimpleMessageReceiver receiver;
        QCOMPARE(__receivers.size(), (size_t)1);
        QCOMPARE(__messages.size(), (size_t)0);
        QCOMPARE(receiver.m_data, QString());

        sendMessage(new MessageBase(0, QString::fromLatin1("Hello there")));
        QCOMPARE(__receivers.size(), (size_t)1);
        QCOMPARE(__messages.size(), (size_t)1);
        QCOMPARE(receiver.m_data, QString());

        deliverMessages();
        QCOMPARE(__receivers.size(), (size_t)1);
        QCOMPARE(__messages.size(), (size_t)0);
        QCOMPARE(receiver.m_data, QString::fromLatin1("Hello there"));

        sendHelloMessage();
        QCOMPARE(__receivers.size(), (size_t)1);
        QCOMPARE(__messages.size(), (size_t)1);

        deliverMessages();
        QCOMPARE(__receivers.size(), (size_t)1);
        QCOMPARE(__messages.size(), (size_t)0);
        QCOMPARE(receiver.m_data.toLower(), QString::fromLatin1("hello"));
    }

    QCOMPARE(__receivers.size(), (size_t)0);
    QCOMPARE(__messages.size(), (size_t)0);
}

void SignalSlotKoan::busDifferent()
{
    QCOMPARE(__receivers.size(), (size_t)0);
    QCOMPARE(__messages.size(), (size_t)0);

    {
        UserInputReceiver ui_receiver;
        MouseClickReceiver mc_receiver;
        QCOMPARE(__receivers.size(), (size_t)2);
        QCOMPARE(__messages.size(), (size_t)0);
        QCOMPARE(ui_receiver.m_data, QString());
        QCOMPARE(mc_receiver.m_x, 0);
        QCOMPARE(mc_receiver.m_y, 0);

        sendMessage(new MessageBase(UserInput, QString::fromLatin1("Hello there")));
        QCOMPARE(__receivers.size(), (size_t)2);
        QCOMPARE(__messages.size(), (size_t)1);
        QCOMPARE(ui_receiver.m_data, QString());
        QCOMPARE(mc_receiver.m_x, 0);
        QCOMPARE(mc_receiver.m_y, 0);

        deliverMessages();
        QCOMPARE(__receivers.size(), (size_t)2);
        QCOMPARE(__messages.size(), (size_t)0);
        QCOMPARE(ui_receiver.m_data, QString::fromLatin1("Hello there"));
        QCOMPARE(mc_receiver.m_x, 0);
        QCOMPARE(mc_receiver.m_y, 0);

        sendMessage(new MouseClickMessage(56, 198, 0));
        deliverMessages();
        QCOMPARE(__receivers.size(), (size_t)2);
        QCOMPARE(__messages.size(), (size_t)0);
        QCOMPARE(ui_receiver.m_data, QString::fromLatin1("Hello there"));
        QCOMPARE(mc_receiver.m_x, 0);
        QCOMPARE(mc_receiver.m_y, 0);

        sendMessage(new MouseClickMessage(560, 243, 1));
        deliverMessages();
        QCOMPARE(__receivers.size(), (size_t)2);
        QCOMPARE(__messages.size(), (size_t)0);
        QCOMPARE(ui_receiver.m_data, QString::fromLatin1("Hello there"));
        QCOMPARE(mc_receiver.m_x, 560);
        QCOMPARE(mc_receiver.m_y, 243);
    }

    QCOMPARE(__receivers.size(), (size_t)0);
    QCOMPARE(__messages.size(), (size_t)0);
}

QTEST_MAIN(SignalSlotKoan)
#include "signal_slot_koan.moc"
