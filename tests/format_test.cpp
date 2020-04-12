#include "format.hpp"
#include "test.hpp"

#include <thread>

TEST("[Format] Empty")
{
    auto str = sk::format("");
    ASSERT(str.empty());

    str = sk::format("%1", "");
    ASSERT(str.empty());

    str = sk::format("%1%2%3", "", "", "");
    ASSERT(str.empty());
}

TEST("[Format] No args")
{
    auto str = sk::format("Hello");
    ASSERT(str == "Hello");
}

TEST("[Format] Basic")
{
    using namespace std::string_literals;

    auto str = sk::format("%1%2%3 %2 %1", 1, 2, 3);
    ASSERT(str == "123 2 1");

    str = sk::format("%1 + %2 == %3", "2"s, "3", "5"s);
    ASSERT(str == "2 + 3 == 5");
}

class Dummy
{
private:
    int m_num{ 0 };

public:
    Dummy() noexcept = default;
    Dummy(Dummy const&) noexcept = default;
    Dummy(Dummy&&) noexcept = default;
    ~Dummy() noexcept = default;

    explicit Dummy(int const num)
        : m_num{ num }
    {
    }

    auto operator=(Dummy const&) noexcept -> Dummy& = default;
    auto operator=(Dummy&&) noexcept -> Dummy& = default;

    friend auto operator<<(std::ostream& os, Dummy const& d) -> std::ostream&
    {
        os << '(' << d.m_num << ')';
        return os;
    }
};

TEST("[Format] Custom type")
{
    auto str = sk::format("%1", Dummy{ 42 });
    ASSERT(str == "(42)");
}

TEST("[Format] Different threads")
{
    std::string s1{};
    std::string s2{};

    auto t1 = std::thread([&s1] { s1 = sk::format("%1", "Hello"); });
    auto t2 = std::thread([&s2] { s2 = sk::format("%1", "World"); });

    t1.join();
    t2.join();

    ASSERT(s1 == "Hello");
    ASSERT(s2 == "World");
}
