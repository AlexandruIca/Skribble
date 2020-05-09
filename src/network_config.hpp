#ifndef NETWORK_CONFIG_HPP
#define NETWORK_CONFIG_HPP
#pragma once

#include <QString>

namespace sk {

inline constexpr int port = 7654;
inline QString host_ip{};

enum class NetworkModes
{
    SINGLE_USER,
    CLIENT,
    SERVER
};

inline NetworkModes networkMode = NetworkModes::SINGLE_USER;

} // namespace sk

#endif // !NETWORK_CONFIG_HPP
