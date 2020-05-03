#include "draw_history.hpp"

#include <QDebug>

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

auto CachedLayers::PixmapInit(QPixmap& pixmap) noexcept -> void
{
    pixmap.fill(m_transparent);
}

CachedLayers::CachedLayers(bool const foreign)
    : m_foreign{ foreign }
{
    m_layers.emplaceBack(sk::config::width, sk::config::height);
}

auto CachedLayers::pushNewLayer() -> void
{
    m_layers.emplaceBack(sk::config::width, sk::config::height);
}

auto CachedLayers::paintBlock(QPainter& painter) -> void
{
    painter.drawPixmap(m_canvasRect, m_layers.get(), m_canvasRect);
}

[[nodiscard]] auto CachedLayers::getLastLayer() noexcept -> QPixmap&
{
    return m_layers.get();
}

[[nodiscard]] auto CachedLayers::getLastLayer() const noexcept -> QPixmap const&
{
    return m_layers.get();
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

DrawHistory::DrawHistory()
{
    m_layers.emplace_back();
    m_cache.emplaceBack();
}

auto DrawHistory::pushNewLayer(bool const foreign) -> void
{
    if(foreign) {
        m_drawingExternally = false;
        m_lastExternalPoint = std::nullopt;
    }
    else {
        m_drawingLocally = false;
        m_lastPoint = std::nullopt;
    }

    for(auto& layer : m_layers) {
        if(layer.foreign() == foreign) {
            layer.pushNewLayer();
        }
    }
}

auto DrawHistory::paintCanvas(QPainter* const painter) -> void
{
    painter->drawPixmap(impl::CachedLayers::getCanvasRect(),
                        m_cache.getLast().getLastLayer(),
                        impl::CachedLayers::getCanvasRect());
    for(auto& layer : m_layers) {
        layer.paintBlock(*painter);
    }
}

auto DrawHistory::popFirst(bool const foreign) -> void
{
    if(m_layers.empty()) {
        return;
    }
    if(m_layers.front().foreign() != foreign) {
        return;
    }

    QPainter painter{ &(m_cache.getLast().getLastLayer()) };
    painter.drawPixmap(impl::CachedLayers::getCanvasRect(),
                       m_layers.front().getLastLayer(),
                       impl::CachedLayers::getCanvasRect());

    m_layers.pop_front();
}

auto DrawHistory::handleExternal(QPoint const& pos, DrawMode& mode) -> void
{
    if(auto& last = m_layers.back(); last.foreign()) {
        if(last.underUndo()) {
            last.pushNewLayer();
        }

        QPainter painter{ &last.getLastLayer() };
        mode.draw(painter, pos, m_lastExternalPoint);
    }
    else {
        if(!m_drawingExternally) {
            this->popFirst(true);

            QPainter painter{ &m_layers.emplace_back(true).getLastLayer() };
            mode.draw(painter, pos, m_lastExternalPoint);
        }
        else {
            for(auto& layer : m_layers) {
                if(layer.foreign()) {
                    QPainter painter{ &layer.getLastLayer() };
                    mode.draw(painter, pos, m_lastExternalPoint);
                    break;
                }
            }
        }
    }

    m_drawingExternally = true;
    m_lastExternalPoint = pos;
}

auto DrawHistory::handleLocal(QPoint const& pos, DrawMode& mode) -> void
{
    if(auto& last = m_layers.back(); !last.foreign()) {
        if(last.underUndo()) {
            last.pushNewLayer();
        }

        QPainter painter{ &last.getLastLayer() };
        mode.draw(painter, pos, m_lastPoint);
        m_drawingLocally = true;
    }
    else {
        if(!m_drawingLocally) {
            this->popFirst(false);
            m_layers.emplace_back(false);
            m_drawingLocally = true;

            QPainter painter{ &m_layers.back().getLastLayer() };
            mode.draw(painter, pos, m_lastPoint);
        }
        else {
            for(auto& layer : m_layers) {
                if(!layer.foreign()) {
                    QPainter painter{ &layer.getLastLayer() };
                    mode.draw(painter, pos, m_lastPoint);
                    break;
                }
            }
        }
    }

    m_lastPoint = pos;
}

auto DrawHistory::drawAt(QPoint const& pos, DrawMode& mode, bool const foreign)
    -> void
{
    if(foreign) {
        this->handleExternal(pos, mode);
    }
    else {
        this->handleLocal(pos, mode);
    }
}

auto DrawHistory::undo(bool const foreign) -> void
{
    for(auto& layer : m_layers) {
        if(layer.foreign() == foreign) {
            static_cast<void>(layer.undo());
            break;
        }
    }
}

auto DrawHistory::redo(bool const foreign) -> void
{
    for(auto& layer : m_layers) {
        if(layer.foreign() == foreign) {
            static_cast<void>(layer.redo());
            break;
        }
    }
}

} // namespace sk
