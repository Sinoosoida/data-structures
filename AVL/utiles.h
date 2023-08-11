#include "cmath"
#include "iostream"
#include <limits.h>

// учимся отличать итерируемые и не итерируемые объекты
template <typename T, typename = void> struct is_iterable : std::false_type {};

template <typename T>
struct is_iterable<T, std::void_t<decltype(std::declval<T>().begin()),
                                  decltype(std::declval<T>().end())>>
    : std::true_type {};

// объявление хеш-функции
template <typename T>unsigned int HashFunc(const T *value);

// если итерироваться не умеем, то смотрим на байты
template <typename T> unsigned int HashFunc(const T *value, std::false_type) {
  const char *byte_array = reinterpret_cast<const char *>(value);
  unsigned int result = 0;
  for (int i = 0; i < sizeof(T); i++) {
    unsigned int char_max = CHAR_MAX;
    result += (char_max << 7 * i) * byte_array[i];
  }
  return result;
}

// если умеем, то на элементы
template <typename T>
unsigned int HashFunc(const T *collection, std::true_type) {
  unsigned int result = 0;
  int j = 0;
  for (auto i = collection->begin(); i < collection->end(); i++) {
    auto *ptr = &(*i);
    unsigned int char_max = CHAR_MAX;
    result += (char_max << 7 * j) * HashFunc(ptr);
    j++;
  }
  return result;
}

// и теперь можем всё это вызывать не парясь, что у нас за тип данных
template <typename T>unsigned int HashFunc(const T *value) {
  return HashFunc(value, is_iterable<T>{});
}

// compare(a,b) =
//-1 (a<b)
// 0 (a=b)
// 1 (a>b)

template <typename T>
int compare(const T *first_collection, const T *second_collection);

// по объекту нельзя итерироваться, будем сравнивать побайтно
template <typename T>
int compare(const T *first, const T *second, std::false_type) {
  const char *first_byte_array = reinterpret_cast<const char *>(first);
  const char *second_byte_array = reinterpret_cast<const char *>(second);
  for (int i = 0; i < sizeof(T); i++) {
    if (first_byte_array[i] == second_byte_array[i])
      continue;
    if (first_byte_array[i] < second_byte_array[i])
      return -1;
    else
      return 1;
  }
  return (0);
}

// можно итерироваться, будем сравнивать элементы
template <typename T>
unsigned int compare(const T *first_collection, const T *second_collection,
                     std::true_type) {
  unsigned int result = 0;
  int k = 0;
  auto j = second_collection->begin();
  for (auto i = first_collection->begin(); i < first_collection->end();
       i++) {

    if (j == second_collection->end()) { // второй список короче т.е. меньше
      return 1;
    }
    if (compare(&(*i), &(*j)) != 0) { // элементы списков разные
      return compare(&(*i), &(*j));
    }
    j++;
  }
  // либо списки одинаковые, либо первый короче

  if (j == second_collection->end()) { // второй список тоже закончился, они одинаковые
    return 0;
  } else { // первый список короче
    return -1;
  }
}

template <typename T>
int compare(const T *first_collection, const T *second_collection) {
  return compare(first_collection, second_collection, is_iterable<T>{});
}