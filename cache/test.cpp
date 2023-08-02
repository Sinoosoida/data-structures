#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "cache.h"
#include "doctest.h"
#include <cmath>

using namespace std;

size_t counter = 0;

auto fetchFunction_str_str = [](const std::string &key) -> std::string {
    ++counter;
    return "Value for key " + key;
};


TEST_CASE("cache without repeating, size = 1") {

    counter = 0;
    const int len = 100;
    const int size = 1;
    LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);

    for (int i = 0; i < len; i++) {
        std::string val = std::to_string(i);
        CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
    }
    CHECK(counter == len * 2);
}

TEST_CASE("cache without repeating, size = 2") {
    counter = 0;
    const int len = 100;
    const int size = 2;
    LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);

    for (int i = 0; i < len; i++) {
        std::string val = std::to_string(i);
        CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
    }
    CHECK(counter == len * 2);
}

TEST_CASE("cache without repeating, size = 10") {
    counter = 0;
    const int len = 100;
    const int size = 10;
    LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);

    for (int i = 0; i < len; i++) {
        std::string val = std::to_string(i);
        CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
    }
    CHECK(counter == len * 2);
}

TEST_CASE("cache without repeating, size = 100") {
    counter = 0;
    const int len = 100;
    const int size = 100;
    LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);

    for (int i = 0; i < len; i++) {
        std::string val = std::to_string(i);
        CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
    }
    CHECK(counter == len * 2);
}

TEST_CASE("cache without repeating, size = 1000") {
    counter = 0;
    const int len = 100;
    const int size = 1000;
    LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);

    for (int i = 0; i < len; i++) {
        std::string val = std::to_string(i);
        CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
    }
    CHECK(counter == len * 2);
}

TEST_CASE("cache with repeating, size = 10, period = 100") {
    counter = 0;
    const int len = 100;
    const int size = 10;
    const int period = 100;
    LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);

    for (int i = 0; i < len; i++) {
        std::string val = std::to_string(i%period);
        CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
    }
    if (period<=size) {
        CHECK(counter-len == period);
    } else {
        CHECK(counter-len == len);
    }
}

TEST_CASE("cache with repeating, size = 100, period = 100") {
    counter = 0;
    const int len = 100;
    const int size = 100;
    const int period = 100;
    LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);

    for (int i = 0; i < len; i++) {
        std::string val = std::to_string(i%period);
        CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
    }
    if (period<=size) {
        CHECK(counter-len == period);
    } else {
        CHECK(counter-len == len);
    }
}

TEST_CASE("cache with repeating, size = 100, period = 10") {
    counter = 0;
    const int len = 100;
    const int size = 100;
    const int period = 10;
    LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);

    for (int i = 0; i < len; i++) {
        std::string val = std::to_string(i%period);
        CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
    }
    if (period<=size) {
        CHECK(counter-len == period);
    } else {
        CHECK(counter-len == len);
    }
}

TEST_CASE("BIG SIZES: cache with repeating, size = 10, period = 10000") {
    counter = 0;
    const int len = 10000000;
    const int size = 10;
    const int period = 10000;
    LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);

    for (int i = 0; i < len; i++) {
        std::string val = std::to_string(i%period);
        CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
    }
    if (period<=size) {
        CHECK(counter-len == period);
    } else {
        CHECK(counter-len == len);
    }
}

TEST_CASE("BIG SIZES: cache with repeating, size = 100000, period = 10000") {
    counter = 0;
    const int len = 10000000;
    const int size = 100000;
    const int period = 100;
    LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);

    for (int i = 0; i < len; i++) {
        std::string val = std::to_string(i%period);
        CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
    }
    if (period<=size) {
        CHECK(counter-len == period);
    } else {
        CHECK(counter-len == len);
    }
}

TEST_CASE("BIG SIZES: cache with repeating, size = 10, period = 10000") {
    counter = 0;
    const int len = 10000000;
    const int size = 10;
    const int period = 10000;
    LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);

    for (int i = 0; i < len; i++) {
        std::string val = std::to_string(i%period);
        CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
    }
    if (period<=size) {
        CHECK(counter-len == period);
    } else {
        CHECK(counter-len == len);
    }
}

