#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "mm.h"
#include <vector>

class TestClass {
public:
  TestClass() = default;

  TestClass(int n) : n(n), name(std::to_string(n)), time(n){};

  TestClass &operator=(const TestClass &other) = default;

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

//template <class T> class mm_test {
//public:
//  mm_test() = default;
//  bool add(T *t, T val) {
//    *t = val;
//    return add(t);
//  }
//  bool add(T *t) {
//    for (int i = 0; i < allocated_data.size(); i++) {
//      if (allocated_data[i].first == t) {
//        return false;
//      }
//    }
//    for (int i = 0; i < deleted_data.size(); i++) {
//      if (deleted_data[i] == t) {
//        deleted_data.erase(deleted_data.begin() + i);
//      }
//    }
//    allocated_data.push_back(std::make_pair(t, *t));
//  }
//  bool remove(T *t) {
//    for (int i = 0; i < allocated_data.size(); i++) {
//      if (allocated_data[i].first == t) {
//        allocated_data.erase(allocated_data.begin() + i);
//        deleted_data.push_back(t);
//        return true;
//      }
//    }
//    return false;
//  }
//  bool check() {
//    for (auto i : allocated_data) {
//      if (*i.first != i.second) {
//        return false;
//      }
//    }
//    return true;
//  }
//  T *get_element() { return allocated_data[0].first; }
//  std::vector<std::pair<T *, T>> allocated_data;
//  std::vector<T *> deleted_data;
//};

using namespace lab618;

//TEST_CASE("Elements are stored correctly, block size:20") {
//
//  CMemoryManager<TestClass> mm = CMemoryManager<TestClass>(20, true);
//  std::vector
//  mm_test<TestClass> MmTest = mm_test<TestClass>();
//  for (int i = 0; i < 100000; i++)
//    MmTest.add(mm.newObject(), i);
//
//  CHECK(MmTest.check());
//}
//
//TEST_CASE("deallocated elements are allocated first, block size:20") {
//
//  CMemoryManager<TestClass> mm = CMemoryManager<TestClass>(20, true);
//  mm_test<TestClass> MmTest = mm_test<TestClass>();
//  for (int i = 0; i < 100000; i++)
//    MmTest.add(mm.newObject(), i);
//
//  CHECK(MmTest.check());
//
//  for (int i = 0; i < 10000; i++)
//    CHECK(mm.deleteObject(MmTest.get_element()) ==
//          MmTest.remove(MmTest.get_element()));
//
//  CHECK(MmTest.check());
//
//  for (int i = 0; i < 10000; i++)
//    MmTest.add(mm.newObject(), i);
//
//  CHECK(MmTest.check());
//
//  CHECK(MmTest.deleted_data.empty());
//}
