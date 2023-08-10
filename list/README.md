# Simple Linked List

This is an implementation of simple linked lists in C++. It contains two types of linked lists - single linked list and double linked list.

## Files

- `list.h` - Contains the declarations of `CSingleLinkedList` and `CDualLinkedList` classes
- `test.cpp` - Contains unit tests for the linked list classes using [doctest](https://github.com/onqtam/doctest)
- `CMakeLists.txt` - Basic CMake file to build the project

## Single Linked List

The `CSingleLinkedList` class implements a singly linked list. It contains:

- `leaf` - Private inner class that represents a node in the list
- `CIterator` - Iterator class to traverse the list
- `pushBack()` - Add element to back of list
- `pushFront()` - Add element to front of list
- `popFront()` - Remove element from front
- `erase()` - Erase element at given iterator
- `getSize()` - Get number of elements
- `clear()` - Clear the list
- `begin()` - Get iterator to beginning

## Double Linked List

The `CDualLinkedList` class implements a doubly linked list. It contains all methods of single linked list, plus:

- `pushBack()` - Add element to back
- `popBack()` - Remove element from back
- `end()` - Get iterator to end
- `eraseAndNext()` - Erase element at iterator and move to next

## Testing

The `test.cpp` file contains extensive unit tests for both classes using the [doctest](https://github.com/onqtam/doctest) framework. All key methods are tested:

- Push front/back
- Pop front/back
- Iterators
- Erase
- Edge cases

The tests can be built and run using CMake.

## Usage

The lists can be used as any standard C++ containers. For example:

```cpp
CSingleLinkedList<int> list;

list.pushBack(1); 
list.pushBack(2);

for (auto it = list.begin(); it != list.end(); ++it) {
  std::cout << *it << "\n"; 
}
```
## Building

This project uses CMake. To build, follow these steps:

1. Create a build directory:

    ```bash
    mkdir build
    ```

2. Change into the build directory:

    ```bash 
    cd build
    ```

3. Run CMake to configure the project and generate build files:

    ```bash
    cmake ..
    ```

4. Build the project using Make:

    ```bash
    make
    ``` 

This will build the `list` executable containing the tests.

Alternatively, you can build the project using your IDE like Visual Studio, Xcode etc by opening the generated project/solution files.