#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "sort.h"
#include "utiles.h"
using namespace templates;
using namespace std;

// Realization of the comparison function
template <class T> int func(const T *a, const T *b) {
  int64_t res = int64_t(*a) - int64_t(*b);
  if (res > 0)
    return 1;
  if (res < 0)
    return -1;
  return 0;
}

// test class
class test_class {
private:
  char a, b;

public:
  test_class(int input) {
    a = input % 255;
    b = input / 255;
  }
  ~test_class() = default;
  explicit operator int() const { return a + b; }
  explicit operator ::int64_t () const { return a + b; }
};


TEST_CASE("Test of utiles") {
  self_destroyed_array<int> array(2, 2, 42);
  int** ptr = array.get_ptr();
  for (int i=0 ; i<2 ; i++) {
    CHECK((*ptr[i])<2);
    CHECK((*ptr[i])>=0);
  }

  *ptr[0] = 0;
  *ptr[1] = 1;
  CHECK(func(ptr[0], ptr[1])==-1);
  CHECK(func(ptr[1], ptr[0])==1);

  *ptr[0] = 1;
  *ptr[1] = 0;
  CHECK(func(ptr[0], ptr[1])==1);
  CHECK(func(ptr[1], ptr[0])==-1);

  *ptr[0] = 0;
  *ptr[1] = 0;
  CHECK(func(ptr[0], ptr[1])==0);
  CHECK(func(ptr[1], ptr[0])==0);
}

TEST_CASE("merge sort: sort of int array, len is 5") {

  const int len = 5;
  // len of array to sort
  const int number_of_iterations = 1000000;
  // number of times an array will be filled with random elements and sorted
  typedef int type;
  // data type to sort
  #define sorting_algorithm mergeSort<type>
  // algorithm to test

  for (int i = 0; i < number_of_iterations; i++) {
    self_destroyed_array<type> array(len, len ,i);
    sorting_algorithm(array.get_ptr(), len, func);
    for (int j = 0; j < len - 1; j++) {
      CHECK(!(func(array.get_ptr()[j], array.get_ptr()[j + 1]) > 0));
    }
  }
}
TEST_CASE("merge sort: sort of int array, len is 100") {

  const int len = 100;
  // len of array to sort
  const int number_of_iterations = 10000;
  // number of times an array will be filled with random elements and sorted
  typedef int type;
// data type to sort
#define sorting_algorithm mergeSort<type>
  // algorithm to test

  for (int i = 0; i < number_of_iterations; i++) {
    self_destroyed_array<type> array(len, len ,i);
    sorting_algorithm(array.get_ptr(), len, func);
    for (int j = 0; j < len - 1; j++) {
      CHECK(!(func(array.get_ptr()[j], array.get_ptr()[j + 1]) > 0));
    }
  }
}
TEST_CASE("merge sort: sort of int array, len is 1000000") {

  const int len = 1000000;
  // len of array to sort
  const int number_of_iterations = 10;
  // number of times an array will be filled with random elements and sorted
  typedef int type;
// data type to sort
#define sorting_algorithm mergeSort<type>
  // algorithm to test

  for (int i = 0; i < number_of_iterations; i++) {
    self_destroyed_array<type> array(len, len ,i);
    sorting_algorithm(array.get_ptr(), len, func);
    for (int j = 0; j < len - 1; j++) {
      CHECK(!(func(array.get_ptr()[j], array.get_ptr()[j + 1]) > 0));
    }
  }
}

TEST_CASE("merge sort: sort of int64_t array, len is 5") {

  const int len = 5;
  // len of array to sort
  const int number_of_iterations = 1000000;
  // number of times an array will be filled with random elements and sorted
  typedef int64_t type;
// data type to sort
#define sorting_algorithm mergeSort<type>
  // algorithm to test

  for (int i = 0; i < number_of_iterations; i++) {
    self_destroyed_array<type> array(len, len ,i);
    sorting_algorithm(array.get_ptr(), len, func);
    for (int j = 0; j < len - 1; j++) {
      CHECK(!(func(array.get_ptr()[j], array.get_ptr()[j + 1]) > 0));
    }
  }
}
TEST_CASE("merge sort: sort of int64_t array, len is 100") {

  const int len = 100;
  // len of array to sort
  const int number_of_iterations = 10000;
  // number of times an array will be filled with random elements and sorted
  typedef int64_t type;
// data type to sort
#define sorting_algorithm mergeSort<type>
  // algorithm to test

  for (int i = 0; i < number_of_iterations; i++) {
    self_destroyed_array<type> array(len, len ,i);
    sorting_algorithm(array.get_ptr(), len, func);
    for (int j = 0; j < len - 1; j++) {
      CHECK(!(func(array.get_ptr()[j], array.get_ptr()[j + 1]) > 0));
    }
  }
}
TEST_CASE("merge sort: sort of int64_t array, len is 1000000") {

  const int len = 1000000;
  // len of array to sort
  const int number_of_iterations = 10;
  // number of times an array will be filled with random elements and sorted
  typedef int64_t type;
// data type to sort
#define sorting_algorithm mergeSort<type>
  // algorithm to test

  for (int i = 0; i < number_of_iterations; i++) {
    self_destroyed_array<type> array(len, len ,i);
    sorting_algorithm(array.get_ptr(), len, func);
    for (int j = 0; j < len - 1; j++) {
      CHECK(!(func(array.get_ptr()[j], array.get_ptr()[j + 1]) > 0));
    }
  }
}

