#ifndef DUMMY_NETWORK_HPP
#define DUMMY_NETWORK_HPP
#pragma once

#include "abstract_network.hpp"

namespace sk {

class DummyNetwork final : public AbstractNetwork
{
private:
public:
    DummyNetwork() noexcept = default;
    DummyNetwork(DummyNetwork const&) = delete;
    DummyNetwork(DummyNetwork&&) noexcept = delete;
    ~DummyNetwork() noexcept override = default;

    auto operator=(DummyNetwork const&) -> DummyNetwork& = delete;
    auto operator=(DummyNetwork&&) noexcept -> DummyNetwork& = delete;

    auto doSomething() -> void override;
};

} // namespace sk

#endif // !DUMMY_NETWORK_HPP
