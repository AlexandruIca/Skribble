#ifndef DRAW_MODE_HPP
#define DRAW_MODE_HPP
#pragma once

///
/// \file
///

#include <QBrush>
#include <QColor>
#include <QPainter>
#include <QPen>

#include <optional>
#include <utility>

#include <QToolBar>

namespace sk {

///
/// \brief Defines methods for drawing with different instruments.
///
/// Currently: \ref BrushMode and \ref PenMode.
///
class DrawMode
{
protected:
    static constexpr int m_defaultWidth = 5;
    static constexpr QColor m_defaultColor{ 0, 0, 0 };

    static int m_width;
    static QColor m_color;

public:
    DrawMode() noexcept = default;
    DrawMode(DrawMode const&) noexcept = default;
    DrawMode(DrawMode&&) noexcept = default;
    virtual ~DrawMode() noexcept = default;

    auto operator=(DrawMode const&) noexcept -> DrawMode& = default;
    auto operator=(DrawMode&&) noexcept -> DrawMode& = default;

    virtual auto draw(QPainter& painter,
                      QPoint const& pos,
                      std::optional<QPoint> const& lastPoint) -> void = 0;

    ///
    /// \brief Gets size of tool.
    ///
    [[nodiscard]] inline auto getWidth() const noexcept -> int
    {
        return m_width;
    }
    ///
    /// \brief Changes size of tool.
    ///
    inline auto setWidth(int const width) noexcept -> void
    {
        m_width = width;
    }

    ///
    /// \brief Gets color of tool.
    ///
    [[nodiscard]] inline auto getColor() const noexcept -> QColor
    {
        return m_color;
    }
    ///
    /// \brief Sets color of tool.
    ///
    inline auto setColor(QColor const& color) noexcept -> void
    {
        m_color = color;
    }

    ///
    /// \brief Tools start out with this color.
    ///
    [[nodiscard]] static constexpr auto getDefaultColor() noexcept -> QColor
    {
        return m_defaultColor;
    }
    ///
    /// \brief Tools start out with this size.
    ///
    [[nodiscard]] static constexpr auto getDefaultWidth() noexcept -> int
    {
        return m_defaultWidth;
    }
};

///
/// \brief Implements drawing with a pen.
///
class PenMode : public DrawMode
{
private:
    QPen m_pen{ QColor{ "black" },
                m_defaultWidth,
                Qt::SolidLine,
                Qt::RoundCap,
                Qt::RoundJoin };

public:
    PenMode() = default;
    PenMode(PenMode const&) noexcept = default;
    PenMode(PenMode&&) noexcept = default;
    ~PenMode() noexcept override = default;

    explicit PenMode(QPen const& pen);
    explicit PenMode(QPen&& pen);

    auto operator=(PenMode const&) noexcept -> PenMode& = default;
    auto operator=(PenMode&&) noexcept -> PenMode& = default;

    auto draw(QPainter& painter,
              QPoint const& pos,
              std::optional<QPoint> const& lastPoint) -> void override;
};

///
/// \brief Implements drawing with a brush.
///
class BrushMode : public DrawMode
{
private:
    QBrush m_brush{ QColor{ "black" }, Qt::BrushStyle::SolidPattern };

public:
    BrushMode() noexcept = default;
    BrushMode(BrushMode const&) noexcept = default;
    BrushMode(BrushMode&&) = default;
    ~BrushMode() noexcept override = default;

    explicit BrushMode(QBrush const& brush);
    explicit BrushMode(QBrush&& brush);

    auto operator=(BrushMode const&) noexcept -> BrushMode& = default;
    auto operator=(BrushMode&&) noexcept -> BrushMode& = default;

    auto draw(QPainter& painter,
              QPoint const& pos,
              std::optional<QPoint> const& lastPos) -> void override;
};

[[nodiscard]] inline auto makeDrawMode(QPen&& pen) -> std::unique_ptr<PenMode>
{
    return std::make_unique<PenMode>(std::forward<QPen>(pen));
}

[[nodiscard]] inline auto makeDrawMode(QBrush&& brush)
    -> std::unique_ptr<BrushMode>
{
    return std::make_unique<BrushMode>(std::forward<QBrush>(brush));
}

[[nodiscard]] inline auto makeDrawMode(PenMode&& pen)
    -> std::unique_ptr<PenMode>
{
    return std::make_unique<PenMode>(std::forward<PenMode>(pen));
}

[[nodiscard]] inline auto makeDrawMode(BrushMode&& brush)
    -> std::unique_ptr<BrushMode>
{
    return std::make_unique<BrushMode>(std::forward<BrushMode>(brush));
}

} // namespace sk

#endif // !DRAW_MODE_HPP
