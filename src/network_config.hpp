#ifndef NETWORK_CONFIG_HPP
#define NETWORK_CONFIG_HPP
#pragma once

///
/// \file
///

#include <QString>

namespace sk {

///
/// \brief Communication port used by Skribble clients/servers.
///
inline constexpr int port = 7654;
///
/// \brief Stores the ip to connect to.
///
/// \note Used only by clients.
///
inline QString host_ip{};

///
/// \brief Connection type.
///
enum class NetworkModes
{
    SINGLE_USER,
    CLIENT,
    SERVER
};

///
/// \brief Stores the connection mode the user has requested.
///
inline NetworkModes networkMode = NetworkModes::SINGLE_USER;

} // namespace sk

#endif // !NETWORK_CONFIG_HPP
