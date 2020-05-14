#ifndef SERVER_HPP
#define SERVER_HPP
#pragma once

///
/// \file
///

#include "abstract_network.hpp"

#include <QTcpServer>
#include <QTcpSocket>

#include <memory>

namespace sk {

///
/// \brief Implements \ref AbstractNetwork for a server.
///
class Server final : public AbstractNetwork
{
private:
    std::unique_ptr<QTcpServer> m_server{ nullptr };
    QTcpSocket* m_socket{ nullptr };

public:
    explicit Server();
    Server(Server const&) = delete;
    Server(Server&&) = delete;
    ~Server() noexcept override;

    auto operator=(Server const&) -> Server& = delete;
    auto operator=(Server &&) -> Server& = delete;

    auto getSocket() -> QTcpSocket* override;
};

} // namespace sk

#endif // !SERVER_HPP
