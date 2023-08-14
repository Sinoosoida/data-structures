# AVL Tree

This is an implementation of an AVL tree data structure in C++.

## Description

An AVL tree is a self-balancing binary search tree. It guarantees O(log n) time complexity for search, insert and delete operations.

The implementation uses a custom memory manager to allocate nodes. It supports the following key operations:

- Insert
- Delete
- Search
- Update

### Balancing

To keep the tree balanced, rotations are performed on nodes whenever the balance factor diffs by 2.

The 4 types of rotations used are:

- Left rotation
- Right rotation
- Left-Right rotation
- Right-Left rotation

After rotations, balance factors are updated recursively up to the root.

## Files

**avltree.h**

Contains the implementation of CAVLTree class.

**utiles.h**

Has utility functions like compare.

**test.cpp**

Contains test cases using doctest framework.

**CMakeLists.txt**

Has instructions to build executable for testing.

## Building and Testing

This project uses CMake.

To build, run:

```
mkdir build
cd build 
cmake ..
make
```

This will build the AVL executable.

To run tests:

```
./AVL
```

## Usage

The CAVLTree class provides following methods:

**Insert:**

```cpp 
bool add(T* pElement)
```

**Delete:**

```cpp
bool remove(const T& element) 
```

**Update:**

```cpp
bool update(T* pElement)  
```

**Search:**

```cpp
T* find(const T& element)
``` 

**Print:**

```cpp
void print() const
```

**Check invariants:**

```cpp  
bool check() const
```

**Clear tree:**

```cpp
void clear()
```

See test.cpp for example usage.

## Analysis

**Time complexity:**

- Insert - O(log n)
- Delete - O(log n)
- Search - O(log n)

**Space complexity:**

O(n)

Where n is number of elements in the tree.

## Notes

- Only allows unique elements
- Does not handle duplicate entries
- Not thread safe