#include "message_parser.hpp"

#include <ostream>

auto operator<<(std::ostream& os, sk::Operation const op) -> std::ostream&
{
    switch(op) {
    case sk::Operation::DRAW_AT: {
        return os << "DRAW_AT";
    }
    case sk::Operation::MOUSE_RELEASED: {
        return os << "MOUSE_RELEASED";
    }
    case sk::Operation::UNDO: {
        return os << "UNDO";
    }
    case sk::Operation::REDO: {
        return os << "REDO";
    }
    case sk::Operation::CHANGE_COLOR: {
        return os << "CHANGE_COLOR";
    }
    case sk::Operation::CHANGE_WIDTH: {
        return os << "CHANGE_WIDTH";
    }
    case sk::Operation::TO_PEN: {
        return os << "TO_PEN";
    }
    case sk::Operation::TO_BRUSH: {
        return os << "TO_BRUSH";
    }
    default: {
        return os << "UNKOWN_OPERATION";
    }
    }
}

#include "test.hpp"

TEST("[MessageParser] Undo")
{
    auto const& [op, a, b, c, d] = sk::parse("u");

    ASSERT(op == sk::Operation::UNDO);

    static_cast<void>(a);
    static_cast<void>(b);
    static_cast<void>(c);
    static_cast<void>(d);
}

TEST("[MessageParser] Redo")
{
    auto const& [op, a, b, c, d] = sk::parse("r");

    ASSERT(op == sk::Operation::REDO);

    static_cast<void>(a);
    static_cast<void>(b);
    static_cast<void>(c);
    static_cast<void>(d);
}

TEST("[MessageParser] DrawAt")
{
    {
        auto const& [op, a, b, c, d] = sk::parse("d 0 0");

        ASSERT(op == sk::Operation::DRAW_AT);
        ASSERT(a == 0);
        ASSERT(b == 0);

        static_cast<void>(c);
        static_cast<void>(d);
    }
    {
        auto const& [op, a, b, c, d] = sk::parse("d 123 -17");

        ASSERT(op == sk::Operation::DRAW_AT);
        ASSERT(a == 123);
        ASSERT(b == -17);

        static_cast<void>(c);
        static_cast<void>(d);
    }
    {
        auto const& [op, a, b, c, d] = sk::parse("d 330 600");

        ASSERT(op == sk::Operation::DRAW_AT);
        ASSERT(a == 330);
        ASSERT(b == 600);

        static_cast<void>(c);
        static_cast<void>(d);
    }
}

TEST("[MessageParser] MouseReleased")
{
    auto const& [op, a, b, c, d] = sk::parse("m");

    ASSERT(op == sk::Operation::MOUSE_RELEASED);

    static_cast<void>(a);
    static_cast<void>(b);
    static_cast<void>(c);
    static_cast<void>(d);
}

TEST("[MessageParser] ChangeColor")
{
    {
        auto const& [op, a, b, c, d] = sk::parse("c 0 0 0 0");

        ASSERT(op == sk::Operation::CHANGE_COLOR);
        ASSERT(a == 0);
        ASSERT(b == 0);
        ASSERT(c == 0);
        ASSERT(d == 0);
    }
    {
        auto const& [op, a, b, c, d] = sk::parse("c 255 255 255 255");

        ASSERT(op == sk::Operation::CHANGE_COLOR);
        ASSERT(a == 255);
        ASSERT(b == 255);
        ASSERT(c == 255);
        ASSERT(d == 255);
    }
}

TEST("[MessageParser] ChangeWidth")
{
    auto const& [op, a, b, c, d] = sk::parse("w 100");

    ASSERT(op == sk::Operation::CHANGE_WIDTH);
    ASSERT(a == 100);

    static_cast<void>(b);
    static_cast<void>(c);
    static_cast<void>(d);
}

TEST("[MessageParser] ToPen")
{
    auto const& [op, a, b, c, d] = sk::parse("p");

    ASSERT(op == sk::Operation::TO_PEN);

    static_cast<void>(a);
    static_cast<void>(b);
    static_cast<void>(c);
    static_cast<void>(d);
}

TEST("[MessageParser] ToBrush")
{
    auto const& [op, a, b, c, d] = sk::parse("b");

    ASSERT(op == sk::Operation::TO_BRUSH);

    static_cast<void>(a);
    static_cast<void>(b);
    static_cast<void>(c);
    static_cast<void>(d);
}

TEST("[MessageParser] Junk")
{
    auto const& [op, a, b, c, d] =
        sk::parse("sja;lfiqeqowfjiew f;w(*#&*((Jf'\"f}F@_P=");

    ASSERT(op == sk::Operation::NONE);
    ASSERT(a == 0);
    ASSERT(b == 0);
    ASSERT(c == 0);
    ASSERT(d == 0);
}
