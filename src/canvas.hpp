#ifndef CANVAS_HPP
#define CANVAS_HPP
#pragma once

#include "draw_history.hpp"
#include "draw_mode.hpp"

#include <QPainter>
#include <QPoint>
#include <QQuickPaintedItem>

#include <memory>
#include <vector>

namespace sk {

class Canvas : public QQuickPaintedItem
{
private:
    Q_OBJECT

    DrawHistory m_history{};
    std::unique_ptr<DrawMode> m_drawMode{};
    bool m_foreign{ false };

public:
    explicit Canvas(QQuickPaintedItem* parent = nullptr);
    Canvas(Canvas const&) = delete;
    Canvas(Canvas&&) = delete;
    ~Canvas() noexcept override = default;

    auto operator=(Canvas const&) = delete;
    auto operator=(Canvas&&) = delete;

    auto paint(QPainter* painter) -> void override;

public slots:
    void mousePositionChanged(QPoint const& pos);
    void mouseReleased();
    void undo();
    void redo();
    void toggleForeign();
};

} // namespace sk

#endif // !CANVAS_HPP
