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
    m_history.drawAt(pos, *m_drawMode);
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
    m_history.pushNewLayer();
}

auto Canvas::undo() -> void
{
    m_history.undo();
    this->update();
}

auto Canvas::redo() -> void
{
    m_history.redo();
    this->update();
}

} // namespace sk
