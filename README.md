# Hash Table

A hash table with collision resolution using chaining is implemented.

## Project Files

* `hash.h` - contains the hash table class implementation
* `utiles.h` - contains helper functions for hashing and comparing elements
* `test.cpp` - unit tests
* `doctest.h` - unit testing framework
* `mm.h` - memory manager implementation
* `CMakeLists.txt` - build configuration file

## Hash Table Class

The `CHash` class implements a hash table with chaining for collision resolution.

Template parameters:

* `T` - type of stored data
* `HashFunc` - hashing function for elements
* `Compare` - comparison function for elements

Main methods:

* `add()` - add element
* `update()` - update element
* `find()` - find element
* `remove()` - remove element

A memory manager from `mm.h` is used for storing the collision chains.

## Testing

Unit tests are implemented in `test.cpp` using the [doctest](https://github.com/onqtam/doctest) framework.
The tests cover the main functionality of the hash table.

To run tests:

```
cmake -Bbuild 
cmake --build build
./build/hash-table
```

## Usage

Example of using the hash table:

```cpp
#include "hash.h"

int main() {

  // Create hash table
  CHash<int, HashFunc, Compare> hashTable(100, 10);

  // Add elements
  hashTable.add(new int(1)); 
  hashTable.add(new int(2));

  // Find element
  int *item = hashTable.find(2);

  // Remove element
  hashTable.remove(1);

  return 0;
}
```