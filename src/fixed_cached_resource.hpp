#ifndef FIXED_CACHED_RESOURCE_HPP
#define FIXED_CACHED_RESOURCE_HPP
#pragma once

///
/// \file
///

#include "format.hpp"

#include <deque>
#include <iterator>
#include <list>
#include <utility>

namespace sk {

///
/// \brief Parameters for \ref FCachedResource.
///
template<typename T>
struct FResTraits
{
    ///
    /// With what to store the layers with, by default `std::list`.
    ///
    using ContainerType = std::list<T>;
    ///
    /// History limit.
    ///
    static constexpr int maxCount = 100;
};

///
/// \brief Class that manages undo/redo.
///
template<typename T, typename Traits = FResTraits<T>>
class FCachedResource
{
private:
    using ContainerType = typename Traits::ContainerType;
    using Iterator = typename ContainerType::iterator;
    using Function = void (*)(T&, T&);
    using InitFunction = void (*)(T&);

    static constexpr int m_maxCount = Traits::maxCount;

    static_assert(m_maxCount > 2, "MaxCount should be bigger than 2");

    ContainerType m_data{};
    Iterator m_iterator{};
    Function m_function{};
    InitFunction m_initFunction{};
    bool m_underUndo{ false };

public:
    FCachedResource() = delete;
    FCachedResource(FCachedResource const&) = default;
    FCachedResource(FCachedResource&&) noexcept = default;
    ~FCachedResource() noexcept = default;

    explicit FCachedResource(Function f, InitFunction init)
        : m_function{ f }
        , m_initFunction{ init }
    {
    }

    auto operator=(FCachedResource const&) -> FCachedResource& = default;
    auto operator=(FCachedResource&&) noexcept -> FCachedResource& = default;

    ///
    /// Pushes a new layer in the list of layers, deleting layers at the front
    /// if history limit has been reached.
    ///
    template<typename... Ts>
    auto emplaceBack(Ts&&... args) -> T&
    {
        if(m_underUndo) {
            m_data.erase(std::next(m_iterator), m_data.end());
            m_underUndo = false;
        }
        if(!m_data.empty()) {
            m_data.emplace_back(*m_iterator);
            ++m_iterator;
        }
        else {
            m_initFunction(m_data.emplace_back(std::forward<Ts>(args)...));
            m_iterator = m_data.begin();
        }

        if(m_data.size() > m_maxCount) {
            m_data.pop_front();
        }

        return *m_iterator;
    }

    ///
    /// \returns true If \ref undo was called more times than \ref redo.
    ///
    [[nodiscard]] inline auto underUndo() const noexcept -> bool
    {
        return m_underUndo;
    }

    ///
    /// \brief Gets the current layer.
    ///
    [[nodiscard]] auto get() noexcept -> T&
    {
        return *m_iterator;
    }
    ///
    /// \brief Gets the current layer as constant.
    ///
    [[nodiscard]] auto get() const noexcept -> T const&
    {
        return *m_iterator;
    }

    ///
    /// Current layer becomes `std::prev(current_layer_iterator)`.
    ///
    /// \returns false If undo is no longer possible.
    ///
    [[nodiscard]] auto undo() -> bool
    {
        if(m_data.size() <= 1) {
            m_underUndo = false;
            return false;
        }
        m_underUndo = true;

        if(std::distance(m_data.begin(), m_iterator) <= 0) {
            return false;
        }

        --m_iterator;
        return true;
    }

    ///
    /// Current layer becomes `std::next(current_layer_iterator)`.
    ///
    /// \returns false If redo is no longer possible.
    ///
    [[nodiscard]] auto redo() -> bool
    {
        if(!m_underUndo) {
            return false;
        }

        ++m_iterator;

        if(std::distance(m_iterator, m_data.end()) <= 1) {
            m_underUndo = false;
            return false;
        }

        return true;
    }

    [[nodiscard]] auto getUnderlying() noexcept -> ContainerType&
    {
        return m_data;
    }
    [[nodiscard]] auto getUnderlying() const noexcept -> ContainerType const&
    {
        return m_data;
    }
};

} // namespace sk

#endif // !FIXED_CACHED_RESOURCE_HPP
