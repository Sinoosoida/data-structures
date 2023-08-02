#include "doctest.h"
#include "hash.h"
#include "string"
#include "utiles.h"
#include <algorithm>
#include <limits.h>
#include <random>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

TEST_CASE("Hash function:equal hash of same data types") {
  size_t ln = 255 * 255;

  int *a = new int[ln * 2];
  char *b = new char[ln * 2];
  double *c = new double[ln * 2];
  std::string *d = new std::string[ln * 2];

  for (int i = 0; i < ln * 2; i++) {
    a[i] = i % ln;
    b[i] = i % ln;
    c[i] = i % ln;
    d[i] = std::string(i % ln % 255, char(i % ln / 255));
  }

  for (int i = 0; i < ln; i++) {
    CHECK(HashFunc(a + i) == HashFunc(a + i + ln));
    CHECK(HashFunc(b + i) == HashFunc(b + i + ln));
    CHECK(HashFunc(c + i) == HashFunc(c + i + ln));
    CHECK(HashFunc(d + i) == HashFunc(d + i + ln));
  }
  delete[] a;
  delete[] b;
  delete[] c;
}

TEST_CASE("compare function:equal values") {
  size_t ln = 255 * 255;

  int *a = new int[ln * 2];
  char *b = new char[ln * 2];
  double *c = new double[ln * 2];
  std::string *d = new std::string[ln * 2];

  for (int i = 0; i < ln * 2; i++) {
    a[i] = i % ln;
    b[i] = i % ln;
    c[i] = i % ln;
    d[i] = std::string(i % ln % 255, char(i % ln / 255));
  }

  for (int i = 0; i < ln; i++) {
    CHECK(compare(a + i, a + i + ln) == 0);
    CHECK(compare(b + i, b + i + ln) == 0);
    CHECK(compare(c + i, c + i + ln) == 0);
    CHECK(compare(d + i, d + i + ln) == 0);
  }
  delete[] a;
  delete[] b;
  delete[] c;
}

TEST_CASE("compare function:lexicographical order") {
  std::vector<std::string> arr = {"A", "AA", "AB", "B", "BA", "BB"};
  for (auto bigger = arr.begin(); bigger < arr.end(); bigger++) {
    for (auto smaller = arr.begin(); smaller < bigger; smaller++) {
      auto smaller_ptr = &(*smaller);
      auto bigger_ptr = &(*bigger);
      CHECK(compare(bigger_ptr, smaller_ptr) == 1);
      CHECK(compare(smaller_ptr, bigger_ptr) == -1);
    }
  }
}

TEST_CASE("Hash function properties test ()") {
  size_t n = 100000;
  size_t reserve_factor = 10;

  SUBCASE("double") {

    // создаём массив double
    std::vector<double> double_vec;
    double_vec.reserve(n);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    while (double_vec.size() < n) {
      double rand_value = dis(gen);

      if (std::find(double_vec.begin(), double_vec.end(), rand_value) ==
          double_vec.end()) {
        double_vec.push_back(rand_value);
      }
    }

    // вычисляем хэш
    std::vector<unsigned int> hash_vector;
    for (auto element : double_vec) {
      hash_vector.push_back(HashFunc(&element));
    }

    // считаем количество уникальных хешей
    std::unordered_set<unsigned int> uniq;
    for (int i = 0; i < n; ++i) {
      uniq.insert(hash_vector[i]);
    }

    CHECK(uniq.size() <= hash_vector.size());
    CHECK(hash_vector.size() - n * n * reserve_factor / UINT32_MAX <=
          uniq.size());
  }
  SUBCASE("string") {

    // создаём массив string
    std::vector<std::string> string_vec;
    string_vec.reserve(n);

    std::string characters =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, characters.size() - 1);

    while (string_vec.size() < n) {
      std::string random_string;
      for (int i = 0; i < 10; ++i) {
        random_string += characters[dist(gen)];
      }

      if (std::find(string_vec.begin(), string_vec.end(), random_string) ==
          string_vec.end()) {
        string_vec.push_back(random_string);
      }
    }

    // вычисляем хэш
    std::vector<unsigned int> hash_vector;
    for (auto element : string_vec) {
      hash_vector.push_back(HashFunc(&element));
    }

    // считаем количество уникальных хешей
    std::unordered_set<unsigned int> uniq;
    for (int i = 0; i < n; ++i) {
      uniq.insert(hash_vector[i]);
    }

    // использую нижнюю оценку на количество уникальных значений хеша n
    // элементов
    CHECK(uniq.size() <= hash_vector.size());
    CHECK(n - n * n * reserve_factor / UINT32_MAX <= uniq.size());
  }
}

