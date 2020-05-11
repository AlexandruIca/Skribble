#include "server.hpp"
#include "network_config.hpp"

#include <QDebug>
#include <QString>

namespace sk {

Server::Server()
{
    m_server = std::make_unique<QTcpServer>(this);
    m_server->listen(QHostAddress::Any, sk::port);

    connect(m_server.get(), &QTcpServer::newConnection, [this] {
        m_socket = m_server->nextPendingConnection();

        if(m_socket == nullptr) {
            qDebug() << "[Server] Could't create socket for pending connection";
        }

        m_socket->write("Hello from server");

        connect(m_socket, &QTcpSocket::readyRead, [this] {
            QString str{ m_socket->readAll() };
            qDebug() << "Server received: " << str;
            emit receivedMessage(str);
        });
    });
}

Server::~Server() noexcept
{
    if(m_socket != nullptr) {
        m_socket->disconnectFromHost();
    }
    m_server->disconnect();
}

auto Server::getSocket() -> QTcpSocket*
{
    return m_socket;
}

} // namespace sk
