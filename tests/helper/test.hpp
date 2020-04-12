#ifndef HELPER_TEST_HPP
#define HELPER_TEST_HPP
#pragma once

#define PASTE_2(a, b) a##b
#define PASTE_1(a, b) PASTE_2(a, b)
#define PASTE(a, b) PASTE_1(a, b)
#define RAND PASTE(tmp_class_name___, __LINE__)
#define RAND2 PASTE(tmp_var_name___, __LINE__)

#include "format.hpp"

#include <atomic>
#include <fstream>
#include <string>
#include <type_traits>
#include <vector>

auto getTestId() -> int;

class TestBase
{
protected:
    std::ofstream m_output{};
    std::string m_name{};
    std::string m_file{};
    int m_line{ 0 };
    int m_testId{ 0 };

    struct Proxy
    {
        std::string str{};

        Proxy() = default;
        ~Proxy() = default;

        template<typename T>
        auto operator+(T const& arg) -> Proxy&
        {
            using TT = std::remove_cv_t<std::remove_reference_t<T>>;
            if constexpr(std::is_same_v<TT, Proxy>) {
                return *this;
            }
            str += sk::format("%1", arg);
            return *this;
        }

        template<typename T>
        auto operator==(T const& other) -> Proxy&
        {
            str += sk::format(" == %1", other);
            return *this;
        }

        template<typename T>
        auto operator!=(T const& other) -> Proxy&
        {
            str += sk::format(" != %1", other);
            return *this;
        }
    };

    TestBase() noexcept = delete;

public:
    TestBase(TestBase const&) = delete;
    TestBase(TestBase&&) noexcept = default;
    virtual ~TestBase() noexcept = default;

    TestBase(std::string const testName,
             std::string const file,
             int const line);

    auto operator=(TestBase const&) -> TestBase& = delete;
    auto operator=(TestBase&&) noexcept -> TestBase& = default;

    virtual auto run(bool& successful) -> void = 0;
};

#define ASSERT(...)                                                            \
    do {                                                                       \
        if(!(__VA_ARGS__)) {                                                   \
            sk::printlnTo(                                                     \
                m_output,                                                      \
                "Assertion failed in %1[%2]:\n\t\t%3\n\tExpanded to:\n\t\t%4", \
                __FILE__,                                                      \
                __LINE__,                                                      \
                #__VA_ARGS__,                                                  \
                (Proxy{} + __VA_ARGS__).str);                                  \
            successful = false;                                                \
            return;                                                            \
        }                                                                      \
    } while(false)

#define TEST(name)                                                             \
    class RAND : public TestBase                                               \
    {                                                                          \
    public:                                                                    \
        RAND()                                                                 \
            : TestBase{ name, __FILE__, __LINE__ }                             \
        {                                                                      \
        }                                                                      \
        ~RAND() noexcept override = default;                                   \
                                                                               \
        auto run(bool& successful) -> void override;                           \
    };                                                                         \
                                                                               \
    static RAND RAND2;                                                         \
                                                                               \
    auto RAND::run(bool& successful)->void

auto getTests() -> std::vector<TestBase*>&;

#ifdef MAIN_EXECUTABLE

#include <cstdlib>
#include <filesystem>

auto getTestId() -> int
{
    static std::atomic<int> id{ 0 };
    id.fetch_add(1);
    return id.load();
}

auto getTests() -> std::vector<TestBase*>&
{
    static std::vector<TestBase*> tests{};
    return tests;
}

TestBase::TestBase(std::string const testName,
                   std::string const file,
                   int const line)
    : m_name{ testName }
    , m_file{ file }
    , m_line{ line }
{
    getTests().push_back(this);
    m_testId = getTestId();
    m_output.open(sk::format("test_%1", m_testId));
    sk::printlnTo(m_output, "%1\n%2\n%3\n", m_name, m_file, m_line);
}

auto startsWith(std::string const& str, std::string const& pattern) -> bool
{
    if(pattern.length() > str.length()) {
        return false;
    }

    for(std::size_t i = 0; i < pattern.length(); ++i) {
        if(str[i] != pattern[i]) {
            return false;
        }
    }

    return true;
}

auto main(int, char*[]) noexcept -> int
{
    int ret = EXIT_SUCCESS;

    for(auto& test : getTests()) {
        bool successful{ true };

        test->run(successful);

        if(!successful) {
            ret = EXIT_FAILURE;
        }
    }

    namespace fs = std::filesystem;

    auto currentDir = fs::path{ "." };
    fs::directory_iterator dirIter{};

    sk::println("Report:");
    for(fs::directory_iterator file(currentDir); file != dirIter; ++file) {
        if(startsWith(file->path().string(), "./test_")) {
            std::ifstream f{ file->path().string() };
            std::string line{};

            std::getline(f, line);
            sk::print("%1 defined at ", line);
            std::getline(f, line);
            sk::print("%1", line);
            std::getline(f, line);
            sk::println("[%1]:", line);

            bool allPassed = true;

            while(std::getline(f, line)) {
                if(line.empty()) {
                    continue;
                }
                allPassed = false;
                sk::println("%1", line);
            }

            if(allPassed) {
                sk::println("\t%1", "All tests here passed!");
            }
            else {
                sk::println("");
            }
        }
    }

    return ret;
}

#endif

#endif // !HELPER_TEST_HPP