TEST_CASE("compare function:different values") {
  size_t n = 1000;

  SUBCASE("double") {

    // создаём массив double
    std::vector<double> double_vec(n);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    for (int i = 0; i < n; i++) {
      double rand_value;
      do {
        rand_value = dis(gen);
      } while (std::find(double_vec.begin(), double_vec.end(), rand_value) !=
               double_vec.end());

      double_vec[i] = rand_value;
    }

    for (auto i = double_vec.begin(); i < double_vec.end(); i++) {
      for (auto j = double_vec.begin(); j < double_vec.end(); j++) {
        if (i != j) {
          CHECK(compare(&(*i), &(*j)) != 0);
        }
      }
    }
  }
  SUBCASE("string") {

    // создаём массив string
    std::vector<std::string> string_vec;
    string_vec.reserve(n);

    std::string characters =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, characters.size() - 1);

    while (string_vec.size() < n) {
      std::string random_string;
      for (int i = 0; i < gen() % 10 + 1; ++i) {
        random_string += characters[dist(gen)];
      }

      if (std::find(string_vec.begin(), string_vec.end(), random_string) ==
          string_vec.end()) {
        string_vec.push_back(random_string);
      }
    }
    for (auto i = string_vec.begin(); i < string_vec.end(); i++) {
      for (auto j = string_vec.begin(); j < string_vec.end(); j++) {
        if (i != j) {
          CHECK(compare(&(*i), &(*j)) != 0);
        }
      }
    }
  }
}

TEST_CASE("Basic operations: hash_table_size") {
  std::vector<int> data{1, 2, 3, 4, 5, 10};
  for (auto &hashTableSize : data) {
    CAPTURE(hashTableSize); // log the current input data
    lab618::CHash<std::string, HashFunc, compare> hash_table(hashTableSize, 5);

    std::string *res;

    std::string str_1 = "1";
    REQUIRE(hash_table.add(&str_1));

    res = hash_table.find("1");
    CHECK(res == &str_1);

    std::string str_2 = "2";
    REQUIRE(hash_table.add(&str_2));

    res = hash_table.find("2");
    CHECK(res == &str_2);

    std::string str_3 = "3";
    REQUIRE(hash_table.add(&str_3));

    res = hash_table.find("3");
    CHECK(res == &str_3);

    res = hash_table.find("4");
    CHECK(res == nullptr);

    std::string str_4 = "4";
    REQUIRE(!hash_table.update(&str_4));

    res = hash_table.find("1");
    CHECK(res == &str_1);

    res = hash_table.find("2");
    CHECK(res == &str_2);

    res = hash_table.find("3");
    CHECK(res == &str_3);

    res = hash_table.find("4");
    CHECK(res == &str_4);

    std::string str_5 = "2";
    REQUIRE(hash_table.update(&str_5));

    res = hash_table.find("2");
    CHECK(res == &str_5);

    res = hash_table.find("1");
    CHECK(res == &str_1);

    res = hash_table.find("3");
    CHECK(res == &str_3);

    res = hash_table.find("4");
    CHECK(res == &str_4);

    REQUIRE(hash_table.remove("1"));

    res = hash_table.find("1");
    CHECK(res == nullptr);

    res = hash_table.find("2");
    CHECK(res == &str_5);

    res = hash_table.find("3");
    CHECK(res == &str_3);

    res = hash_table.find("4");
    CHECK(res == &str_4);

    REQUIRE(!hash_table.remove("1"));

    REQUIRE(!hash_table.remove("5"));

    REQUIRE(hash_table.remove("4"));

    res = hash_table.find("2");
    CHECK(res == &str_5);

    res = hash_table.find("3");
    CHECK(res == &str_3);

    res = hash_table.find("1");
    CHECK(res == nullptr);

    res = hash_table.find("4");
    CHECK(res == nullptr);

    hash_table.clear();

    res = hash_table.find("1");
    CHECK(res == nullptr);

    res = hash_table.find("2");
    CHECK(res == nullptr);

    res = hash_table.find("3");
    CHECK(res == nullptr);

    res = hash_table.find("4");
    CHECK(res == nullptr);
  }
}

TEST_CASE("Stress test") {
  const int num_elements = 1000000;

  std::vector<int> data{num_elements / 100, num_elements / 10, num_elements / 2,
                        num_elements, num_elements * 2};

  for (auto &hashTableSize : data) {
    CAPTURE(hashTableSize); // log the current input data

    lab618::CHash<int, HashFunc, compare> hash_table(hashTableSize,
                                                     num_elements);

    std::vector<int> elements;
    for (int i = 0; i < num_elements; ++i) {
      elements.push_back(i);
    }

    std::vector<int> order;
    for (int i = 0; i < num_elements; ++i) {
      order.push_back(i);
    }

    std::shuffle(order.begin(), order.end(),
                 std::mt19937(std::random_device()()));

    for (int i = 0; i < num_elements; ++i) {
      hash_table.add(&elements[order[i]]);
    }

    for (int element : elements) {
      REQUIRE(hash_table.find(element) != nullptr);
    }

    std::shuffle(order.begin(), order.end(),
                 std::mt19937(std::random_device()()));

    for (int i = 0; i < num_elements; ++i) {
      REQUIRE(hash_table.remove(elements[order[i]]));
    }

    for (int element : elements) {
      REQUIRE(hash_table.find(element) == nullptr);
    }
  }
}
