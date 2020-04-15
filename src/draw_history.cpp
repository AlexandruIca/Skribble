#include "draw_history.hpp"

#include <algorithm>
#include <cstddef>
#include <deque>
#include <iterator>

namespace sk::impl {

auto CachedLayers::PixmapDrawer(QPixmap& dest, QPixmap& src) -> void
{
    QPainter painter{ &dest };
    painter.drawPixmap(
        CachedLayers::m_canvasRect, src, CachedLayers::m_canvasRect);
}

CachedLayers::CachedLayers(bool const foreign)
    : m_foreign{ foreign }
{
    m_layers.emplaceBack(sk::config::width, sk::config::height)
        .fill(m_transparent);
}

auto CachedLayers::pushNewLayer() -> void
{
    m_layers.emplaceBack(sk::config::width, sk::config::height)
        .fill(m_transparent);
}

auto CachedLayers::paintBlock(QPainter& painter) -> void
{
    m_layers.reduceTo([&painter](QPixmap const& src) -> void {
        painter.drawPixmap(m_canvasRect, src, m_canvasRect);
    });
}

[[nodiscard]] auto CachedLayers::getLastLayer() noexcept -> QPixmap&
{
    return m_layers.getLast();
}

[[nodiscard]] auto CachedLayers::getLastLayer() const noexcept -> QPixmap const&
{
    return m_layers.getLast();
}

[[nodiscard]] auto CachedLayers::undo() -> bool
{
    return m_layers.undo();
}

auto CachedLayers::redo() -> bool
{
    return m_layers.redo();
}

} // namespace sk::impl

namespace sk {

auto DrawHistory::CachedDrawer(impl::CachedLayers& dest,
                               impl::CachedLayers& src) -> void
{
    QPainter painter{ &dest.getLastLayer() };
    src.paintBlock(painter);
}

[[nodiscard]] auto DrawHistory::getLastLayer(bool const foreign) -> QPixmap&
{
    return this->getLastLayerIter(foreign).getLastLayer();
}

[[nodiscard]] auto DrawHistory::getLastLayerIter(bool const foreign)
    -> impl::CachedLayers&
{
    auto it = m_layers.getUnderlying().rbegin();

    while(it != m_layers.getUnderlying().rend() && it->foreign() != foreign) {
        ++it;
    }

    if(it->foreign() != foreign) {
        // ERROR
    }

    return *it;
}

DrawHistory::DrawHistory()
{
    m_layers.emplaceBack();
}

auto DrawHistory::pushNewLayer(bool const foreign) -> void
{
    if(m_layers.getUnderlying().back().foreign() != foreign) {
        m_layers.emplaceBack(foreign);
    }
    else {
        m_layers.getUnderlying().back().pushNewLayer();
    }
    m_lastPoint = std::nullopt;
}

auto DrawHistory::paintCanvas(QPainter* const painter) -> void
{
    m_layers.reduceTo([&painter](impl::CachedLayers& src) -> void {
        src.paintBlock(*painter);
    });
}

auto DrawHistory::drawAt(QPoint const& pos, QPen const& pen, bool const foreign)
    -> void
{
    if(auto& last = this->getLastLayerIter(foreign); last.underUndo()) {
        last.pushNewLayer();
    }

    QPainter painter{ &this->getLastLayer(foreign) };
    painter.setPen(pen);

    if(m_lastPoint.has_value()) {
        painter.drawLine(m_lastPoint.value(), pos);
    }
    else {
        painter.drawPoint(pos);
    }

    m_lastPoint = pos;
}

auto DrawHistory::undo(bool const foreign) -> void
{
    // We undo two times here if undo is hit for the 'first' time
    // because remember: when the user releases the left click
    // we create a new layer which is an empty layer.
    // If we delete this if then the user has to press 'Ctrl-z' two times
    // to see the undo take effect because the firsst time it only skips
    // the empty layer.
    auto& last = this->getLastLayerIter(foreign);
    /*
    if(!last.underUndo()) {
        static_cast<void>(last.undo());
    }*/
    static_cast<void>(last.undo());
}

auto DrawHistory::redo(bool const foreign) -> void
{
    static_cast<void>(this->getLastLayerIter(foreign).redo());
}

} // namespace sk
