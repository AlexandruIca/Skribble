#include "network_factory.hpp"
#include "network_config.hpp"

namespace sk {

auto NetworkFactory::create(sk::NetworkModes const mode)
    -> std::unique_ptr<AbstractNetwork>
{
    switch(mode) {
    case sk::NetworkModes::SINGLE_USER: {
        return std::make_unique<DummyNetwork>();
    }
    case sk::NetworkModes::CLIENT: {
        return std::make_unique<Client>(sk::host_ip);
    }
    case sk::NetworkModes::SERVER: {
        return std::make_unique<Server>();
    }
    default: {
        return nullptr;
    }
    }
}

} // namespace sk
