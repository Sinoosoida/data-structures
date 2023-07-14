# Sorting Algorithms

This repository contains implementations of the merge sort and heap sort algorithms in C++. Both algorithms are declared in `sort.h`. Additionally, there are test cases for both algorithms, implemented using the doctest framework.

In this implementation, the sorting algorithms take an array of pointers to elements and sort the pointers themselves, not the actual elements. This approach allows for faster sorting in cases where the elements are structures and also enables sorting elements of different sizes.
## Merge Sort

![Image](https://th.bing.com/th/id/R.a29c0dd0186d1f8cef3c5ebdedf3e5a3?rik=yN%2bMf%2bXFsza67Q&riu=http%3a%2f%2fejklike.github.io%2fassets%2f20170301%2fmergesort.gif&ehk=YHUWa6IpDR8jnJSkvWw7ANRth1sehhdUxeswtLo%2f6EA%3d&risl=&pid=ImgRaw&r=0)

Merge sort is a divide-and-conquer algorithm that recursively splits a given array into two halves, sorts them separately, and then merges the sorted halves back together to produce a sorted array. It has a time complexity of O(n log n).

To use the merge sort implementation, include `sort.h` in your code and call the `mergeSort()` function, passing in the array to be sorted.

## Heap Sort

![Image](https://upload.wikimedia.org/wikipedia/commons/4/4d/Heapsort-example.gif)

Heap sort is another comparison-based sorting algorithm that uses a binary heap data structure. It works by building a max-heap from the given array and repeatedly extracting the maximum element from the heap to obtain a sorted sequence. Heap sort also has a time complexity of O(n log n).

To use the heap sort implementation, include `sort.h` in your code and call the `heapSort()` function, passing in the array to be sorted.

## Testing

The test cases for both merge sort and heap sort are implemented using the doctest framework. The test cases can be found in the `test.cpp` file. To run the tests, you need to have doctest installed and configured in your project. Execute the tests and verify that all assertions pass to ensure the correctness of the sorting algorithms.