#include "canvas.hpp"

#include <cstddef>
#include <iostream>
#include <utility>

namespace sk {

Canvas::Canvas(QQuickPaintedItem* parent)
    : QQuickPaintedItem{ parent }
{
    m_drawMode = makeDrawMode(PenMode{});
}

auto Canvas::mousePositionChanged(QPoint const& pos) -> void
{
    if(m_foreign) {
        auto const prevColor = m_drawMode->getColor();
        int const prevWidth = m_drawMode->getWidth();

        m_drawMode->setColor(Qt::red);
        m_drawMode->setWidth(20);

        m_history.drawAt(pos, *m_drawMode, m_foreign);

        m_drawMode->setColor(prevColor);
        m_drawMode->setWidth(prevWidth);
    }
    else {
        m_history.drawAt(pos, *m_drawMode, m_foreign);
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
}

auto Canvas::undo() -> void
{
    m_history.undo(m_foreign);
    this->update();
}

auto Canvas::redo() -> void
{
    m_history.redo(m_foreign);
    this->update();
}

auto Canvas::toggleForeign() -> void
{
    m_foreign = !m_foreign;
}

} // namespace sk
