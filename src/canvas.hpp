#ifndef CANVAS_HPP
#define CANVAS_HPP
#pragma once

#include <QPainter>
#include <QPoint>
#include <QQuickPaintedItem>

#include <vector>

namespace sk {

class Canvas : public QQuickPaintedItem
{
private:
    Q_OBJECT

    std::vector<QPoint> m_points{};

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
};

} // namespace sk

#endif // !CANVAS_HPP