TEST_CASE("BIG SIZES: random access. size=10, period=100") {
    counter = 0;

    double safety_factor = 10;
    const int len = 100000000;
    const int size = 10;
    const int period = 100;
    LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);

    for (int i = 0; i < len; i++) {
        std::string val = std::to_string(rand()%period);
        CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
    }

    double p_of_new_request = 1 - (size / period);
    double expectation_of_number_of_requests = p_of_new_request * (len);
    double dispersion = p_of_new_request*(1-p_of_new_request)*(len);
//    expectation_of_number_of_requests+=period;
    double standard_deviation = sqrt(dispersion);

    CHECK(counter-len >= expectation_of_number_of_requests - standard_deviation*safety_factor);
    CHECK(counter-len <= expectation_of_number_of_requests + standard_deviation*safety_factor);
}

TEST_CASE("BIG SIZES: random access. size=50, period=100") {
    counter = 0;

    double safety_factor = 10;
    const int len = 100000000;
    const int size = 50;
    const int period = 100;
    LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);

    for (int i = 0; i < len; i++) {
        std::string val = std::to_string(rand()%period);
        CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
    }

    double p_of_new_request = 1 - (size / period);
    double expectation_of_number_of_requests = p_of_new_request * (len);
    double dispersion = p_of_new_request*(1-p_of_new_request)*(len);
//    expectation_of_number_of_requests+=period;
    double standard_deviation = sqrt(dispersion);

    CHECK(counter-len >= expectation_of_number_of_requests - standard_deviation*safety_factor);
    CHECK(counter-len <= expectation_of_number_of_requests + standard_deviation*safety_factor);
}

TEST_CASE("BIG SIZES: random access. size=90, period=100") {
    counter = 0;

    double safety_factor = 10;
    const int len = 100000000;
    const int size = 90;
    const int period = 100;
    LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);

    for (int i = 0; i < len; i++) {
        std::string val = std::to_string(rand()%period);
        CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
    }

    double p_of_new_request = 1 - (size / period);
    double expectation_of_number_of_requests = p_of_new_request * (len);
    double dispersion = p_of_new_request*(1-p_of_new_request)*(len);
//    expectation_of_number_of_requests+=period;
    double standard_deviation = sqrt(dispersion);

    CHECK(counter-len >= expectation_of_number_of_requests - standard_deviation*safety_factor);
    CHECK(counter-len <= expectation_of_number_of_requests + standard_deviation*safety_factor);
}

TEST_CASE("BIG SIZES: random access. size=100, period=1000") {
    counter = 0;

    double safety_factor = 10;
    const int len = 1000000000;
    const int size = 100;
    const int period = 1000;
    LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);

    for (int i = 0; i < len; i++) {
        std::string val = std::to_string(rand()%period);
        CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
    }

    double p_of_new_request = 1 - (size / period);
    double expectation_of_number_of_requests = p_of_new_request * (len);
    double dispersion = p_of_new_request*(1-p_of_new_request)*(len);
    //    expectation_of_number_of_requests+=period;
    double standard_deviation = sqrt(dispersion);

    CHECK(counter-len >= expectation_of_number_of_requests - standard_deviation*safety_factor);
    CHECK(counter-len <= expectation_of_number_of_requests + standard_deviation*safety_factor);
}

TEST_CASE("BIG SIZES: random access. size=100, period=1000") {
    counter = 0;

//    double safety_factor = 10;
    const int len = 1000000000;
    const int size = 100000;
    const int period = 100000;
    LRUCache<std::string, std::string> cache(size, fetchFunction_str_str);

    for (int i = 0; i < len; i++) {
        std::string val = std::to_string(rand()%period);
        CHECK(((*(cache.get(val))) == fetchFunction_str_str(val)));
    }

//    double p_of_new_request = 1 - (size / period);
//    double expectation_of_number_of_requests = p_of_new_request * (len);
//    double dispersion = p_of_new_request*(1-p_of_new_request)*(len);
//    //    expectation_of_number_of_requests+=period;
//    double standard_deviation = sqrt(dispersion);

//    CHECK(counter-len >= expectation_of_number_of_requests - standard_deviation*safety_factor);
    CHECK(counter-len <= size);
}


