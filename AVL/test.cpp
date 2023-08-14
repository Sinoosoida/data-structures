#include "doctest.h"
#include "string"
#include "utiles.h"
#include <algorithm>
#include <random>
#include <string>
#include <vector>
#include "avltree.h"

TEST_CASE("Basic operations") {
  lab618::CAVLTree<std::string, compare> cavlTree(5);

  std::string *res;

  std::string str_1 = "1";
  REQUIRE(cavlTree.add(&str_1));

  res = cavlTree.find("1");
  CHECK(res == &str_1);

  std::string str_2 = "2";
  REQUIRE(cavlTree.add(&str_2));

  res = cavlTree.find("2");
  CHECK(res == &str_2);

  std::string str_3 = "3";
  REQUIRE(cavlTree.add(&str_3));

  res = cavlTree.find("3");
  CHECK(res == &str_3);

  res = cavlTree.find("4");
  CHECK(res == nullptr);

  std::string str_4 = "4";
  REQUIRE(!cavlTree.update(&str_4));

  res = cavlTree.find("1");
  CHECK(res == &str_1);

  res = cavlTree.find("2");
  CHECK(res == &str_2);

  res = cavlTree.find("3");
  CHECK(res == &str_3);

  res = cavlTree.find("4");
  CHECK(res == &str_4);

  std::string str_5 = "2";
  REQUIRE(cavlTree.update(&str_5));

  res = cavlTree.find("2");
  CHECK(res == &str_5);

  res = cavlTree.find("1");
  CHECK(res == &str_1);

  res = cavlTree.find("3");
  CHECK(res == &str_3);

  res = cavlTree.find("4");
  CHECK(res == &str_4);

  REQUIRE(cavlTree.remove("1"));

  res = cavlTree.find("1");
  CHECK(res == nullptr);

  res = cavlTree.find("2");
  CHECK(res == &str_5);

  res = cavlTree.find("3");
  CHECK(res == &str_3);

  res = cavlTree.find("4");
  CHECK(res == &str_4);

  REQUIRE(!cavlTree.remove("1"));

  REQUIRE(!cavlTree.remove("5"));

  REQUIRE(cavlTree.remove("4"));

  res = cavlTree.find("2");
  CHECK(res == &str_5);

  res = cavlTree.find("3");
  CHECK(res == &str_3);

  res = cavlTree.find("1");
  CHECK(res == nullptr);

  res = cavlTree.find("4");
  CHECK(res == nullptr);

  cavlTree.clear();

  res = cavlTree.find("1");
  CHECK(res == nullptr);

  res = cavlTree.find("2");
  CHECK(res == nullptr);

  res = cavlTree.find("3");
  CHECK(res == nullptr);

  res = cavlTree.find("4");
  CHECK(res == nullptr);
}



TEST_CASE("Stress test") {
  std::mt19937 rand_setup(42);

  const int num_elements = 100000;
  lab618::CAVLTree<int, compare> cavlTree(5);

  std::vector<int> elements;
  for (int i = 0; i < num_elements; ++i) {
    elements.push_back(i);
  }

  std::vector<int> order;
  for (int i = 0; i < num_elements; ++i) {
    order.push_back(i);
  }

  std::shuffle(order.begin(), order.end(), rand_setup);

  for (int i = 0; i < num_elements; ++i) {
    cavlTree.add(&elements[order[i]]);
    CHECK(cavlTree.size() == i+1);
    REQUIRE(cavlTree.check());
  }

  for (int element : elements) {
    REQUIRE(cavlTree.find(element) != nullptr);
  }

  std::shuffle(order.begin(), order.end(), rand_setup);

  for (int i = 0; i < num_elements; ++i) {
    REQUIRE(cavlTree.remove(elements[order[i]]));
    CHECK(cavlTree.size() == num_elements-i-1);
    REQUIRE(cavlTree.check());
  }

  for (int element : elements) {
    REQUIRE(cavlTree.find(element) == nullptr);
  }
}
