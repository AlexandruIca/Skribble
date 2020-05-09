#include "client.hpp"
#include "network_config.hpp"

#include <QDebug>
#include <QGuiApplication>

#include <iostream>

namespace sk {

Client::Client(QString const& ip)
{
    qDebug() << "Port to connect to: " << ip;
    m_server = std::make_unique<QTcpSocket>(this);
    m_server->connectToHost(ip, sk::port);

    if(!m_server->waitForConnected()) {
        qDebug() << "Could't establish connection to the server!";
    }

    connect(m_server.get(), &QTcpSocket::readyRead, [this] {
        QString str{ m_server->readAll() };
        qDebug() << "Client received: " << str;
        emit receivedMessage(str);
    });
    connect(m_server.get(), &QTcpSocket::disconnected, [] {
        qDebug() << "Quitting from client!";
        QGuiApplication::quit();
    });

    m_server->write("Hello");
}

Client::~Client() noexcept
{
    m_server->disconnectFromHost();
}

auto Client::doSomething() -> void
{
    qDebug() << "Hello from Client!";
    m_server->write("Hello from Client::DoSomething");
}

auto Client::getSocket() -> QTcpSocket*
{
    return m_server.get();
}

} // namespace sk
