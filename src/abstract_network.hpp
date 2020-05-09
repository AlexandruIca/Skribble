#ifndef ABSTRACT_NETWORK_HPP
#define ABSTRACT_NETWORK_HPP
#pragma once

#include <QColor>
#include <QObject>
#include <QPoint>
#include <QString>
#include <QTcpSocket>

namespace sk {

class AbstractNetwork : public QObject
{
    Q_OBJECT

public:
    explicit AbstractNetwork(QObject* parent = nullptr);
    AbstractNetwork(AbstractNetwork const&) = delete;
    AbstractNetwork(AbstractNetwork&&) noexcept = delete;
    ~AbstractNetwork() noexcept override = default;

    auto operator=(AbstractNetwork const&) -> AbstractNetwork& = delete;
    auto operator=(AbstractNetwork&&) noexcept -> AbstractNetwork& = delete;

    virtual auto doSomething() -> void = 0;
    virtual auto getSocket() -> QTcpSocket* = 0;

    auto sendDrawAt(QPoint const& pos) -> void;
    auto sendMouseReleased() -> void;
    auto sendUndo() -> void;
    auto sendRedo() -> void;
    auto sendChangeColor(QColor const& color) -> void;
    auto sendChangeWidth(int const width) -> void;
    auto sendToPen() -> void;
    auto sendToBrush() -> void;

signals:
    void receivedMessage(QString const& msg);
};

} // namespace sk

#endif // !ABSTRACT_NETWORK_HPP
