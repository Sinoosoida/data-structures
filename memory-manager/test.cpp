#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "mm.h"
#include <random>
#include <string>
#include <typeinfo>
#include <vector>

using namespace lab618;

// Вспомогательный класс для тестирования
class TestClass {
public:
  TestClass() = default;

  TestClass(int n) : n(n), name(std::to_string(n)), time(n){};

  TestClass &operator=(const TestClass &other) = default;

  virtual ~TestClass() = default;

  TestClass(const TestClass &other) = default;

  bool operator==(const TestClass &other) const {
    return ((n == other.n) && (name == other.name) && (time == other.time));
  }

  bool operator!=(const TestClass &other) const { return !(*this == other); }

private:
  int n = 0;
  std::string name;
  time_t time = time_t();
};

// класс, тестирующий менеджер памяти
template <class T> class mm_test {
public:
  mm_test() = default;
  bool add(T *t, T val) {
    *t = val;
    return add(t);
  }
  void print() {
    std::cout << allocated_data.size() << " elements was allocated"
              << std::endl;
    std::cout << deleted_data.size() << " elements was deallocated"
              << std::endl;
  }
  bool add(T *t) {
    for (int i = 0; i < allocated_data.size(); i++) {
      if (allocated_data[i].first == t) {
        return false;
      }
    }
    for (int i = 0; i < deleted_data.size(); i++) {
      if (deleted_data[i] == t) {
        deleted_data.erase(deleted_data.begin() + i);
      }
    }
    allocated_data.push_back(std::make_pair(t, *t));
    return true;
  }
  bool remove(T *t) {
    for (int i = 0; i < allocated_data.size(); i++) {
      if (allocated_data[i].first == t) {
        allocated_data.erase(allocated_data.begin() + i);
        deleted_data.push_back(t);
        return true;
      }
    }
    return false;
  }
  bool check() {
    std::vector<T *> ptrs;
    for (auto i : allocated_data) {
      ptrs.push_back(i.first);
    }
    std::sort(ptrs.begin(), ptrs.end());

    for (int i = 1; i < ptrs.size(); i++) {
      if (reinterpret_cast<std::byte *>(ptrs[i]) -
              reinterpret_cast<std::byte *>(ptrs[i - 1]) <
          sizeof(T)) {
        return false;
      }
    }

    for (auto i : allocated_data) {
      if (*i.first != i.second) {
        return false;
      }
    }

    return true;
  }
  T *get_element() {
    if (allocated_data.empty()) {
      throw;
    }
    return allocated_data[0].first;
  }
  std::vector<std::pair<T *, T>> allocated_data;
  std::vector<T *> deleted_data;
};

TEST_CASE_TEMPLATE("elementary operations", T, TestClass, bool, uint8_t,
                   uint64_t, std::string, int, double) {

  auto mm = CMemoryManager<T>(20, true);
  auto mmTest = mm_test<T>();

  for (int i = 0; i < 10; i++) {
    REQUIRE(mmTest.add(mm.newObject()));
  }

  REQUIRE(mmTest.check());

  for (int i = 0; i < 5; i++) {
    auto element = mmTest.get_element();
    CHECK(mm.deleteObject(element) == mmTest.remove(element));
  }
  REQUIRE(mmTest.check());

  for (int i = 0; i < 20; i++) {
    REQUIRE(mmTest.add(mm.newObject()));
  }

  REQUIRE(mmTest.check());

  for (int i = 0; i < 20; i++) {
    auto element = mmTest.get_element();
    CHECK(mm.deleteObject(element) == mmTest.remove(element));
  }
  REQUIRE(mmTest.check());

  for (int i = 0; i < 5; i++) {
    auto element = mmTest.get_element();
    CHECK(mm.deleteObject(element) == mmTest.remove(element));
  }
  REQUIRE(mmTest.check());
}

// Проверка того, что удалённые данные вновь выделяются
TEST_CASE_TEMPLATE("Reusing space test", T, TestClass, bool, uint8_t, uint64_t,
                   std::string, int, double) {
  auto mm = CMemoryManager<T>(10);

  auto *p1 = mm.newObject();
  auto *p2 = mm.newObject();

  CHECK(p1 != nullptr);
  CHECK(p2 != nullptr);
  CHECK(p1 != p2);

  mm.deleteObject(p1);

  mm.deleteObject(p1);

  auto *p3 = mm.newObject();
  CHECK(p3 == p1);
}

// Проверка копирования и присваивания объектов
TEST_CASE_TEMPLATE("Reusing space test", T, TestClass, bool, uint8_t, uint64_t,
                   int, double) {
  auto mm = CMemoryManager<T>(10);

  auto *src = mm.newObject();
  *src = T(42);

  auto *dst = mm.newObject();
  *dst = *src;

  CHECK(*src == *dst);

  *dst = T(0);
  CHECK(*src != *dst);
}

TEST_CASE_TEMPLATE("Stress test", T, TestClass, bool, uint8_t, uint64_t,
                   std::string, int, double) {

  std::vector<int> block_sizes{1, 5, 10, 50, 100, 500, 1000, 5000, 10000};
  for (auto &block_size : block_sizes) {
    CMemoryManager<T> mm(block_size);
    auto mmTest = mm_test<T>();

    std::mt19937 gen(42);

    for (int i = 0; i < 200000; i++) {

      if (mmTest.allocated_data.empty()) {
        mmTest.add(mm.newObject());
      } else {
        if (gen() % 2) {
          mmTest.add(mm.newObject());
        } else {
          int index = gen() % mmTest.allocated_data.size();
          auto *element = mmTest.allocated_data[index].first;
          CHECK(mm.deleteObject(element) == mmTest.remove(element));
        }
      }

      CHECK(mmTest.check()); // проверка целостности данных
    }
  }
}

TEST_CASE_TEMPLATE("Bad pointer removing test", T, TestClass, bool, uint8_t,
                   uint64_t, std::string, int, double) {
  auto mm = CMemoryManager<T>(7);
  auto mmTest = mm_test<T>();

  for (int i = 0; i < 100; i++) {
    mmTest.add(mm.newObject());
  }

  auto correct_pointer = mmTest.get_element();
  for (int i = -10000; i < 10000; i++) {
    auto some_pointer =
        reinterpret_cast<T *>(reinterpret_cast<uint8_t *>(correct_pointer) + i);
    CHECK(mm.deleteObject(some_pointer) == mmTest.remove(some_pointer));
  }
}