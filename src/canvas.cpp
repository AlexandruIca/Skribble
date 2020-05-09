#include "canvas.hpp"
#include "client.hpp"
#include "dummy_network.hpp"
#include "message_parser.hpp"
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
        // m_network->doSomething();
        m_network->sendDrawAt(pos);
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
    // m_network->doSomething();
    m_network->sendMouseReleased();
}

auto Canvas::undo() -> void
{
    m_history.undo(m_foreign);
    // m_network->doSomething();
    m_network->sendUndo();
    this->update();
}

auto Canvas::redo() -> void
{
    m_history.redo(m_foreign);
    // m_network->doSomething();
    m_network->sendRedo();
    this->update();
}

auto Canvas::toggleForeign() -> void
{
    m_foreign = !m_foreign;
}

auto Canvas::onReceivedMessage(QString const& msg) -> void
{
    auto const& [op, a, b, c, d] = sk::parse(msg.toStdString());

    switch(op) {
    case sk::Operation::DRAW_AT: {
        auto const prevColor = m_drawMode->getColor();
        int const prevWidth = m_drawMode->getWidth();

        m_drawMode->setColor(m_foreignColor);
        m_drawMode->setWidth(m_foreignWidth);

        m_history.drawAt({ a, b }, *m_drawMode, true);

        m_drawMode->setColor(prevColor);
        m_drawMode->setWidth(prevWidth);

        this->update();
        return;
    }
    case sk::Operation::MOUSE_RELEASED: {
        m_history.pushNewLayer(true);
        this->update();
        return;
    }
    case sk::Operation::CHANGE_COLOR: {
        m_foreignColor = QColor{ a, b, c, d };
        return;
    }
    case sk::Operation::CHANGE_WIDTH: {
        m_foreignWidth = a;
        return;
    }
    case sk::Operation::UNDO: {
        m_history.undo(true);
        this->update();
        return;
    }
    case sk::Operation::REDO: {
        m_history.redo(true);
        this->update();
        return;
    }
    case sk::Operation::TO_PEN:
    case sk::Operation::TO_BRUSH: {
        qDebug() << "[Canvas] Received tool change!";
        return;
    }
    case sk::Operation::NONE: {
        qDebug() << "[Canvas] Received empty string, shouldn't happen!";
        return;
    }
    default: {
        qDebug() << "[Canvas] Case not handled when receiving message!";
        return;
    }
    }
}

} // namespace sk
