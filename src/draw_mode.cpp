#include "draw_mode.hpp"

namespace sk {

int DrawMode::m_width = DrawMode::m_defaultWidth;
QColor DrawMode::m_color = DrawMode::m_defaultColor;

PenMode::PenMode(QPen const& pen)
    : m_pen{ pen }
{
}

PenMode::PenMode(QPen&& pen)
    : m_pen{ std::move(pen) }
{
}

auto PenMode::draw(QPainter& painter,
                   QPoint const& pos,
                   std::optional<QPoint> const& lastPoint) -> void
{
    m_pen.setColor(m_color);
    m_pen.setWidth(m_width);
    painter.setPen(m_pen);

    if(lastPoint.has_value()) {
        painter.drawLine(pos, lastPoint.value());
    }
    else {
        painter.drawPoint(pos);
    }
}

BrushMode::BrushMode(QBrush const& brush)
    : m_brush{ brush }
{
}

BrushMode::BrushMode(QBrush&& brush)
    : m_brush{ std::move(brush) }
{
}

auto BrushMode::draw(QPainter& painter,
                     QPoint const& pos,
                     std::optional<QPoint> const&) -> void
{
    m_brush.setColor(m_color);
    painter.setBrush(m_brush);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(pos, m_width / 2, m_width / 2);
}

} // namespace sk
