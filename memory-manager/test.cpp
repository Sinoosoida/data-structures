#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "mm.h"
#include <vector>

class TestClass {
    public:
    TestClass() = default;

    TestClass(int n)
        : n(n), name(std::to_string(n)), time(n) {};

    TestClass& operator=(const TestClass& other) = default;

    TestClass(const TestClass& other) = default;

    bool operator==(const TestClass& other) const {
        return ((n == other.n) && (name == other.name) && (time == other.time));
    }

    bool operator!=(const TestClass& other) const { return !(*this == other); }

    private:
    int n = 0;
    std::string name;
    time_t time = time_t();
};

using namespace lab618;

TEST_CASE("Testing MM: Creating and deleting elements") {
    int block_size = 10

    CMemoryManager mm = CMemoryManager<TestClass>(size, true);
    int amount = 1000;
    for (int i = 0; i < amount; ++i) {
        TestClass *obj = mm.newObject();
        CHECK(*obj == TestClass());
        if (i % 2) {
          CHECK(mm.deleteObject(obj) == true);
        }
    }
    TestClass* wrong_obj = new TestClass();
    CHECK(mm.deleteObject(wrong_obj) == false);
    CHECK_NOTHROW(mm.clear());
}