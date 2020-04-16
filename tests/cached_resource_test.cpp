#define MAIN_EXECUTABLE
#include "test.hpp"

#include "cached_resource.hpp"

namespace {

auto adder = [](int& dest, int& src) -> void { dest += src; };

}

template<typename T>
struct FTraits
{
    using ContainerType = std::deque<T>;
    static constexpr int cacheGap = 3;
    static constexpr int maxCount = sk::ResourceTraits<T>::maxCount;
};

TEST("[CachedResource] Undo/Redo")
{
    sk::CachedResource<int, FTraits<int>> res{ adder };

    for(int i = 0; i < 10; ++i) {
        res.emplaceBack(i + 1);
    }

    int sum{ 0 };
    res.reduceTo(sum);

    ASSERT(sum == 55);
    ASSERT(res.getLast() == 10);
    ASSERT((*res.getLastCache()) == 45);

    bool undo = res.undo();

    ASSERT(undo == true);
    ASSERT((*res.getLastCache()) == 45);
    ASSERT(res.underUndo());

    undo = res.undo();

    ASSERT(undo == true);
    ASSERT((*res.getLastCache()) == 21);
    ASSERT(res.underUndo());

    bool redo = res.redo();

    ASSERT(redo == true);
    ASSERT(*(res.getLastCache()) == 45);
    ASSERT(res.underUndo());

    redo = res.redo();

    ASSERT(redo == false);
    ASSERT(*(res.getLastCache()) == 45);
    ASSERT(res.underUndo() == false);

    for(int i = 0; i < 9; ++i) {
        undo = res.undo();
        ASSERT(res.underUndo());
    }

    ASSERT(undo == true);
    ASSERT(res.getLastCache() == nullptr);
    ASSERT(res.getLast() == 1);
    ASSERT(res.underUndo());

    undo = res.undo();

    ASSERT(undo == true);
    ASSERT(res.underUndo());

    sum = 0;
    res.reduceTo(sum);

    ASSERT(sum == 0);

    undo = res.undo();

    ASSERT(undo == false);
    ASSERT(res.underUndo());

    for(int i = 0; i < 9; ++i) {
        redo = res.redo();
    }

    ASSERT(redo == true);
    ASSERT(res.underUndo());

    redo = res.redo();

    ASSERT(redo == false);
    ASSERT(res.underUndo() == false);
}

template<typename T>
struct Trait2
{
    using ContainerType = std::deque<T>;
    static constexpr int cacheGap = 5;
    static constexpr int maxCount = sk::ResourceTraits<T>::maxCount;
};

TEST("[CachedResource] emplaceBack")
{
    sk::CachedResource<int, Trait2<int>> res{ adder };
    bool undo{ true };
    bool redo{ true };

    for(int i = 0; i < Trait2<int>::cacheGap * 2; ++i) {
        res.emplaceBack(i + 1);
        ASSERT(res.underUndo() == false);
    }

    ASSERT(res.getLast() == 10);
    ASSERT(*(res.getLastCache()) == 15);

    for(int i = 0; i < Trait2<int>::cacheGap; ++i) {
        undo = res.undo();
        ASSERT(undo == true);
        ASSERT(res.underUndo() == true);
    }

    ASSERT(res.getLastCache() == nullptr);
    ASSERT(res.getLast() == 5);

    undo = res.undo();

    ASSERT(undo == true);
    ASSERT(res.getLastCache() == nullptr);

    redo = res.redo();

    ASSERT(redo == true);
    ASSERT(res.getLastCache() != nullptr);
    ASSERT(*(res.getLastCache()) == 15);
    ASSERT(res.underUndo() == true);

    res.emplaceBack(100);

    ASSERT(res.underUndo() == false);
    ASSERT(res.redo() == false);
    ASSERT(res.getLastCache() != nullptr);
    ASSERT(*(res.getLastCache()) == 15);
    ASSERT(res.getLast() == 100);

    int sum{ 0 };
    res.reduceTo(sum);

    ASSERT(sum == 115);
}

template<typename T>
struct Traits3
{
    using ContainerType = std::deque<T>;
    static constexpr int maxCount = 2;
    static constexpr int cacheGap = 3;
};

TEST("[CachedResource] maxCount")
{
    sk::CachedResource<int, Traits3<int>> res{ adder };

    for(int i = 0; i < 10; ++i) {
        res.emplaceBack(i + 1);
    }

    ASSERT(res.getLastCache() != nullptr);
    ASSERT(*(res.getLastCache()) == 45);
    ASSERT(res.getUnderlying().size() == 7);
    ASSERT(res.getUnderlying().front() == 10);

    res.emplaceBack(11);
    res.emplaceBack(12);

    ASSERT(res.getLastCache() != nullptr);
    ASSERT(*(res.getLastCache()) == 45);

    res.emplaceBack(13);

    ASSERT(res.getLastCache() != nullptr);
    ASSERT(*(res.getLastCache()) == 78);

    bool undo{ true };

    for(int i = 0; i < 3; ++i) {
        undo = res.undo();
        ASSERT(undo == true);
        ASSERT(res.underUndo() == true);
    }

    ASSERT(res.getLastCache() != nullptr);
    ASSERT(*(res.getLastCache()) == 45);

    int sum{ 0 };
    res.reduceTo(sum);

    ASSERT(sum == 55);

    undo = res.undo();

    ASSERT(undo == true);

    sum = 0;
    res.reduceTo(sum);

    ASSERT(sum == 45);
    ASSERT(res.getLastCache() != nullptr);
    ASSERT(*(res.getLastCache()) == 45);

    bool redo{ true };

    for(int i = 0; i < 3; ++i) {
        redo = res.redo();
        ASSERT(res.underUndo() == true);
        ASSERT(redo == true);
    }

    redo = res.redo();

    ASSERT(redo == false);
    ASSERT(res.getLastCache() != nullptr);
    ASSERT(*(res.getLastCache()) == 78);

    sum = 0;
    res.reduceTo(sum);

    ASSERT(sum == 91);
    ASSERT(res.getUnderlying().front() == 28);
    ASSERT(res.getUnderlying().size() == 7);

    for(int i = 0; i < 7; ++i) {
        undo = res.undo();
        ASSERT(res.underUndo() == true);
        ASSERT(undo == true);
    }

    undo = res.undo();

    ASSERT(undo == false);

    redo = res.redo();
    sum = 0;
    res.reduceTo(sum);

    ASSERT(redo == true);
    ASSERT(res.getLastCache() == nullptr);
    ASSERT(res.underUndo() == true);
    ASSERT(sum == 28);

    static_cast<void>(res.redo());
    static_cast<void>(res.redo());
    sum = 0;
    res.reduceTo(sum);

    ASSERT(res.getLastCache() != nullptr);
    ASSERT(*(res.getLastCache()) == 45);
    ASSERT(res.underUndo() == true);
    ASSERT(sum == 45);

    res.emplaceBack(10);
    sum = 0;
    res.reduceTo(sum);

    ASSERT(res.underUndo() == false);
    ASSERT(sum == 55);

    sum = 0;
    res.reduceTo(sum);

    ASSERT(sum == 55);
}
