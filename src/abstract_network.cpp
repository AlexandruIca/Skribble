#include "abstract_network.hpp"
#include "format.hpp"

#include <QByteArray>
#include <QString>

#include <string>

namespace sk {

namespace {

auto handleWrite(QTcpSocket& socket) -> void
{
    if(!socket.waitForBytesWritten()) {
        qDebug() << "Couldn't send message!";
    }
}

} // namespace

AbstractNetwork::AbstractNetwork(QObject* parent)
    : QObject{ parent }
{
}

auto AbstractNetwork::sendDrawAt(QPoint const& pos) -> void
{
    if(auto* socket = this->getSocket(); socket != nullptr) {
        std::string msg = sk::format("d %1 %2", pos.x(), pos.y());
        socket->write(QByteArray::fromStdString(msg));
        handleWrite(*socket);
    }
}

auto AbstractNetwork::sendMouseReleased() -> void
{
    if(auto* socket = this->getSocket(); socket != nullptr) {
        socket->write("m");
        handleWrite(*socket);
    }
}

auto AbstractNetwork::sendUndo() -> void
{
    if(auto* socket = this->getSocket(); socket != nullptr) {
        socket->write("u");
        handleWrite(*socket);
    }
}

auto AbstractNetwork::sendRedo() -> void
{
    if(auto* socket = this->getSocket(); socket != nullptr) {
        socket->write("r");
        handleWrite(*socket);
    }
}

auto AbstractNetwork::sendChangeColor(QColor const& color) -> void
{
    if(auto* socket = this->getSocket(); socket != nullptr) {
        std::string msg = sk::format("c %1 %2 %3 %4",
                                     color.red(),
                                     color.green(),
                                     color.blue(),
                                     color.alpha());
        socket->write(QByteArray::fromStdString(msg));
        handleWrite(*socket);
    }
}

auto AbstractNetwork::sendChangeWidth(int const width) -> void
{
    if(auto* socket = this->getSocket(); socket != nullptr) {
        std::string msg = sk::format("w %1", width);
        socket->write(QByteArray::fromStdString(msg));
        handleWrite(*socket);
    }
}

auto AbstractNetwork::sendToBrush() -> void
{
    if(auto* socket = this->getSocket(); socket != nullptr) {
        socket->write("b");
        handleWrite(*socket);
    }
}

auto AbstractNetwork::sendToPen() -> void
{
    if(auto* socket = this->getSocket(); socket != nullptr) {
        socket->write("p");
        handleWrite(*socket);
    }
}

} // namespace sk
