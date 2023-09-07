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

std::vector<double> benchmarkCHash(int num_elements,
                                   int block_size = 10000,
                                   int hashTableSize = 1000000) {

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

  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < num_elements; ++i) {
    hash_table.add(&elements[order[i]]);
  }

  auto point1 = std::chrono::high_resolution_clock::now();

  // удаляем элементы
  std::shuffle(order.begin(), order.end(), rand_setup);
  auto point2 = std::chrono::high_resolution_clock::now();
  for (int pos : order) {
    hash_table.remove(elements[pos]);
  }
  auto end = std::chrono::high_resolution_clock::now();

  double remove_ms =
      std::chrono::duration<double, std::milli>(end - point2).count();
  double  add_ms =
      std::chrono::duration<double, std::milli>(point1 - start).count();

  return {add_ms, remove_ms};
}

std::vector<double> benchmarkCAVLTree(int num_elements,
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

  auto start = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < num_elements; ++i) {
    avl_tree.add(&elements[order[i]]);
  }

  auto point1 = std::chrono::high_resolution_clock::now();

  // удаляем элементы
  std::shuffle(order.begin(), order.end(), rand_setup);
  auto point2 = std::chrono::high_resolution_clock::now();
  for (int pos : order) {
    avl_tree.remove(elements[pos]);
  }
  auto end = std::chrono::high_resolution_clock::now();

  double remove_ms =
      std::chrono::duration<double, std::milli>(end - point2).count();
  double  add_ms =
      std::chrono::duration<double, std::milli>(point1 - start).count();

  return {add_ms, remove_ms};
}

int main() {

  const int defoult_block_size(1000000);
  const std::vector<int> test_sizes = {20000,40000, 60000,   80000,  100000,
                                       120000, 140000, 160000,180000, 200000};
  const std::string out_file("/home/mike/CLionProjects/data-structures/test/data.csv");
  const std::string avl_name("AVL");
  const std::string hash_name("HASH");

  const size_t test_operation_index = 1;

  std::ofstream file;
  file.open(out_file, std::ios::out | std::ios::trunc);

  for (auto test_size : test_sizes) {
    std::cout << "testing:\t" << test_size << std::endl;
    auto res =
        benchmarkCHash(test_size, defoult_block_size);
    double time = res[test_operation_index];
    file << hash_name << "," << test_size << "," << int(time) << ",\n";
    res = benchmarkCAVLTree(test_size, defoult_block_size);
    time = res[test_operation_index];
    file << avl_name << "," << test_size << "," << int(time) << ",\n";
  }
  file.close();
}