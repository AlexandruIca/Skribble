#ifndef DRAW_HISTORY_HPP
#define DRAW_HISTORY_HPP
#pragma once

#include "cached_resource.hpp"
#include "canvas_config.hpp"

#include <QColor>
#include <QPainter>
#include <QPixmap>
#include <QPoint>

#include <deque>

namespace sk::impl {

class CachedLayers
{
private:
    bool const m_foreign{ false };

    static auto PixmapDrawer(QPixmap& dest, QPixmap& src) -> void;

    sk::CachedResource<QPixmap> m_layers{ &PixmapDrawer };

    static constexpr QColor m_transparent{ 0, 0, 0, 0 };
    static constexpr QRect m_canvasRect{
        0, 0, sk::config::width, sk::config::height
    };

public:
    explicit CachedLayers(bool const foreign = false);
    CachedLayers(CachedLayers const&) = default;
    CachedLayers(CachedLayers&&) = default;
    ~CachedLayers() noexcept = default;

    auto operator=(CachedLayers const&) -> CachedLayers& = delete;
    auto operator=(CachedLayers &&) -> CachedLayers& = delete;

    auto pushNewLayer() -> void;
    auto paintBlock(QPainter& painter) -> void;
    [[nodiscard]] auto getLastLayer() noexcept -> QPixmap&;
    [[nodiscard]] auto getLastLayer() const noexcept -> QPixmap const&;

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
    [[nodiscard]] auto undo() -> bool;
    auto redo() -> bool;
};

} // namespace sk::impl

namespace sk {

class DrawHistory
{
private:
    static auto CachedDrawer(impl::CachedLayers& dest, impl::CachedLayers& src)
        -> void;

    struct Traits
    {
        using ContainerType = std::deque<impl::CachedLayers>;
        static constexpr int cacheGap = 3;
        static constexpr int maxCount = 10;
    };

    sk::CachedResource<impl::CachedLayers, Traits> m_layers{ &CachedDrawer };
    std::optional<QPoint> m_lastPoint{ std::nullopt };

    [[nodiscard]] auto getLastLayer(bool const foreign = false) -> QPixmap&;
    [[nodiscard]] auto getLastLayerIter(bool const foreign = false)
        -> impl::CachedLayers&;

public:
    DrawHistory();
    DrawHistory(DrawHistory const&) = default;
    DrawHistory(DrawHistory&&) = default;
    ~DrawHistory() noexcept = default;

    auto operator=(DrawHistory const&) -> DrawHistory& = default;
    auto operator=(DrawHistory &&) -> DrawHistory& = default;

    auto pushNewLayer(bool const foreign = false) -> void;
    auto paintCanvas(QPainter* const painter) -> void;

    auto drawAt(QPoint const& pos, QPen const& pen, bool const foreign = false)
        -> void;
    auto undo(bool const foreign = false) -> void;
    auto redo(bool const foreign = false) -> void;
};

} // namespace sk

#endif // !DRAW_HISTORY_HPP
