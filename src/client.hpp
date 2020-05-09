#ifndef CLIENT_HPP
#define CLIENT_HPP
#pragma once

#include "abstract_network.hpp"

#include <QString>
#include <QTcpSocket>

#include <memory>

namespace sk {

class Client final : public AbstractNetwork
{
private:
    std::unique_ptr<QTcpSocket> m_server{ nullptr };

public:
    explicit Client(QString const& ip);
    Client(Client const&) = delete;
    Client(Client&&) = delete;
    ~Client() noexcept override;

    auto operator=(Client const&) = delete;
    auto operator=(Client&&) = delete;

    auto doSomething() -> void override;
};

} // namespace sk

#endif // !CLIENT_HPP
