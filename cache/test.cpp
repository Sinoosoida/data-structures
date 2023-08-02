#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "cache.h"
#include "doctest.h"
#include <cmath>

using namespace std;

size_t counter = 0;

// This is a key-value retrieval function, it is assumed that the execution time
// of this function is significant,
//  and this justifies the use of a cache.
//  This function also allows you to count the number of times it has been run.
auto fetchFunction_str_str = [](const std::string &key) -> std::string {
  ++counter;
  return "Value for key " + key;
};

TEST_CASE("Fill cache") {
  auto fetch = [](int key) { return key; };

  LRUCache<int, int> cache(5, fetch);

  for(int i = 1; i <= 5; i++) {
    cache.put(i, i);
  }

  cache.put(6, 6); // This won't evict anything

  CHECK(*cache.get(1) == 1); // 1 is still there

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

  for(int i = 0; i < 1000; i++) {
    if(std::rand() % 2 == 0) { // put
      int key = std::rand() % 1000;
      cache.put(key, key);
    }
    else { // get
      int key = std::rand() % 1000;
      const int* val = cache.get(key);
      if(val) {
        CHECK(*val == key);
      }
    }
  }
}


//TEST_CASE("elements without repeating") {
//
//  counter = 0;
//  const int len = 100; //number of unique pairs
//  const int size = 10; //cache size
//  LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);
//
//  for (int i = 0; i < len; i++) {
//    std::string val = std::to_string(i);
//    CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
//    CHECK(counter == len * 2);
//  }
//}
//
//TEST_CASE("cache with repeating, size = 10, period = 100") {
//  counter = 0;
//  const int len = 100; //number of unique pairs
//  const int size = 10; //cache size
//  const int period = 100; //number of unique pairs
//  LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);
//
//  for (int i = 0; i < len; i++) {
//    std::string val = std::to_string(i % period);
//    CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
//  }
//  if (period <= size) {
//    CHECK(counter - len == period);
//  } else {
//    CHECK(counter - len == len);
//  }
//}

//TEST_CASE("cache with repeating, size = 100, period = 100") {
//  counter = 0;
//  const int len = 100;
//  const int size = 100;
//  const int period = 100;
//  LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);
//
//  for (int i = 0; i < len; i++) {
//    std::string val = std::to_string(i % period);
//    CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
//  }
//  if (period <= size) {
//    CHECK(counter - len == period);
//  } else {
//    CHECK(counter - len == len);
//  }
//}
//
//TEST_CASE("cache with repeating, size = 100, period = 10") {
//  counter = 0;
//  const int len = 100;
//  const int size = 100;
//  const int period = 10;
//  LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);
//
//  for (int i = 0; i < len; i++) {
//    std::string val = std::to_string(i % period);
//    CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
//  }
//  if (period <= size) {
//    CHECK(counter - len == period);
//  } else {
//    CHECK(counter - len == len);
//  }
//}

//TEST_CASE("BIG SIZES: cache with repeating, size = 10, period = 10000") {
//  counter = 0;
//  const int len = 10000000;
//  const int size = 10;
//  const int period = 10000;
//  LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);
//
//  for (int i = 0; i < len; i++) {
//    std::string val = std::to_string(i % period);
//    CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
//  }
//  if (period <= size) {
//    CHECK(counter - len == period);
//  } else {
//    CHECK(counter - len == len);
//  }
//}

TEST_CASE("BIG SIZES: cache with repeating, size = 100000, period = 10000") {
  counter = 0;
  const int len = 10000000;
  const int size = 100000;
  const int period = 100;
  LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);

  for (int i = 0; i < len; i++) {
    std::string val = std::to_string(i % period);
    CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
  }
  if (period <= size) {
    CHECK(counter - len == period);
  } else {
    CHECK(counter - len == len);
  }
}

TEST_CASE("BIG SIZES: cache with repeating, size = 10, period = 10000") {
  counter = 0;
  const int len = 10000000;
  const int size = 10;
  const int period = 10000;
  LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);

  for (int i = 0; i < len; i++) {
    std::string val = std::to_string(i % period);
    CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
  }
  if (period <= size) {
    CHECK(counter - len == period);
  } else {
    CHECK(counter - len == len);
  }
}

