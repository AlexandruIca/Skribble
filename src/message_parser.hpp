#ifndef MESSAGE_PARSER_HPP
#define MESSAGE_PARSER_HPP
#pragma once

///
/// \file
///

#include <string>
#include <tuple>

namespace sk {

///
/// \brief Possible kinds of messages to be sent over the network.
///
enum class Operation
{
    UNDO,
    REDO,
    DRAW_AT,
    MOUSE_RELEASED,
    CHANGE_COLOR,
    CHANGE_WIDTH,
    TO_BRUSH,
    TO_PEN,
    NONE
};

///
/// \brief Helper to parse messages received over the network.
///
/// The structure of a message is `kind a b c d`, where:
/// * `kind` cand be one of 'u' - undo, 'r' - redo, 'd' - draw_at, 'm' - mouse
///   released, 'c' - change color, 'w' - change width, 'b' - to brush, 'p' -
///   to pen
/// * `a` is an integer used by 'w', 'd', 'c'
/// * `b` is an integer used by 'd', 'c'
/// * `c` and `d` are only used by 'c'
///
/// \returns { kind, a, b, c, d }
///
[[nodiscard]] auto parse(std::string const& msg)
    -> std::tuple<Operation, int, int, int, int>;

} // namespace sk

#endif // !MESSAGE_PARSER_HPP
