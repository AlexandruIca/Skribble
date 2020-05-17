## STL
* `std::deque` used by default in [CachedResource][cached_resource_deque]
* `std::list` used by default in [FCachedResource][fcached_resource_list]
* `std::optional` used by [DrawHistory][draw_history_hpp_optional] to remember where the last drawn point was
* C++17's `emplace_back` returning a reference to the emplaced object used in [CachedLayers][draw_history_cpp_emplace_back_ref]
* Helpers from `<iterator>` used in [CachedResource][cached_resource_std_distance] and [FCachedResource][fcached_resource_std_next]:
    - `std::advance`
    - `std::distance`
    - `std::next`
* C++14/17 type traits used in [Tests][test_helper_type_traits]:
    - `std::remove_cv`
    - `std::remove_reference_t`
    - `std::is_same_v`
* Multithreading utilities used in [Tests][test_helper_thread_pool] to run tests in parallel:
    - `std::atomic`
    - `std::thread`
    - `std::condition_variable`
    - `std::mutex`
    - `std::unique_lock`
    - `std::future`
    - `std::packaged_task`
* C++17's `std::filesystem` used to iterate over test reports in [Tests][test_helper_filesystem]
* C++17's `std::invoke` and `std::invoke_result_t` used to return the correct type in [ThreadPool::push][test_helper_thread_pool_push] and to pass correct function call to thread pool workers
* `std::unique_ptr` used in:
    - [Canvas][canvas_hpp_unique_ptr]
    - [Server][server_hpp_unique_ptr]
    - [Client][client_hpp_unique_ptr]
* `std::shared_ptr` used in [ThreadPool::push][test_helper_thread_pool_push] to correctly store the result of the given function

## Language
* Lambdas used for `reduceTo` in [CachedResource][cached_resource_reduce_to]
* Trailing return types used basically everywhere
* Init statments in `if` used in:
    - [DrawHistory][draw_history_cpp_init_if] in `handleExternal` and `handleLocal`
    - [AbstractNetwork][abstract_network_cpp_send] in `send*` functions
* C++17's `[[nodiscard]]` used almost everywhere
* `explicit` used for most constructors
* `noexcept` move constructors and assignment operators whenever appropiate
* C++17's nested namespace definition used for [CachedLayers][draw_history_hpp_nested_ns]
* Anonymous namespaces used to define tests that do not conflict with other tests defined in different files but at the same line in [Tests][test_helper_anon_ns]
* `constexpr` used for configuration in:
    - [CachedLayers][draw_history_hpp_constexpr]
    - [config][canvas_config_inline_constexpr]
    - [network\_config][network_config_inline_constexpr]
    - [CachedResource][cached_resource_constexpr]
    - [FCachedResource][fcached_resource_constexpr]
* C++17's `inline` variables used in:
    - [config][canvas_config_inline_constexpr]
    - [network\_config][network_config_inline_constexpr]
* Templates used in:
    - [CachedResource][cached_resource_template]
    - [FCachedResource][fcached_resource_template]
* `static_assert` used in:
    - [CachedResource][cached_resource_static_assert]
    - [FCachedResource][fcached_resource_static_assert]
* Variadic templates + fold expressions + perfect forwarding used to implement:
    - `emplaceBack` in [CachedResource][cached_resource_emplace_back] and [FCachedResource][fcached_resource_emplace_back]
    - `sk::format` in [Format][format_hpp]
    - `ThreadPool::push` in [Tests][test_helper_thread_pool_push]
* Abstract classes:
    - `TestBase` in [Tests][test_base]
    - `AbstractNetwork` in [abstract\_network.hpp][abstract_network_class]
* Exceptions used in [Tests][test_helper_thread_pool_push] to check if a test is being pushed after termination(extends `std::exception`)

## RTTI
* Didn't really use it ourselves, but if you count Qt it's used almost everywhere to make use of signals and slots, mostly in [Canvas][canvas_hpp_slots] and [AbstractNetwork][abstract_network_signals]

