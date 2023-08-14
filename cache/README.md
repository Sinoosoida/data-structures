# Cache

This project implements a least recently used (LRU) cache in C++.

## Files

- `test.cpp` - Contains unit tests using the [doctest](https://github.com/onqtam/doctest) testing framework
- `cache.h` - Header file containing the `LRUCache` class template implementation
- `CMakeLists.txt` - CMake build configuration file

## LRUCache

The `LRUCache` class provides a template for implementing an LRU cache. It takes in two template parameters:

- `KeyType` - The key type for cache lookup
- `ValueType` - The value type stored in the cache

It stores key-value pairs internally in a `std::list` ordered by most recent access. A `std::unordered_map` maps keys to the iterator position in the list for fast lookup.

The size of the cache is set during construction by passing a max capacity. If the cache grows beyond this capacity, the least recently used items will be removed.

A fetch function can also be provided to populate cache misses.

Main methods:

- `put` - Inserts a key-value pair, updates pair if key already exists
- `get` - Gets a value for a key, calls fetch function if not present

## Usage

The cache can be used as:

```cpp
auto fetch = [](int key) { /* return value for key */ };

LRUCache<int, int> cache(100, fetch); 

cache.put(1, 10);
cache.get(1); // Returns 10
```

## Testing

Unit tests are provided in `test.cpp` using the doctest framework. They test:

- Basic usage
- Cache capacity
- Random insert/lookup

The tests can be built and run using CMake.