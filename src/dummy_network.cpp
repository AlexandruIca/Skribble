#include "dummy_network.hpp"

#include <QDebug>

namespace sk {

auto DummyNetwork::doSomething() -> void
{
    qDebug() << "Called from Dummy object!";
}

} // namespace sk