## Operator Overloading
* Couldn't find much use for it, we have an overloaded `+=` to add new tests in [TestBase][test_pluseq]

## Networking(Socket Programming)
* Made use of Qt's helpers for TCP sockets/servers:
    - `QTcpServer` in [Server][server_tcp_server]
    - `QTcpSocket` in [Server][server_tcp_server] and [Client][client_tcp_socket]

Qt's signals and slots allowed for a very clean implementation.

## Design Patterns
* Client/Server: in [Client][client_hpp] and [Server][server_hpp] to create and maintain a peer-to-peer connection
* Factory: in [NetworkFactory][network_factory]
* Singleton: in [TestSingleton][test_singleton] to manage test creation globally
* Command Pattern: in [CachedResource][cached_resource] and [FCachedResource][fcached_resource] used to maintain history of insertions for undo/redo. Each modification to the canvas is stored as a separate image in a list(until the limit is reached, by default 100 images AKA 100 undos at most)
* Observer Pattern: in [Canvas][canvas_hpp_slots] and [AbstractNetwork][abstract_network_signals] if you count Qt's signals/slots

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

[cached_resource_deque]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/cached_resource.hpp#L17
[fcached_resource_list]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/fixed_cached_resource.hpp#L17
[draw_history_hpp_optional]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/draw_history.hpp#L94
[draw_history_cpp_emplace_back_ref]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/draw_history.cpp#L136
[cached_resource_std_distance]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/cached_resource.hpp#L69
[fcached_resource_std_next]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/fixed_cached_resource.hpp#L59
[test_helper_type_traits]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/tests/helper/test.hpp#L46
[test_helper_thread_pool]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/tests/helper/test.hpp#L168
[test_helper_filesystem]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/tests/helper/test.hpp#L348
[test_helper_thread_pool_push]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/tests/helper/test.hpp#L227
[canvas_hpp_unique_ptr]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/canvas.hpp#L26
[server_hpp_unique_ptr]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/server.hpp#L17
[client_hpp_unique_ptr]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/client.hpp#L17
[cached_resource_reduce_to]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/cached_resource.hpp#L171
[draw_history_cpp_init_if]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/draw_history.cpp#L124
[abstract_network_cpp_send]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/abstract_network.cpp#L27
[draw_history_hpp_nested_ns]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/draw_history.hpp#L20
[test_helper_anon_ns]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/tests/helper/test.hpp#L99
[draw_history_hpp_constexpr]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/draw_history.hpp#L32
[canvas_config_inline_constexpr]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/canvas_config.hpp#L7
[network_config_inline_constexpr]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/network_config.hpp#L9
[cached_resource_constexpr]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/cached_resource.hpp#L18
[fcached_resource_constexpr]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/fixed_cached_resource.hpp#L18
[cached_resource_template]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/cached_resource.hpp#L14
[fcached_resource_template]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/fixed_cached_resource.hpp#L14
[cached_resource_static_assert]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/cached_resource.hpp#L32
[fcached_resource_static_assert]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/fixed_cached_resource.hpp#L32
[cached_resource_emplace_back]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/cached_resource.hpp#L113
[fcached_resource_emplace_back]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/fixed_cached_resource.hpp#L55
[format_hpp]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/format.hpp#L15
[test_base]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/tests/helper/test.hpp#L22
[abstract_network_class]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/abstract_network.hpp#L13
[canvas_hpp_slots]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/canvas.hpp#L43
[abstract_network_signals]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/abstract_network.hpp#L37
[test_pluseq]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/tests/helper/test.hpp#L284
[server_tcp_server]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/server.hpp#L17
[client_tcp_socket]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/src/client.hpp#L17
[test_singleton]: https://github.com/AlexandruIca/Skribble/blob/498a8cc0285f4adcf041773ea74738ab49d55cc6/tests/helper/test.hpp#L264