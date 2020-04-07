#include "canvas.hpp"

#include <iostream>

namespace sk {

Canvas::Canvas(QQuickPaintedItem* parent)
    : QQuickPaintedItem{ parent }
{
}

auto Canvas::mousePositionChanged(QPoint const& pos) -> void
{
    m_points.emplace_back(pos.x(), pos.y());
    this->update();
}

auto Canvas::paint(QPainter* painter) -> void
{
    painter->setPen(QPen{
        QColor{ "blue" }, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin });
    for(auto const& point : m_points) {
        painter->drawPoint(point);
    }
}

} // namespace sk