TEST_CASE("merge sort: sort of test_class array, len is 5") {

  const int len = 5;
  // len of array to sort
  const int number_of_iterations = 1000000;
  // number of times an array will be filled with random elements and sorted
  typedef test_class type;
// data type to sort
#define sorting_algorithm mergeSort<type>
  // algorithm to test

  for (int i = 0; i < number_of_iterations; i++) {
    self_destroyed_array<type> array(len, len ,i);
    sorting_algorithm(array.get_ptr(), len, func);
    for (int j = 0; j < len - 1; j++) {
      CHECK(!(func(array.get_ptr()[j], array.get_ptr()[j + 1]) > 0));
    }
  }
}
TEST_CASE("merge sort: sort of test_class array, len is 100") {

  const int len = 100;
  // len of array to sort
  const int number_of_iterations = 10000;
  // number of times an array will be filled with random elements and sorted
  typedef test_class type;
// data type to sort
#define sorting_algorithm mergeSort<type>
  // algorithm to test

  for (int i = 0; i < number_of_iterations; i++) {
    self_destroyed_array<type> array(len, len ,i);
    sorting_algorithm(array.get_ptr(), len, func);
    for (int j = 0; j < len - 1; j++) {
      CHECK(!(func(array.get_ptr()[j], array.get_ptr()[j + 1]) > 0));
    }
  }
}
TEST_CASE("merge sort: sort of test_class array, len is 1000000") {

  const int len = 1000000;
  // len of array to sort
  const int number_of_iterations = 10;
  // number of times an array will be filled with random elements and sorted
  typedef test_class type;
// data type to sort
//#define sorting_algorithm mergeSort<type>
  // algorithm to test

  for (int i = 0; i < number_of_iterations; i++) {
    self_destroyed_array<type> array(len, len ,i);
    sorting_algorithm(array.get_ptr(), len, func);
    for (int j = 0; j < len - 1; j++) {
      CHECK(!(func(array.get_ptr()[j], array.get_ptr()[j + 1]) > 0));
    }
  }
}

TEST_CASE("heap sort: sort of int array, len is 5") {

  const int len = 5;
  // len of array to sort
  const int number_of_iterations = 1000000;
  // number of times an array will be filled with random elements and sorted
  typedef int type;
// data type to sort
#define sorting_algorithm heapSort<type>
  // algorithm to test

  for (int i = 0; i < number_of_iterations; i++) {
    self_destroyed_array<type> array(len, len ,i);
    sorting_algorithm(array.get_ptr(), len, func);
    for (int j = 0; j < len - 1; j++) {
      CHECK(!(func(array.get_ptr()[j], array.get_ptr()[j + 1]) > 0));
    }
  }
}
TEST_CASE("heap sort: sort of int array, len is 100") {

  const int len = 100;
  // len of array to sort
  const int number_of_iterations = 10000;
  // number of times an array will be filled with random elements and sorted
  typedef int type;
// data type to sort
#define sorting_algorithm heapSort<type>
  // algorithm to test

  for (int i = 0; i < number_of_iterations; i++) {
    self_destroyed_array<type> array(len, len ,i);
    sorting_algorithm(array.get_ptr(), len, func);
    for (int j = 0; j < len - 1; j++) {
      CHECK(!(func(array.get_ptr()[j], array.get_ptr()[j + 1]) > 0));
    }
  }
}
TEST_CASE("heap sort: sort of int array, len is 1000000") {

  const int len = 1000000;
  // len of array to sort
  const int number_of_iterations = 10;
  // number of times an array will be filled with random elements and sorted
  typedef int type;
// data type to sort
#define sorting_algorithm heapSort<type>
  // algorithm to test

  for (int i = 0; i < number_of_iterations; i++) {
    self_destroyed_array<type> array(len, len ,i);
    sorting_algorithm(array.get_ptr(), len, func);
    for (int j = 0; j < len - 1; j++) {
      CHECK(!(func(array.get_ptr()[j], array.get_ptr()[j + 1]) > 0));
    }
  }
}

