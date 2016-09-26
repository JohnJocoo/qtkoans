#include "callbacks.h"

void InputDataReceiver::inputReady(QString data)
{
    /**
     * You received your data with callback, now
     * store it.
     */
}

void InputDataProvider::setCallback(Callback* callback)
{
    /**
     * Someone wants to register callback,
     * so store it.
     */
}

void InputDataProvider::updateData(QString name, QString surname)
{
    /**
     * Concatirnate name + " " + surname,
     * store result and notify receiver if
     * callback != NULL
     */
}

void InputDataProvider2::setCallback(Callback* callback)
{
    /**
     * Someone wants to register callback,
     * so store it.
     */
}

void InputDataProvider2::removeCallback(Callback* callback)
{
    /**
     * Unregister callback, setting it to NULL.
     */
}

void InputDataProvider2::updateData(QString name, QString surname)
{
    /**
     * Concatirnate name + " " + surname,
     * store result and notify receiver if
     * callback != NULL
     */
}

void InputDataReceiver2::inputReady(QString data)
{
    /**
     * You received your data with callback, now
     * store it.
     */
}

void InputDataReceiver2::setProvider(ProviderInterface* provider)
{
    /**
     * So here concept changes a little:
     * you no longer register callback to provider,
     * but register provider to receiver.
     * Receiver stores pointer to provider to
     * notify when it's deleted, and when registers
     * itself as callback to provider.
     */
}

InputDataReceiver2::~InputDataReceiver2()
{
    /**
     * If m_provider isn't NULL notify it
     * that object is deleted by calling removeCallback().
     */
}

InputDataProvider::InputDataProvider()
    : m_callback(NULL)
{}

InputDataProvider2::InputDataProvider2()
    : m_callback(NULL)
{}

InputDataReceiver2::InputDataReceiver2()
    : m_provider(NULL)
{}

Callback::~Callback()
{}
