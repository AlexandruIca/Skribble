#include "canvas.hpp"

#include <cstddef>
#include <iostream>

namespace sk {

Canvas::Canvas(QQuickPaintedItem* parent)
    : QQuickPaintedItem{ parent }
{
    m_points.emplace_back();
}

auto Canvas::mousePositionChanged(QPoint const& pos) -> void
{
    m_points.back().emplace_back(pos.x(), pos.y());
    this->update();
}

auto Canvas::paint(QPainter* painter) -> void
{
    painter->setRenderHints(QPainter::Antialiasing |
                            QPainter::SmoothPixmapTransform);
    painter->setPen(m_pen);

    for(std::size_t j = 0; j < m_points.size(); ++j) {
        for(std::size_t i = 1; i < m_points[j].size(); ++i) {
            painter->drawLine(m_points[j][i - 1], m_points[j][i]);
        }
    }
}

auto Canvas::mouseReleased() -> void
{
    m_points.emplace_back();
}

} // namespace sk
