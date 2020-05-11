#include "message_parser.hpp"

#include <string>
#include <vector>

namespace sk {

namespace {

auto split(std::string const& str, std::string const& pattern)
    -> std::vector<std::string>
{
    std::vector<std::string> result{};
    std::string tmp{};

    for(char const ch : str) {
        if(pattern.find(ch) != std::string::npos) {
            if(!tmp.empty()) {
                result.push_back(tmp);
                tmp.clear();
            }
        }
        else {
            tmp.push_back(ch);
        }
    }

    if(!tmp.empty()) {
        result.push_back(tmp);
    }

    return result;
}

} // namespace

auto parse(std::string const& msg) -> std::tuple<Operation, int, int, int, int>
{
    if(msg.empty()) {
        return { Operation::NONE, 0, 0, 0, 0 };
    }

    auto words = split(msg, " ");

    switch(words[0][0]) {
    case 'd': {
        return {
            Operation::DRAW_AT, std::stoi(words[1]), std::stoi(words[2]), 0, 0
        };
    }
    case 'm': {
        return { Operation::MOUSE_RELEASED, 0, 0, 0, 0 };
    }
    case 'u': {
        return { Operation::UNDO, 0, 0, 0, 0 };
    }
    case 'r': {
        return { Operation::REDO, 0, 0, 0, 0 };
    }
    case 'c': {
        return { Operation::CHANGE_COLOR,
                 std::stoi(words[1]),
                 std::stoi(words[2]),
                 std::stoi(words[3]),
                 std::stoi(words[4]) };
    }
    case 'w': {
        return { Operation::CHANGE_WIDTH, std::stoi(words[1]), 0, 0, 0 };
    }
    case 'p': {
        return { Operation::TO_PEN, 0, 0, 0, 0 };
    }
    case 'b': {
        return { Operation::TO_BRUSH, 0, 0, 0, 0 };
    }
    default: {
        return { Operation::NONE, 0, 0, 0, 0 };
    }
    }
}

} // namespace sk
