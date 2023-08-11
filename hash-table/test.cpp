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
