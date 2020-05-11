#ifndef DRAW_HISTORY_HPP
#define DRAW_HISTORY_HPP
#pragma once

#include "cached_resource.hpp"
#include "canvas_config.hpp"
#include "draw_mode.hpp"
#include "fixed_cached_resource.hpp"

#include <QColor>
#include <QPainter>
#include <QPixmap>
#include <QPoint>

#include <deque>
#include <memory>
#include <optional>
#include <vector>

namespace sk::impl {

class CachedLayers
{
private:
    bool const m_foreign{ false };

    static auto PixmapDrawer(QPixmap& dest, QPixmap& src) -> void;
    static auto PixmapInit(QPixmap& pixmap) noexcept -> void;

    sk::FCachedResource<QPixmap> m_layers{ &PixmapDrawer, &PixmapInit };

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

    static constexpr auto getCanvasRect() -> QRect
    {
        return m_canvasRect;
    }

    auto pushNewLayer() -> void;
    auto paintBlock(QPainter& painter) -> void;
    [[nodiscard]] auto getLastLayer() noexcept -> QPixmap&;
    [[nodiscard]] auto getLastLayer() const noexcept -> QPixmap const&;

    [[nodiscard]] constexpr auto foreign() const noexcept -> bool
    {
        return m_foreign;
    }

    [[nodiscard]] inline auto underUndo() const noexcept -> bool
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

    std::list<impl::CachedLayers> m_layers{};
    sk::CachedResource<impl::CachedLayers> m_cache{};

    std::optional<QPoint> m_lastPoint{ std::nullopt };
    std::optional<QPoint> m_lastExternalPoint{ std::nullopt };

    bool m_drawingLocally{ false };
    bool m_drawingExternally{ false };

    auto handleExternal(QPoint const& pos, DrawMode& mode) -> void;
    auto handleLocal(QPoint const& pos, DrawMode& mode) -> void;
    auto popFirst(bool const foreign) -> void;

public:
    DrawHistory();
    DrawHistory(DrawHistory const&) = delete;
    DrawHistory(DrawHistory&&) = default;
    ~DrawHistory() noexcept = default;

    auto operator=(DrawHistory const&) -> DrawHistory& = delete;
    auto operator=(DrawHistory &&) -> DrawHistory& = delete;

    auto pushNewLayer(bool const foreign = false) -> void;
    auto paintCanvas(QPainter* const painter) -> void;

    auto drawAt(QPoint const& pos, DrawMode& mode, bool const foreign = false)
        -> void;
    auto undo(bool const foreign = false) -> void;
    auto redo(bool const foreign = false) -> void;
};

} // namespace sk

#endif // !DRAW_HISTORY_HPP