// TEST_CASE("BIG SIZES: random access. size=10, period=100") {
//     counter = 0;
//
//     double safety_factor = 10;
//     const int len = 100000000;
//     const int size = 10;
//     const int period = 100;
//     LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);
//
//     for (int i = 0; i < len; i++) {
//         std::string val = std::to_string(rand()%period);
//         CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
//     }
//
//     double p_of_new_request = 1 - (size / period);
//     double expectation_of_number_of_requests = p_of_new_request * (len);
//     double dispersion = p_of_new_request*(1-p_of_new_request)*(len);
////    expectation_of_number_of_requests+=period;
//    double standard_deviation = sqrt(dispersion);
//
//    CHECK(counter-len >= expectation_of_number_of_requests -
//    standard_deviation*safety_factor); CHECK(counter-len <=
//    expectation_of_number_of_requests + standard_deviation*safety_factor);
//}

// TEST_CASE("BIG SIZES: random access. size=50, period=100") {
//     counter = 0;
//
//     double safety_factor = 10;
//     const int len = 100000000;
//     const int size = 50;
//     const int period = 100;
//     LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);
//
//     for (int i = 0; i < len; i++) {
//         std::string val = std::to_string(rand()%period);
//         CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
//     }
//
//     double p_of_new_request = 1 - (size / period);
//     double expectation_of_number_of_requests = p_of_new_request * (len);
//     double dispersion = p_of_new_request*(1-p_of_new_request)*(len);
////    expectation_of_number_of_requests+=period;
//    double standard_deviation = sqrt(dispersion);
//
//    CHECK(counter-len >= expectation_of_number_of_requests -
//    standard_deviation*safety_factor); CHECK(counter-len <=
//    expectation_of_number_of_requests + standard_deviation*safety_factor);
//}

// TEST_CASE("BIG SIZES: random access. size=90, period=100") {
//     counter = 0;
//
//     double safety_factor = 10;
//     const int len = 100000000;
//     const int size = 90;
//     const int period = 100;
//     LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);
//
//     for (int i = 0; i < len; i++) {
//         std::string val = std::to_string(rand()%period);
//         CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
//     }
//
//     double p_of_new_request = 1 - (size / period);
//     double expectation_of_number_of_requests = p_of_new_request * (len);
//     double dispersion = p_of_new_request*(1-p_of_new_request)*(len);
////    expectation_of_number_of_requests+=period;
//    double standard_deviation = sqrt(dispersion);
//
//    CHECK(counter-len >= expectation_of_number_of_requests -
//    standard_deviation*safety_factor); CHECK(counter-len <=
//    expectation_of_number_of_requests + standard_deviation*safety_factor);
//}

// TEST_CASE("BIG SIZES: random access. size=100, period=1000") {
//     counter = 0;
//
//     double safety_factor = 10;
//     const int len = 1000000000;
//     const int size = 100;
//     const int period = 1000;
//     LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);
//
//     for (int i = 0; i < len; i++) {
//         std::string val = std::to_string(rand()%period);
//         CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
//     }
//
//     double p_of_new_request = 1 - (size / period);
//     double expectation_of_number_of_requests = p_of_new_request * (len);
//     double dispersion = p_of_new_request*(1-p_of_new_request)*(len);
//     //    expectation_of_number_of_requests+=period;
//     double standard_deviation = sqrt(dispersion);
//
//     CHECK(counter-len >= expectation_of_number_of_requests -
//     standard_deviation*safety_factor); CHECK(counter-len <=
//     expectation_of_number_of_requests + standard_deviation*safety_factor);
// }

// TEST_CASE("BIG SIZES: random access. size=100, period=1000") {
//     counter = 0;
//
////    double safety_factor = 10;
//    const int len = 1000000000;
//    const int size = 100000;
//    const int period = 100000;
//    LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);
//
//    for (int i = 0; i < len; i++) {
//        std::string val = std::to_string(rand()%period);
//        CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
//    }
//
////    double p_of_new_request = 1 - (size / period);
////    double expectation_of_number_of_requests = p_of_new_request * (len);
////    double dispersion = p_of_new_request*(1-p_of_new_request)*(len);
////    //    expectation_of_number_of_requests+=period;
////    double standard_deviation = sqrt(dispersion);
//
////    CHECK(counter-len >= expectation_of_number_of_requests -
///standard_deviation*safety_factor);
//    CHECK(counter-len <= size);
//}
