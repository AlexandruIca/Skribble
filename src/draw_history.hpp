#ifndef DRAW_HISTORY_HPP
#define DRAW_HISTORY_HPP
#pragma once

#include "cached_resource.hpp"
#include "canvas_config.hpp"

#include <QColor>
#include <QPainter>
#include <QPixmap>
#include <QPoint>

#include <algorithm>
#include <cstddef>
#include <deque>
#include <iostream>
#include <iterator>
#include <optional>

namespace sk::impl {

class CachedLayers
{
private:
    bool const m_foreign{ false };

    static auto PixmapDrawer(QPixmap& dest, QPixmap& src) -> void
    {
        QPainter painter{ &dest };
        painter.drawPixmap(
            CachedLayers::m_canvasRect, src, CachedLayers::m_canvasRect);
    }

    sk::CachedResource<QPixmap> m_layers{ &PixmapDrawer };

    static constexpr QColor m_transparent{ 0, 0, 0, 0 };
    static constexpr QRect m_canvasRect{
        0, 0, sk::config::width, sk::config::height
    };

public:
    explicit CachedLayers(bool const foreign = false)
        : m_foreign{ foreign }
    {
        m_layers.emplaceBack(sk::config::width, sk::config::height)
            .fill(m_transparent);
    }
    CachedLayers(CachedLayers const&) = default;
    CachedLayers(CachedLayers&&) = default;
    ~CachedLayers() noexcept = default;

    auto operator=(CachedLayers const&) -> CachedLayers& = delete;
    auto operator=(CachedLayers &&) -> CachedLayers& = delete;

    auto pushNewLayer() -> void
    {
        m_layers.emplaceBack(sk::config::width, sk::config::height)
            .fill(m_transparent);
    }

    auto paintBlock(QPainter& painter) -> void
    {
        m_layers.reduceTo([&painter](QPixmap const& src) -> void {
            painter.drawPixmap(m_canvasRect, src, m_canvasRect);
        });
    }

    [[nodiscard]] auto getLastLayer() noexcept -> QPixmap&
    {
        return m_layers.getLast();
    }
    [[nodiscard]] auto getLastLayer() const noexcept -> QPixmap const&
    {
        return m_layers.getLast();
    }

    [[nodiscard]] constexpr auto foreign() const noexcept -> bool
    {
        return m_foreign;
    }

    [[nodiscard]] constexpr auto underUndo() const noexcept -> bool
    {
        return m_layers.underUndo();
    }

    ///
    /// \returns true If undo was successful.
    ///          false If already at oldest change
    ///
    [[nodiscard]] auto undo() -> bool
    {
        return m_layers.undo();
    }

    auto redo() -> bool
    {
        return m_layers.redo();
    }
};

} // namespace sk::impl

namespace sk {

class DrawHistory
{
private:
    static auto CachedDrawer(impl::CachedLayers& dest, impl::CachedLayers& src)
        -> void
    {
        QPainter painter{ &dest.getLastLayer() };
        src.paintBlock(painter);
    }

    struct Traits
    {
        using ContainerType = std::deque<impl::CachedLayers>;
        static constexpr int cacheGap = 3;
        static constexpr int maxCount = 50;
    };

    // std::deque<impl::CachedLayers> m_layers{};
    sk::CachedResource<impl::CachedLayers, Traits> m_layers2{ &CachedDrawer };
    std::optional<QPoint> m_lastPoint{ std::nullopt };

    [[nodiscard]] auto getLastLayer(bool const foreign = false) -> QPixmap&
    {
        return this->getLastLayerIter(foreign).getLastLayer();
    }

    [[nodiscard]] auto getLastLayerIter(bool const foreign = false)
        -> impl::CachedLayers&
    {
        auto it = m_layers2.getUnderlying().rbegin();

        while(it != m_layers2.getUnderlying().rend() &&
              it->foreign() != foreign) {
            ++it;
        }

        if(it->foreign() != foreign) {
            // ERROR
        }

        return *it;
    }

public:
    DrawHistory()
    {
        m_layers2.emplaceBack();
    }
    DrawHistory(DrawHistory const&) = default;
    DrawHistory(DrawHistory&&) = default;
    ~DrawHistory() noexcept = default;

    auto operator=(DrawHistory const&) -> DrawHistory& = default;
    auto operator=(DrawHistory &&) -> DrawHistory& = default;

    auto pushNewLayer(bool const foreign = false) -> void
    {
        /*
        if(m_layers.back().foreign() != foreign) {
            m_layers.emplace_back(foreign);
        }
        else {
            m_layers.back().pushNewLayer();
        }*/

        if(m_layers2.getUnderlying().back().foreign() != foreign) {
            m_layers2.emplaceBack(foreign);
        }
        else {
            m_layers2.getUnderlying().back().pushNewLayer();
        }
        m_lastPoint = std::nullopt;
    }

    auto paintCanvas(QPainter* const painter) -> void
    {
        m_layers2.reduceTo([&painter](impl::CachedLayers& src) -> void {
            src.paintBlock(*painter);
        });
    }

    auto drawAt(QPoint const& pos, QPen const& pen, bool const foreign = false)
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

    auto undo(bool const foreign = false) -> void
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

    auto redo(bool const foreign = false) -> void
    {
        static_cast<void>(this->getLastLayerIter(foreign).redo());
    }
};

} // namespace sk

#endif // !DRAW_HISTORY_HPP
