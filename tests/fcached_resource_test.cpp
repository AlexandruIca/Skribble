#include "test.hpp"

#include "fixed_cached_resource.hpp"

auto add(int& dest, int const& src) -> void
{
    dest += src;
}

namespace {

auto adder = [](int& dest, int& src) -> void { dest += src; };
auto init = [](int& num) -> void { num = 0; };

} // namespace

template<typename T>
struct FTraits
{
    using ContainerType = std::deque<T>;
    static constexpr int maxCount = 10;
};

TEST("[FCachedResource] Undo/Redo")
{
    sk::FCachedResource<int, FTraits<int>> res{ adder, init };

    for(int i = 0; i < 10; ++i) {
        add(res.emplaceBack(), i + 1);
    }

    ASSERT(res.get() == 55);

    bool undo = res.undo();

    ASSERT(undo == true);
    ASSERT(res.underUndo());

    for(int i = 0; i < 8; ++i) {
        undo = res.undo();
        ASSERT(undo == true);
        ASSERT(res.underUndo());
    }

    undo = res.undo();

    ASSERT(undo == false);
    ASSERT(res.get() == 1);
    ASSERT(res.underUndo());

    bool redo = res.redo();

    ASSERT(redo == true);
    ASSERT(res.get() == 3);
    ASSERT(res.underUndo());

    for(int i = 0; i < 7; ++i) {
        redo = res.redo();
        ASSERT(redo == true);
        ASSERT(res.underUndo());
    }

    redo = res.redo();

    ASSERT(redo == false);
    ASSERT(res.underUndo() == false);

    for(int i = 0; i < 9; ++i) {
        undo = res.undo();
        ASSERT(undo == true);
        ASSERT(res.underUndo());
    }

    add(res.emplaceBack(), 10);

    ASSERT(res.get() == 11);
    ASSERT(!res.underUndo());
    ASSERT(res.redo() == false);
}

template<typename T>
struct FTrait2
{
    using ContainerType = std::deque<T>;
    static constexpr int maxCount = 10;
};

TEST("[FCachedResource] emplaceBack")
{
    sk::FCachedResource<int, FTrait2<int>> res{ adder, init };

    for(int i = 0; i < FTrait2<int>::maxCount; ++i) {
        add(res.emplaceBack(), i + 1);
        ASSERT(res.underUndo() == false);
    }

    ASSERT(res.get() == 55);
    ASSERT(res.getUnderlying().size() == 10);

    bool undo = res.undo();

    ASSERT(undo == true);
    ASSERT(res.underUndo());

    for(int i = 0; i < 3; ++i) {
        undo = res.undo();
        ASSERT(undo == true);
        ASSERT(res.underUndo());
    }

    ASSERT(res.get() == 21);

    add(res.emplaceBack(), 10);

    ASSERT(!res.underUndo());
    ASSERT(res.get() == 31);
    ASSERT(res.redo() == false);

    while(res.undo()) {
    }

    add(res.emplaceBack(), 10);

    ASSERT(res.get() == 11);
    ASSERT(!res.underUndo());
}

template<typename T>
struct FTraits3
{
    using ContainerType = std::deque<T>;
    static constexpr int maxCount = 3;
};

TEST("[FCachedResource] maxCount")
{
    sk::FCachedResource<int, FTraits3<int>> res{ adder, init };

    for(int i = 0; i < 10; ++i) {
        add(res.emplaceBack(), i + 1);
        ASSERT(!res.underUndo());
    }

    ASSERT(res.getUnderlying().size() == FTraits3<int>::maxCount);
    ASSERT(res.get() == 55);

    add(res.emplaceBack(), 11);
    add(res.emplaceBack(), 12);

    ASSERT(res.get() == 78);
    ASSERT(res.getUnderlying().size() == FTraits3<int>::maxCount);

    add(res.emplaceBack(), 13);

    ASSERT(res.get() == 91);
    ASSERT(res.getUnderlying().size() == FTraits3<int>::maxCount);

    bool undo{ true };

    for(int i = 0; i < 2; ++i) {
        undo = res.undo();
        ASSERT(undo == true);
        ASSERT(res.underUndo());
    }

    undo = res.undo();

    ASSERT(undo == false);
    ASSERT(res.get() == 66);
    ASSERT(res.underUndo());

    bool redo = res.redo();

    ASSERT(redo == true);
    ASSERT(res.get() == 78);
    ASSERT(res.underUndo());

    redo = res.redo();

    ASSERT(redo == false);
    ASSERT(res.get() == 91);
    ASSERT(!res.underUndo());
}
