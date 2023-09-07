#include "avltree.h"
#include "hash.h"
#include "utiles.h"
#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <limits.h>
#include <random>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

std::vector<double> benchmarkCHash(int num_elements, int test_sample_size = 100,
                                   int block_size = 10000,
                                   int hashTableSize = 100000000) {

  std::mt19937 rand_setup(42);

  lab618::CHash<int, HashFunc, compare> hash_table(hashTableSize, block_size);

  // заполняем таблицу данными
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
    hash_table.add(&elements[order[i]]);
  }

  // теперь сделаем тест
  std::vector<int> suborder;

  std::shuffle(order.begin(), order.end(), rand_setup);
  std::copy(order.begin(), order.begin() + test_sample_size,
            std::back_inserter(suborder));

  // находим элементы
  auto start = std::chrono::high_resolution_clock::now();
  for (int pos : suborder) {
    hash_table.find(elements[pos]);
  }
  auto point1 = std::chrono::high_resolution_clock::now();

  // удаляем элементы
  for (int pos : suborder) {
    hash_table.remove(elements[pos]);
  }
  auto point2 = std::chrono::high_resolution_clock::now();

  // добавляем элементы
  for (int i = 0; i < test_sample_size; ++i) {
    hash_table.add(&elements[suborder[i]]);
  }

  auto end = std::chrono::high_resolution_clock::now();

  double add_ms =
      std::chrono::duration<double, std::milli>(end - point2).count();
  double remove_ms =
      std::chrono::duration<double, std::milli>(point2 - point1).count();
  double fins_ms =
      std::chrono::duration<double, std::milli>(point1 - start).count();

  return {add_ms, remove_ms, fins_ms};
}

std::vector<double> benchmarkCAVLTree(int num_elements,
                                      int test_sample_size = 100,
                                      int block_size = 10000) {

  std::mt19937 rand_setup(42);

  lab618::CAVLTree<int, compare> avl_tree(block_size);

  // заполняем таблицу данными
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
    avl_tree.add(&elements[order[i]]);
  }

  // теперь сделаем тест
  std::vector<int> suborder;

  std::shuffle(order.begin(), order.end(), rand_setup);
  std::copy(order.begin(), order.begin() + test_sample_size,
            std::back_inserter(suborder));

  // находим элементы
  auto start = std::chrono::high_resolution_clock::now();
  for (int pos : suborder) {
    avl_tree.find(elements[pos]);
  }
  auto point1 = std::chrono::high_resolution_clock::now();

  // удаляем элементы
  for (int pos : suborder) {
    avl_tree.remove(elements[pos]);
  }
  auto point2 = std::chrono::high_resolution_clock::now();

  // добавляем элементы
  for (int i = 0; i < test_sample_size; ++i) {
    avl_tree.add(&elements[suborder[i]]);
  }

  auto end = std::chrono::high_resolution_clock::now();

  double add_ms =
      std::chrono::duration<double, std::milli>(end - point2).count();
  double remove_ms =
      std::chrono::duration<double, std::milli>(point2 - point1).count();
  double fins_ms =
      std::chrono::duration<double, std::milli>(point1 - start).count();

  return {add_ms, remove_ms, fins_ms};
}

int main() {

  const int defoult_block_size(10000);
  const int defoult_test_sample_size(500);
  const std::vector<int> test_sizes = {1000, 2000, 5000, 10000, 20000,   50000,  100000,
                                       200000, 500000, 1000000,2000000, 5000000};
  const std::string out_file("/home/mike/CLionProjects/data-structures/test/data.csv");
  const std::string avl_name("AVL");
  const std::string hash_name("HASH");

  const size_t test_operation_index = 2;

  std::ofstream file;
  file.open(out_file, std::ios::out | std::ios::trunc);

  for (auto test_size : test_sizes) {
    auto res =
        benchmarkCHash(test_size, defoult_test_sample_size, defoult_block_size);
    double time = res[test_operation_index];
    file << hash_name << "," << test_size << "," << time << ",\n";
    res = benchmarkCAVLTree(test_size, defoult_test_sample_size, defoult_block_size);
    time = res[test_operation_index];
    file << avl_name << "," << test_size << "," << time << ",\n";
  }
  file.close();
}