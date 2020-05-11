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
#include <cstdlib>
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
        Proxy(Proxy const&) = delete;
        Proxy(Proxy&&) = delete;
        ~Proxy() = default;

        auto operator=(Proxy const&) = delete;
        auto operator=(Proxy&&) = delete;

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

public:
    TestBase() = delete;
    TestBase(TestBase const&) = delete;
    TestBase(TestBase&&) noexcept = default;
    virtual ~TestBase() noexcept = default;

    TestBase(std::string testName, std::string file, int const line);

    auto operator=(TestBase const&) -> TestBase& = delete;
    auto operator=(TestBase&&) noexcept -> TestBase& = default;

    virtual auto run(std::atomic<int>& successful) -> void = 0;
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
            successful.store(EXIT_FAILURE);                                    \
            return;                                                            \
        }                                                                      \
    } while(false)

#define TEST(name)                                                             \
    namespace {                                                                \
    class RAND : public TestBase                                               \
    {                                                                          \
    public:                                                                    \
        RAND()                                                                 \
            : TestBase{ name, __FILE__, __LINE__ }                             \
        {                                                                      \
        }                                                                      \
        RAND(RAND const&) = delete;                                            \
        RAND(RAND&&) = delete;                                                 \
        ~RAND() noexcept override = default;                                   \
                                                                               \
        auto operator=(RAND const&) -> RAND& = delete;                         \
        auto operator=(RAND &&) -> RAND& = delete;                             \
                                                                               \
        auto run(std::atomic<int>& successful) -> void override;               \
    };                                                                         \
                                                                               \
    static RAND RAND2;                                                         \
    }                                                                          \
                                                                               \
    auto RAND::run(std::atomic<int>& successful)->void

auto getTests() -> std::vector<TestBase*>&;

//#define MAIN_EXECUTABLE
#ifdef MAIN_EXECUTABLE

#include <atomic>
#include <condition_variable>
#include <cstdlib>
#include <filesystem>
#include <functional>
#include <future>
#include <memory>
#include <queue>
#include <stdexcept>
#include <thread>
#include <utility>

class ThreadPool
{
private:
    std::vector<std::thread> m_workers{};
    std::queue<std::function<void()>> m_tasks{};
    std::mutex m_mutex{};
    std::condition_variable m_cv{};

    bool m_stop{ false };

public:
    ThreadPool(ThreadPool const&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ~ThreadPool() noexcept
    {
        {
            std::unique_lock<std::mutex> lock{ m_mutex };
            m_stop = true;
        }

        m_cv.notify_all();

        for(auto& thread : m_workers) {
            thread.join();
        }
    }

    explicit ThreadPool(
        std::size_t const numThreads = std::thread::hardware_concurrency())
    {
        m_workers.reserve(numThreads);

        for(std::size_t i = 0; i < numThreads; ++i) {
            m_workers.emplace_back([this]() -> void {
                for(;;) {
                    std::function<void()> task{};
                    {
                        std::unique_lock<std::mutex> lock{ m_mutex };
                        m_cv.wait(lock, [this] {
                            return m_stop || !m_tasks.empty();
                        });

                        if(m_stop && m_tasks.empty()) {
                            return;
                        }

                        task = std::move(m_tasks.front());
                        m_tasks.pop();
                    }

                    task();
                }
            });
        }
    }

    auto operator=(ThreadPool const&) -> ThreadPool& = delete;
    auto operator=(ThreadPool &&) -> ThreadPool& = delete;

    template<typename F, typename... Args>
    auto push(F&& f, Args&&... args)
        -> std::future<std::invoke_result_t<F, Args...>>
    {
        using T = std::invoke_result_t<F, Args...>;

        auto task = std::make_shared<std::packaged_task<T()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<T> result = task->get_future();
        {
            std::unique_lock<std::mutex> lock{ m_mutex };

            if(m_stop) {
                throw std::runtime_error{
                    "Attempted to push a thread to a terminated thread pool!"
                };
            }

            m_tasks.emplace([task] { (*task)(); });
        }

        m_cv.notify_one();
        return result;
    }
};

auto getTestId() -> int
{
    static std::atomic<int> id{ 0 };
    id.fetch_add(1);
    return id.load();
}

class TestSingleton
{
private:
    TestSingleton() noexcept = default;

public:
    TestSingleton(TestSingleton const&) = delete;
    TestSingleton(TestSingleton&&) = delete;
    ~TestSingleton() noexcept = default;

    auto operator=(TestSingleton const&) -> TestSingleton& = delete;
    auto operator=(TestSingleton&&) noexcept -> TestSingleton& = delete;

    static auto getInstance() -> std::vector<TestBase*>&
    {
        static std::vector<TestBase*> tests{};
        return tests;
    }
};

auto operator+=(std::vector<TestBase*> tests, TestBase* test) -> void
{
    tests.push_back(test);
}

auto getTests() -> std::vector<TestBase*>&
{
    return TestSingleton::getInstance();
}

TestBase::TestBase(std::string testName, std::string file, int const line)
    : m_name{ std::move(testName) }
    , m_file{ std::move(file) }
    , m_line{ line }
{
    getTests() += this;
    m_testId = getTestId();
    m_output.open(sk::format("test_%1", m_testId));
    sk::printlnTo(m_output, "%1\n%2\n%3", m_name, m_file, m_line);
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
    std::atomic<int> successful{ EXIT_SUCCESS };
    {
        ThreadPool pool{};
        std::vector<std::future<void>> workers{};

        for(auto& test : getTests()) {

            try {
                workers.emplace_back(
                    pool.push([&]() -> void { test->run(successful); }));
            }
            catch(std::exception const& e) {
                sk::println("%1", e.what());
            }
            catch(...) {
                sk::println("Uncaught exception!");
            }
        }

        for(auto& worker : workers) {
            worker.get();
        }
    }
    ret = successful.load();

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
