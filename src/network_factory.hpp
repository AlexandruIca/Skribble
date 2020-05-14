#ifndef NETWORK_FACTORY_HPP
#define NETWORK_FACTORY_HPP
#pragma once

///
/// \file
///

#include "client.hpp"
#include "dummy_network.hpp"
#include "network_config.hpp"
#include "server.hpp"

#include <memory>

namespace sk {

///
/// \brief Helper implementing Factory design pattern.
///
class NetworkFactory
{
public:
    static auto create(sk::NetworkModes const mode)
        -> std::unique_ptr<AbstractNetwork>;
};

} // namespace sk

#endif // !NETWORK_FACTORY_HPP
