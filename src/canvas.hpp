#ifndef CANVAS_HPP
#define CANVAS_HPP
#pragma once

#include <QObject>
#include <QPoint>

namespace sk {

class Canvas : public QObject
{
private:
    Q_OBJECT

public:
    explicit Canvas(QObject* parent = nullptr);
    Canvas(Canvas const&) = delete;
    Canvas(Canvas&&) = delete;
    ~Canvas() noexcept override = default;

    auto operator=(Canvas const&) = delete;
    auto operator=(Canvas&&) = delete;

public slots:
    void mousePositionChanged(QPoint const& pos);
};

} // namespace sk

#endif // !CANVAS_HPP
