#ifndef NETWORK_CONFIG_HPP
#define NETWORK_CONFIG_HPP
#pragma once

namespace sk {

inline constexpr int port = 7654;

enum class NetworkModes
{
    SINGLE_USER,
    CLIENT,
    SERVER
};

inline NetworkModes networkMode = NetworkModes::SINGLE_USER;

} // namespace sk

#endif // !NETWORK_CONFIG_HPP
