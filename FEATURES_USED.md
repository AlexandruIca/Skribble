## STL
* `std::deque` used by default in [CachedResource][cached_resource]
* `std::list` used by default in [FCachedResource][fcached_resource]
* `std::optional` used by [DrawHistory][draw_history_hpp] to remember where the last drawn point was
* C++17's `emplace_back` returning a reference to the emplaced object used in [CachedLayers][draw_history_hpp]
* Helpers from `<iterator>` used in [CachedResource][cached_resource] and [FCachedResource][fcached_resource]:
    - `std::advance`
    - `std::distance`
* C++17's `std::is_same_v` type trait used in [Tests][test_helper]
* C++14's `_v` type traits used in [Tests][test_helper]
* Multithreading utilities used in [Tests][test_helper] to run tests in parallel:
    - `std::atomic`
    - `std::thread`
    - `std::condition_variable`
    - `std::mutex`
    - `std::unique_lock`
    - `std::future`
    - `std::packaged_task`
* C++17's `std::filesystem` used to iterate over test reports in [Tests][test_helper]
* C++17's `std::invoke` and `std::invoke_result_t` used to return the correct type in [ThreadPool::push][test_helper] and to pass correct function call to thread pool workers
* `std::unique_ptr` used in:
    - [Canvas][canvas_hpp]
    - [Server][server_hpp]
    - [Client][client_hpp]
* `std::shared_ptr` used in [ThreadPool::push][test_helper] to correctly store the result of the given function

## Language
* Lambdas used for `reduceTo` in [DrawHistory][draw_history_cpp]
* Trailing return types used basically everywhere
* Init statments in `if` used in:
    - [DrawHistory][draw_history_cpp] in `drawAt`
    - [AbstractNetwork][abstract_network_cpp] in `send*` functions
* C++17's `[[nodiscard]]` used almost everywhere
* `explicit` used for most constructors
* `noexcept` move constructors and assignment operators whenever appropiate
* C++17's nested namespace definition used for [CachedLayers][draw_history_hpp]
* Anonymous namespaces used to define tests that do not conflict with other tests defined in different files but at the same line in [Tests][test_helper]
* `constexpr` used for configuration in:
    - [CachedLayers][draw_history_hpp]
    - [config][canvas_config]
    - [network\_config][network_config]
    - [CachedResource][cached_resource]
    - [FCachedResource][fcached_resource]
* C++17's `inline` variables used in:
    - [config][canvas_config]
    - [network\_config][network_config]
* Templates used in:
    - [CachedResource][cached_resource]
    - [FCachedResource][fcached_resource]
    - [network\_config][network_config]
* `static_assert` used in:
    - [CachedResource][cached_resource]
    - [FCachedResource][fcached_resource]
* Variadic templates + fold expressions + perfect forwarding used to implement:
    - `emplaceBack` in [CachedResource][cached_resource] and [FCachedResource][fcached_resource]
    - `sk::format` in [Format][format]
    - `ThreadPool::push` in [Tests][test_helper]
* Abstract classes:
    - `TestBase` in [Tests][test_helper]
    - `AbstractNetwork` in [abstract\_network.hpp][abstract_network_hpp]
* Exceptions used in [Tests][test_helper] to check if a test is being pushed after termination(extends `std::exception`)

## RTTI
* Didn't really use it ourselves, but if you count Qt it's used almost everywhere to make use of signals and slots, mostly in [Canvas][canvas_hpp] and [AbstractNetwork][abstract_network_hpp]

## Operator Overloading
* Couldn't find much use for it, we have an overloaded `+=` to add new tests in [TestBase][test_helper]

## Networking(Socket Programming)
* Made use of Qt's helpers for TCP sockets/servers:
    - `QTcpServer` in [Server][server_hpp]
    - `QTcpSocket` in [Server][server_hpp] and [Client][client_hpp]

Qt's signals and slots allowed for a very clean implementation.

