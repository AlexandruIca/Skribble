#ifndef CACHED_RESOURCE_HPP
#define CACHED_RESOURCE_HPP
#pragma once

///
/// \file
///

#include <algorithm>
#include <deque>
#include <functional>
#include <iterator>
#include <limits>
#include <utility>

namespace sk {

///
/// \brief Parameters for \ref CachedResource.
///
template<typename T>
struct ResourceTraits
{
    using ContainerType = std::deque<T>;
    ///
    /// When to store a new cache.
    ///
    static constexpr int cacheGap = 5;
    ///
    /// History limit.
    ///
    static constexpr int maxCount = 20;
};

///
/// \brief Another utility that can manage undo/redo.
///
/// This was actually supposed to be \ref FCachedResource. It turned out we were
/// overcomplicating ourselves with the `store cache after n layers`. It was
/// incredibly tedious to implement(address sanitizer found some iterator
/// invalidations) and didn't provide much benefit. This is only used when
/// multiple users are connected to store local cache for each of them.
///
template<typename T, typename Traits = ResourceTraits<T>>
class CachedResource
{
private:
    using ContainerType = typename Traits::ContainerType;
    using Iterator = typename ContainerType::iterator;

    static constexpr auto m_cacheGap = Traits::cacheGap;
    static constexpr auto m_maxCount = Traits::maxCount;

    static_assert(m_cacheGap > 0, "The cache gap should be bigger than 0!");
    static_assert(m_maxCount > 1, "The cache limit should be bigger than 1!");

    ContainerType m_data{};
    ContainerType m_cache{};

    Iterator m_dataLimit{};
    Iterator m_cacheLimit{};

    bool m_underUndo{ false };

    using Function = void (*)(T&, T&);
    Function m_function;

    [[nodiscard]] auto plus(Iterator it, int const count) const
    {
        std::advance(it, count);
        return it;
    }

    [[nodiscard]] auto getNumCaches() noexcept -> int
    {
        return m_cache.empty() ? 0
                               : static_cast<int>(std::distance(m_cache.begin(),
                                                                m_cacheLimit));
    }

    [[nodiscard]] inline auto noCaches() noexcept -> bool
    {
        return this->getNumCaches() <= 0;
    }

    [[nodiscard]] auto getIteratorPastCache() noexcept -> Iterator
    {
        auto it = m_data.begin();
        auto const offset = this->getNumCaches() * m_cacheGap;
        auto const max =
            static_cast<int>(std::distance(m_data.begin(), m_dataLimit));

        std::advance(it, std::min(max, offset));

        return it;
    }

    auto clearUndo() -> void
    {
        auto const dist = std::distance(m_dataLimit, m_data.end());

        for(int i = 0; i < dist; ++i) {
            m_data.pop_back();
        }
        m_dataLimit = m_data.end();

        if(this->noCaches()) {
            m_cache.clear();
        }
        else {
            auto const cacheDist = std::distance(m_cacheLimit, m_cache.end());

            for(int i = 0; i < cacheDist; ++i) {
                m_cache.pop_back();
            }
            m_cacheLimit = m_cache.end();
        }

        m_underUndo = false;
    }

public:
    CachedResource() = default;
    explicit CachedResource(Function f)
        : m_function{ f }
    {
    }
    CachedResource(CachedResource const&) = default;
    CachedResource(CachedResource&&) noexcept = default;
    ~CachedResource() noexcept = default;

    auto operator=(CachedResource const&) -> CachedResource& = default;
    auto operator=(CachedResource&&) noexcept -> CachedResource& = default;

