#ifndef DUMMY_NETWORK_HPP
#define DUMMY_NETWORK_HPP
#pragma once

///
/// \file
///

#include "abstract_network.hpp"

#include <QTcpSocket>

namespace sk {

///
/// \brief Implement \ref AbstractNetwork for single-user mode.
///
class DummyNetwork final : public AbstractNetwork
{
private:
public:
    DummyNetwork() = default;
    DummyNetwork(DummyNetwork const&) = delete;
    DummyNetwork(DummyNetwork&&) = delete;
    ~DummyNetwork() noexcept override = default;

    auto operator=(DummyNetwork const&) -> DummyNetwork& = delete;
    auto operator=(DummyNetwork&&) noexcept -> DummyNetwork& = delete;

    auto getSocket() -> QTcpSocket* override;
};

} // namespace sk

#endif // !DUMMY_NETWORK_HPP
