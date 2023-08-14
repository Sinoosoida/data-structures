#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "cache.h"
#include "doctest.h"
#include <cmath>

using namespace std;

TEST_CASE("Fill cache") {
  auto fetch = [](int key) { return key; };

  LRUCache<int, int> cache(5, fetch);

  for (int i = 1; i <= 5; i++) {
    cache.put(i, i + 1);
  }

  CHECK(*cache.get(1) == 2); // 1 is still there

  CHECK(*cache.get(6) == 6); // 6 was added
}

TEST_CASE("Insert existing key") {
  auto fetch = [](std::string key) { return "val" + key; };

  LRUCache<std::string, std::string> cache(5, fetch);

  cache.put("a", "vala");
  cache.put("b", "valb");

  cache.put("a", "newvala"); // Update existing key

  CHECK(*cache.get("a") == "newvala");
  CHECK(*cache.get("b") == "valb");
}

TEST_CASE("Random operations") {
  auto fetch = [](int key) { return key; };

  LRUCache<int, int> cache(100, fetch);

  std::srand(std::time(nullptr));

  for (int i = 0; i < 1000; i++) {
    if (std::rand() % 2 == 0) { // put
      int key = std::rand() % 1000;
      cache.put(key, key);
    } else { // get
      int key = std::rand() % 1000;
      const int *val = cache.get(key);
      if (val) {
        CHECK(*val == key);
      }
    }
  }
}

size_t counter = 0;

// This is a key-value retrieval function, it is assumed that the execution time
// of this function is significant,
//  and this justifies the use of a cache.
//  This function also allows you to count the number of times it has been run.
auto fetchFunction_str_str = [](const std::string &key,
                                bool count_request = true) -> std::string {
  if (count_request)
    ++counter;
  return "Value for key " + key;
};

TEST_CASE("Random operations, big size") {

  //  Probability theory doesn't always favor us,
  //  but if the program works as expected,
  //  the probability of failing the test is
  //  0.004 * 5[test cases] * 2[checks per test]
  //  if safety_factor = 3 is used

  const int safety_factor = 3;

  std::vector<std::vector<int>> data{{1000000, 1, 10},
                                     {1000000, 10, 10},
                                     {1000000, 100, 300},
                                     {1000000, 299, 300},
                                     {1000000, 1, 300}};

  for (auto &test_data : data) {

    const int len = test_data[0];        // number of requests
    const int cache_size = test_data[1]; // number of elements saved in cache
    const int period = test_data[2];     // number of unique requests

    CAPTURE(len);
    CAPTURE(cache_size);
    CAPTURE(period);

    counter = 0;
    std::srand(42);

    /**
     * This uses a theoretical formula to estimate the number of spares.
     * It uses the central limit theorem.
     */
    LRUCache<std::string, std::string> cache(cache_size, fetchFunction_str_str);

    for (int i = 0; i < len; i++) {
      std::string key = std::to_string(rand() % period);

      auto real_val = fetchFunction_str_str(key, false);
      auto cached_val = *(cache.get(key));

      //      CHECK(cached_val == real_val);
    }

    // the probability that the requested item will not be in the cache
    double p_of_new_request = 1 - (double(cache_size) / double(period));

    // estimated number of fetch function runs
    double expectation_of_number_of_requests = p_of_new_request * (len);

    // single item variance calculation
    double dispersion_of_one_element =
        p_of_new_request * (1 - p_of_new_request);

    // single item standard deviation
    double standard_deviation_of_one_element = sqrt(dispersion_of_one_element);

    // standard deviation of request count
    double standard_deviation_of_number_of_requests =
        sqrt(len) * standard_deviation_of_one_element;

    CHECK(counter >=
          expectation_of_number_of_requests -
              standard_deviation_of_number_of_requests * safety_factor);

    CHECK(counter <=
          expectation_of_number_of_requests + cache_size +
              standard_deviation_of_number_of_requests * safety_factor);
  }
}