## Design Patterns
* Client/Server: in [Client][client_hpp] and [Server][server_hpp] to create and maintain a peer-to-peer connection
* Factory: in [NetworkFactory][network_factory]
* Singleton: in [TestSingleton][test_helper] to manage test creation globally
* Command Pattern: in [CachedResource][cached_resource] and [FCachedResource][fcached_resource] used to maintain history of insertions for undo/redo. Each modification to the canvas is stored as a separate image in a list(until the limit is reached, by default 100 images AKA 100 undos at most)
* Observer Pattern: in [Canvas][canvas_hpp] and [AbstractNetwork][abstract_network_hpp] if you count Qt's signals/slots

## Extra
* QML used for declaring the user interface. All zooming/scrolling and all controls are implemented in QML, thus the UI and implementation are independent
* Clang-Tidy used to check for mistakes/improvements in code using [scripts/run-clang-tidy.py][script_run_clang_tidy]
* Clang-Format used to format the code (and verify that the code is formatted according to `.clang-format` using [scripts/run-clang-format.py][script_run_clang_format])
* Clazy used to check for common Qt mistakes/improvements in code using [scripts/run-clazy.sh][script_run_clazy]
* Modern CMake used to describe how the code should be built, making use of `EXPORT_COMPILE_COMMANDS` to get a compilation database for other tools to use
* `cmake-format` is used to format all CMake files according to `.cmake-format.yaml` using the [scripts/run-cmake-format.sh][script_run_cmake_format]
* Addres, Leak, Undefined sanitizers are used to check for memory leaks and undefined behavior in code(found some nasty bugs with those! - for example incrementing invalidated iterators in [FCachedResource][fcached_resource])
* Code coverage can be generated by specifying `-DENABLE_COVERAGE=ON` when calling `cmake`
* All tests defined in [tests/][tests_folder] are run with `ctest` by having `-DENABLE_TESTS=ON`
* Code is continuously checked/run with above tools/scripts in Github Actions on every push/pr
* Code is also deployed on windows with Github Actions and you can actually download binaries for windows directly in [Releases][releases]

[cached_resource]: https://github.com/AlexandruIca/Skribble/blob/develop/src/cached_resource.hpp
[fcached_resource]: https://github.com/AlexandruIca/Skribble/blob/develop/src/fixed_cached_resource.hpp
[draw_history_hpp]: https://github.com/AlexandruIca/Skribble/blob/develop/src/draw_history.hpp
[draw_history_cpp]: https://github.com/AlexandruIca/Skribble/blob/develop/src/draw_history.cpp
[test_helper]: https://github.com/AlexandruIca/Skribble/blob/develop/tests/helper/test.hpp
[canvas_config]: https://github.com/AlexandruIca/Skribble/blob/develop/src/canvas_config.hpp
[network_config]: https://github.com/AlexandruIca/Skribble/blob/develop/src/network_config.hpp
[network_factory]: https://github.com/AlexandruIca/Skribble/blob/develop/src/network_factory.cpp
[format]: https://github.com/AlexandruIca/Skribble/blob/develop/src/format.hpp
[canvas_hpp]: https://github.com/AlexandruIca/Skribble/blob/develop/src/canvas.hpp
[script_run_clang_tidy]: https://github.com/AlexandruIca/Skribble/blob/develop/scripts/run-clang-tidy.py
[script_run_clang_format]: https://github.com/AlexandruIca/Skribble/blob/develop/scripts/run-clang-format.py
[script_run_clazy]: https://github.com/AlexandruIca/Skribble/blob/develop/scripts/run-clazy.sh
[script_run_cmake_format]: https://github.com/AlexandruIca/Skribble/blob/develop/scripts/run-cmake-format.sh
[tests_folder]: https://github.com/AlexandruIca/Skribble/blob/develop/tests
[abstract_network_hpp]: https://github.com/AlexandruIca/Skribble/blob/master/src/abstract_network.hpp
[abstract_network_cpp]: https://github.com/AlexandruIca/Skribble/blob/master/src/abstract_network.cpp
[server_hpp]: https://github.com/AlexandruIca/Skribble/blob/master/src/server.hpp
[server_cpp]: https://github.com/AlexandruIca/Skribble/blob/master/src/server.cpp
[client_hpp]: https://github.com/AlexandruIca/Skribble/blob/master/src/client.hpp
[client_cpp]: https://github.com/AlexandruIca/Skribble/blob/master/src/client.cpp
[releases]: https://github.com/AlexandruIca/Skribble/releases
