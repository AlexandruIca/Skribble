#ifndef MESSAGE_PARSER_HPP
#define MESSAGE_PARSER_HPP
#pragma once

#include <string>
#include <tuple>

namespace sk {

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

[[nodiscard]] auto parse(std::string const& msg)
    -> std::tuple<Operation, int, int, int, int>;

} // namespace sk

#endif // !MESSAGE_PARSER_HPP
