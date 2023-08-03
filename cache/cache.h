#include <iostream>
#include <unordered_map>
#include <list>
#include <functional>

// LRUCache class template implementing a least recently used cache
template<typename KeyType, typename ValueType>
class LRUCache {
public:

  // Constructor takes cache capacity and fetch function
  LRUCache(int capacity, std::function<ValueType(const KeyType&)> fetchFunction)
      : _capacity(capacity), _fetchFunction(fetchFunction) {}

  void print() {
    std::cout << "Cache content:" << std::endl;
    std::cout << "Capacity: " << _capacity << std::endl;
    std::cout << "Size: " << _cacheItemsMap.size() << std::endl;

    std::cout << "Items:" << std::endl;
    for (const auto& item : _cacheItemsList) {
      std::cout << "  " << item.first << ": " << item.second << std::endl;
    }
  }

  // Puts key-value pair in cache, moves pair to front of list
  void put(const KeyType& key, const ValueType& value) {

    // Check if key already exists
    auto it = _cacheItemsMap.find(key);
    if (it != _cacheItemsMap.end()) {

      // If so, remove old pair
      _cacheItemsList.erase(it->second);
      _cacheItemsMap.erase(it);
    }

    // Insert new pair at front
    _cacheItemsList.push_front({key, value});
    _cacheItemsMap[key] = _cacheItemsList.begin();

    // If over capacity, remove least recently used item
    if (_cacheItemsMap.size() > _capacity) {
      auto last = _cacheItemsList.end();
      last--;
      _cacheItemsMap.erase(last->first);
      _cacheItemsList.pop_back();
    }
  }

  // Gets value for key, returns nullptr if not found
  // Moves found pair to front of list
  const ValueType* get(const KeyType& key) {

    // Lookup list iterator in map
    auto it = _cacheItemsMap.find(key);

    // Not found
    if (it == _cacheItemsMap.end()) {

      // Fetch value using fetch function
      ValueType value = _fetchFunction(key);

      // Insert this key-value pair
      put(key, value);

      // Lookup again to get list iterator
      it = _cacheItemsMap.find(key);
    }

    // Splice found node to front to mark most recent
    _cacheItemsList.splice(_cacheItemsList.begin(), _cacheItemsList, it->second);

    return &(it->second->second);
  }

private:

  // List storing key-value pairs ordered by recency
  std::list<std::pair<KeyType, ValueType>> _cacheItemsList;

  // Map from key to iterator in list for fast lookup
  std::unordered_map<KeyType, typename std::list<std::pair<KeyType,ValueType>>::iterator> _cacheItemsMap;

  // Max capacity of cache
  int _capacity;

  // Function to fetch value if not in cache
  std::function<ValueType(const KeyType&)> _fetchFunction;

};