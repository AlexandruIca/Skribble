#include "dummy_network.hpp"

#include <QDebug>

namespace sk {

auto DummyNetwork::getSocket() -> QTcpSocket*
{
    return nullptr;
}

} // namespace sk