    ///
    /// Similar to \ref FCachedResource::emplaceBack.
    ///
    template<typename... Ts>
    auto emplaceBack(Ts&&... ts) -> T&
    {
        if(m_underUndo) {
            this->clearUndo();

            if(m_data.size() % m_cacheGap == 0) {
                goto skip_cache;
            }
        }

        if(m_data.size() == m_cacheGap) {
            m_cache.emplace_back(*(m_data.begin()));
            auto begin = this->plus(m_data.begin(), 1);

            for(auto it = begin; it != m_data.end(); ++it) {
                m_function(m_cache.back(), *it);
            }

            m_cacheLimit = m_cache.end();
        }
        else if(!m_data.empty() && m_data.size() % m_cacheGap == 0) {
            auto const begin = this->getIteratorPastCache();
            m_cache.emplace_back(m_cache.back());

            for(auto it = begin; it != m_data.end(); ++it) {
                m_function(m_cache.back(), *it);
            }

            m_cacheLimit = m_cache.end();
        }
        if(m_cache.size() > m_maxCount) {
            m_function(m_cache.front(),
                       *(this->plus(m_data.begin(), m_cacheGap)));

            for(int i = 0; i <= m_cacheGap; ++i) {
                m_data.pop_front();
            }

            m_data.emplace_front(m_cache.front());
            m_cache.pop_front();
        }

    skip_cache:
        m_data.emplace_back(std::forward<Ts>(ts)...);
        m_dataLimit = m_data.end();
        return m_data.back();
    }

    ///
    /// \brief Get tha last stored cache or nullptr if no cache was built yet.
    ///
    [[nodiscard]] auto getLastCache() noexcept -> T*
    {
        if(this->noCaches()) {
            return nullptr;
        }

        return &(*(this->plus(m_cacheLimit, -1)));
    }

    ///
    /// Called for the last built cache + all remaining layers.
    ///
    auto reduceTo(T& value) -> void
    {
        if(this->noCaches()) {
            for(auto it = m_data.begin(); it != m_dataLimit; ++it) {
                m_function(value, *it);
            }
            return;
        }

        m_function(value, *(this->getLastCache()));
        auto begin = this->getIteratorPastCache();

        for(auto it = begin; it != m_dataLimit; ++it) {
            m_function(value, *it);
        }
    }

    ///
    /// Same as \ref reduceTo but can take a callback to know what to do.
    ///
    template<typename F2>
    auto reduceTo(F2 f) -> void
    {
        if(this->noCaches()) {
            for(auto it = m_data.begin(); it != m_dataLimit; ++it) {
                f(*it);
            }
            return;
        }

        f(*(this->getLastCache()));
        auto begin = this->getIteratorPastCache();

        for(auto it = begin; it != m_dataLimit; ++it) {
            f(*it);
        }
    }

    [[nodiscard]] auto getLast() noexcept -> T&
    {
        return *(this->plus(m_dataLimit, -1));
    }
    [[nodiscard]] auto getLast() const noexcept -> T const&
    {
        return *(this->plus(m_dataLimit, -1));
    }

    ///
    /// Similar to \ref FCachedResource::underUndo.
    ///
    [[nodiscard]] constexpr auto underUndo() const noexcept -> bool
    {
        return m_underUndo;
    }

    ///
    /// \returns true If undo was successful.
    ///          false If already at oldest change.
    ///
    [[nodiscard]] auto undo() -> bool
    {
        m_underUndo = true;

        auto const distance = std::distance(m_data.begin(), m_dataLimit);
        if(distance == 0) {
            return false;
        }

        --m_dataLimit;

        bool const onCache = ((distance % m_cacheGap) == 0);
        if(!this->noCaches() && onCache) {
            --m_cacheLimit;
        }

        return true;
    }

    ///
    /// \returns true If redo can be done still.
    ///          false If got to the newest change.
    ///
    [[nodiscard]] auto redo() -> bool
    {
        if(!m_underUndo) {
            return false;
        }
        bool val = true;
        if(this->plus(m_dataLimit, 1) == m_data.end()) {
            m_underUndo = false;
            val = false;
        }

        ++m_dataLimit;

        auto const distance = std::distance(m_data.begin(), m_dataLimit);

        if(!m_cache.empty()) {
            auto const availableCaches =
                std::distance(m_cacheLimit, m_cache.end()) > 0;
            if(availableCaches && distance % m_cacheGap == 0) {
                ++m_cacheLimit;
            }
        }

        return val;
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

#endif // !CACHED_SEQ_HPP