TEST_CASE("heap sort: sort of int64_t array, len is 5") {

  const int len = 5;
  // len of array to sort
  const int number_of_iterations = 1000000;
  // number of times an array will be filled with random elements and sorted
  typedef int64_t type;
// data type to sort
#define sorting_algorithm heapSort<type>
  // algorithm to test

  for (int i = 0; i < number_of_iterations; i++) {
    self_destroyed_array<type> array(len, len ,i);
    sorting_algorithm(array.get_ptr(), len, func);
    for (int j = 0; j < len - 1; j++) {
      CHECK(!(func(array.get_ptr()[j], array.get_ptr()[j + 1]) > 0));
    }
  }
}
TEST_CASE("heap sort: sort of int64_t array, len is 100") {

  const int len = 100;
  // len of array to sort
  const int number_of_iterations = 10000;
  // number of times an array will be filled with random elements and sorted
  typedef int64_t type;
// data type to sort
#define sorting_algorithm heapSort<type>
  // algorithm to test

  for (int i = 0; i < number_of_iterations; i++) {
    self_destroyed_array<type> array(len, len ,i);
    sorting_algorithm(array.get_ptr(), len, func);
    for (int j = 0; j < len - 1; j++) {
      CHECK(!(func(array.get_ptr()[j], array.get_ptr()[j + 1]) > 0));
    }
  }
}
TEST_CASE("heap sort: sort of int64_t array, len is 1000000") {

  const int len = 1000000;
  // len of array to sort
  const int number_of_iterations = 10;
  // number of times an array will be filled with random elements and sorted
  typedef int64_t type;
// data type to sort
#define sorting_algorithm heapSort<type>
  // algorithm to test

  for (int i = 0; i < number_of_iterations; i++) {
    self_destroyed_array<type> array(len, len ,i);
    sorting_algorithm(array.get_ptr(), len, func);
    for (int j = 0; j < len - 1; j++) {
      CHECK(!(func(array.get_ptr()[j], array.get_ptr()[j + 1]) > 0));
    }
  }
}

TEST_CASE("heap sort: sort of test_class array, len is 5") {

  const int len = 5;
  // len of array to sort
  const int number_of_iterations = 1000000;
  // number of times an array will be filled with random elements and sorted
  typedef test_class type;
// data type to sort
#define sorting_algorithm heapSort<type>
  // algorithm to test

  for (int i = 0; i < number_of_iterations; i++) {
    self_destroyed_array<type> array(len, len ,i);
    sorting_algorithm(array.get_ptr(), len, func);
    for (int j = 0; j < len - 1; j++) {
      CHECK(!(func(array.get_ptr()[j], array.get_ptr()[j + 1]) > 0));
    }
  }
}
TEST_CASE("heap sort: sort of test_class array, len is 100") {

  const int len = 100;
  // len of array to sort
  const int number_of_iterations = 10000;
  // number of times an array will be filled with random elements and sorted
  typedef test_class type;
// data type to sort
#define sorting_algorithm heapSort<type>
  // algorithm to test

  for (int i = 0; i < number_of_iterations; i++) {
    self_destroyed_array<type> array(len, len ,i);
    sorting_algorithm(array.get_ptr(), len, func);
    for (int j = 0; j < len - 1; j++) {
      CHECK(!(func(array.get_ptr()[j], array.get_ptr()[j + 1]) > 0));
    }
  }
}
TEST_CASE("heap sort: sort of test_class array, len is 1000000") {

  const int len = 1000000;
  // len of array to sort
  const int number_of_iterations = 10;
  // number of times an array will be filled with random elements and sorted
  typedef test_class type;
// data type to sort
#define sorting_algorithm heapSort<type>
  // algorithm to test

  for (int i = 0; i < number_of_iterations; i++) {
    self_destroyed_array<type> array(len, len ,i);
    sorting_algorithm(array.get_ptr(), len, func);
    for (int j = 0; j < len - 1; j++) {
      CHECK(!(func(array.get_ptr()[j], array.get_ptr()[j + 1]) > 0));
    }
  }
}