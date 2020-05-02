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

[[nodiscard]] auto DrawHistory::getLastLayer(bool const foreign) -> QPixmap&
{
    return this->getLastLayerIter(foreign).getLastLayer();
}

[[nodiscard]] auto DrawHistory::getLastLayerIter(bool const foreign)
    -> impl::CachedLayers&
{
    /*
    auto it = m_layers.getUnderlying().rbegin();

    while(it != m_layers.getUnderlying().rend() && it->foreign() != foreign) {
        ++it;
    }

    if(it->foreign() != foreign) {
        // ERROR
    }

    return *it;*/

    for(auto& it : m_layers2) {
        if(it.foreign() == foreign) {
            return it;
        }
    }

    return *m_layers2.begin();
}

DrawHistory::DrawHistory()
{
    // m_layers.emplaceBack();
    m_layers2.emplace_back();
}

auto DrawHistory::pushNewLayer(bool const foreign) -> void
{
    /*
    if(m_layers.getUnderlying().back().foreign() != foreign) {
        m_layers.emplaceBack(foreign);
    }
    else {
        m_layers.getUnderlying().back().pushNewLayer();
    }*/
    if(foreign) {
        m_drawingExternally = false;
        m_lastExternalPoint = std::nullopt;
    }
    else {
        m_drawingLocally = false;
        m_lastPoint = std::nullopt;
    }

    for(auto& layer : m_layers2) {
        if(layer.foreign() == foreign) {
            layer.pushNewLayer();
        }
    }

    qDebug() << "popped\n";
}

auto DrawHistory::paintCanvas(QPainter* const painter) -> void
{
    /*
    m_layers.reduceTo([&painter](impl::CachedLayers& src) -> void {
        src.paintBlock(*painter);
    });*/
    painter->drawPixmap(impl::CachedLayers::getCanvasRect(),
                        m_cache.getLastLayer(),
                        impl::CachedLayers::getCanvasRect());
    for(auto& layer : m_layers2) {
        layer.paintBlock(*painter);
    }
}

auto DrawHistory::popFirst(bool const foreign) -> void
{
    if(m_layers2.empty()) {
        return;
    }
    if(m_layers2.front().foreign() != foreign) {
        return;
    }

    QPainter painter{ &(m_cache.getLastLayer()) };
    painter.drawPixmap(impl::CachedLayers::getCanvasRect(),
                       m_layers2.front().getLastLayer(),
                       impl::CachedLayers::getCanvasRect());

    m_layers2.pop_front();
}

auto DrawHistory::handleExternal(QPoint const& pos, DrawMode& mode) -> void
{
    if(auto& last = m_layers2.back(); last.foreign()) {
        if(last.underUndo()) {
            last.pushNewLayer();
        }

        QPainter painter{ &last.getLastLayer() };
        mode.draw(painter, pos, m_lastExternalPoint);
    }
    else {
        if(!m_drawingExternally) {
            this->popFirst(true);

            QPainter painter{ &m_layers2.emplace_back(true).getLastLayer() };
            mode.draw(painter, pos, m_lastExternalPoint);
        }
        else {
            for(auto& layer : m_layers2) {
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
    if(auto& last = m_layers2.back(); !last.foreign()) {
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
            m_layers2.emplace_back(false);
            m_drawingLocally = true;

            QPainter painter{ &m_layers2.back().getLastLayer() };
            mode.draw(painter, pos, m_lastPoint);
        }
        else {
            for(auto& layer : m_layers2) {
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
    /*
    if(auto& last = this->getLastLayerIter(foreign); last.underUndo()) {
        last.pushNewLayer();
    }

    QPainter painter{ &this->getLastLayer(foreign) };
    mode.draw(painter, pos, m_lastPoint);*/

    if(foreign) {
        this->handleExternal(pos, mode);
    }
    else {
        this->handleLocal(pos, mode);
    }
}

auto DrawHistory::undo(bool const foreign) -> void
{
    // We undo two times here if undo is hit for the 'first' time
    // because remember: when the user releases the left click
    // we create a new layer which is an empty layer.
    // If we delete this if then the user has to press 'Ctrl-z' two times
    // to see the undo take effect because the firsst time it only skips
    // the empty layer.
    // auto& last = this->getLastLayerIter(foreign);
    /*
    if(!last.underUndo()) {
        static_cast<void>(last.undo());
    }*/

    for(auto& layer : m_layers2) {
        if(layer.foreign() == foreign) {
            qDebug() << "Undoing!!\n";
            static_cast<void>(layer.undo());
            break;
        }
    }
}

auto DrawHistory::redo(bool const foreign) -> void
{
    for(auto& layer : m_layers2) {
        if(layer.foreign() == foreign) {
            static_cast<void>(layer.redo());
            break;
        }
    }
}

} // namespace sk
