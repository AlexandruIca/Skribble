#include "canvas.hpp"
#include "client.hpp"
#include "dummy_network.hpp"
#include "network_config.hpp"
#include "network_factory.hpp"
#include "server.hpp"

#include <cstddef>
#include <iostream>
#include <utility>

namespace sk {

Canvas::Canvas(QQuickPaintedItem* parent)
    : QQuickPaintedItem{ parent }
{
    m_drawMode = makeDrawMode(PenMode{});
    m_network = NetworkFactory::create(sk::networkMode);
    connect(m_network.get(),
            &AbstractNetwork::receivedMessage,
            this,
            &Canvas::onReceivedMessage);
    /*
        switch(sk::networkMode) {
        case sk::NetworkModes::SINGLE_USER: {
            m_network = std::make_unique<DummyNetwork>();
            connect(m_network.get(),
                    &AbstractNetwork::receivedMessage,
                    this,
                    &Canvas::onReceivedMessage);
            break;
        }
        case sk::NetworkModes::CLIENT: {
            m_network = std::make_unique<Client>(sk::host_ip);
            connect(m_network.get(),
                    &AbstractNetwork::receivedMessage,
                    this,
                    &Canvas::onReceivedMessage);
            break;
        }
        case sk::NetworkModes::SERVER: {
            m_network = std::make_unique<Server>();
            break;
        }
        }*/
}

auto Canvas::mousePositionChanged(QPoint const& pos) -> void
{
    if(m_foreign) {
        auto const prevColor = m_drawMode->getColor();
        int const prevWidth = m_drawMode->getWidth();

        m_drawMode->setColor(m_foreignColor);
        m_drawMode->setWidth(m_foreignWidth);

        m_history.drawAt(pos, *m_drawMode, m_foreign);

        m_drawMode->setColor(prevColor);
        m_drawMode->setWidth(prevWidth);
    }
    else {
        m_history.drawAt(pos, *m_drawMode, m_foreign);
        m_network->doSomething();
    }
    this->update();
}

auto Canvas::paint(QPainter* painter) -> void
{
    painter->setRenderHints(QPainter::Antialiasing |
                            QPainter::SmoothPixmapTransform);
    m_history.paintCanvas(painter);
}

auto Canvas::mouseReleased() -> void
{
    m_history.pushNewLayer(m_foreign);
    m_network->doSomething();
}

auto Canvas::undo() -> void
{
    m_history.undo(m_foreign);
    m_network->doSomething();
    this->update();
}

auto Canvas::redo() -> void
{
    m_history.redo(m_foreign);
    m_network->doSomething();
    this->update();
}

auto Canvas::toggleForeign() -> void
{
    m_foreign = !m_foreign;
}

auto Canvas::onReceivedMessage(QString const& msg) -> void
{
    qDebug() << "[Canvas] Received: " << msg;
}

} // namespace sk
