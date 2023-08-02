#include <iostream>
#include <unordered_map>
#include <list>
#include <functional>

template<typename KeyType, typename ValueType>
class LRUCache {
    public:
    LRUCache(int capacity, std::function<ValueType(const KeyType&)> fetchFunction)
        : _capacity(capacity), _fetchFunction(fetchFunction) {}

    void put(const KeyType& key, const ValueType& value) {
        auto it = _cacheItemsMap.find(key);
        if (it != _cacheItemsMap.end()) {
            _cacheItemsList.erase(it->second);
            _cacheItemsMap.erase(it);
        }

        _cacheItemsList.push_front({ key, value });
        _cacheItemsMap[key] = _cacheItemsList.begin();

        if (_cacheItemsMap.size() > _capacity) {
            auto last = _cacheItemsList.end();
            last--;
            _cacheItemsMap.erase(last->first);
            _cacheItemsList.pop_back();
        }
    }

    const ValueType* get(const KeyType& key) {
        auto it = _cacheItemsMap.find(key);
        if (it == _cacheItemsMap.end()) {
            ValueType value = _fetchFunction(key);
            put(key, value);
            it = _cacheItemsMap.find(key);
        }

        _cacheItemsList.splice(_cacheItemsList.begin(), _cacheItemsList, it->second);
        return &(it->second->second);
    }

    private:
    std::list<std::pair<KeyType, ValueType>> _cacheItemsList;
    std::unordered_map<KeyType, typename std::list<std::pair<KeyType, ValueType>>::iterator> _cacheItemsMap;
    int _capacity;
    std::function<ValueType(const KeyType&)> _fetchFunction;
};
