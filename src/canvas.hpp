#ifndef CANVAS_HPP
#define CANVAS_HPP
#pragma once

///
/// \file
///

#include "abstract_network.hpp"
#include "draw_history.hpp"
#include "draw_mode.hpp"

#include <QPainter>
#include <QPoint>
#include <QQuickPaintedItem>
#include <QTcpServer>
#include <QTcpSocket>

#include <memory>
#include <vector>

///
/// All Skribble functionality is implemented in this namespace.
///
namespace sk {

///
/// \brief The class where all the magic happens.
///
/// This contains all behaviour related
/// to painting itself.
///
/// All of \ref mousePositionChanged, \ref mouseReleased, \ref undo, \ref redo,
/// \ref changeColor, \ref changeWidth are signals that other than modifying
/// the local canvas accordingly, they also send the event over the network to
/// the other user.
///
class Canvas : public QQuickPaintedItem
{
private:
    Q_OBJECT

    DrawHistory m_history{};
    std::unique_ptr<DrawMode> m_drawMode{};
    std::unique_ptr<AbstractNetwork> m_network{ nullptr };

    QColor m_foreignColor{ DrawMode::getDefaultColor() };
    int m_foreignWidth{ DrawMode::getDefaultWidth() };

public:
    explicit Canvas(QQuickPaintedItem* parent = nullptr);
    Canvas(Canvas const&) = delete;
    Canvas(Canvas&&) = delete;
    ~Canvas() noexcept override = default;

    auto operator=(Canvas const&) = delete;
    auto operator=(Canvas&&) = delete;

    ///
    /// Draws the canvas.
    ///
    auto paint(QPainter* painter) -> void override;

public slots:
    ///
    /// This triggers a paint event on the canvas.
    ///
    void mousePositionChanged(QPoint const& pos);
    ///
    /// This triggers 'creating a new layer' event.
    ///
    void mouseReleased();
    ///
    /// This goes back to the previously stored layer.
    ///
    void undo();
    ///
    /// Opposite of \ref undo.
    ///
    void redo();
    ///
    /// Called when a message through the network is received. Also handles
    /// the actual events that were received.
    ///
    void onReceivedMessage(QString const& msg);
    ///
    /// Changes the color of the pen/brush to draw with.
    ///
    void changeColor(QColor const& color);
    ///
    /// Changes the size of the brush/pen.
    ///
    void changeWidth(const int width);
};

} // namespace sk

#endif // !CANVAS_HPP
