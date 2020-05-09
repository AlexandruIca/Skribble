#ifndef ABSTRACT_NETWORK_HPP
#define ABSTRACT_NETWORK_HPP
#pragma once

#include <QObject>
#include <QString>

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

signals:
    void receivedMessage(QString const& msg);
};

} // namespace sk

#endif // !ABSTRACT_NETWORK_HPP
