#include "sort.h"


namespace templates {
template<class T>
void swap(T &FirstElement, T &SecondElement) {
  T PtrElement(FirstElement);
  FirstElement = SecondElement;
  SecondElement = PtrElement;
}

void mergeSort(void **ppArray, int length, CompareSortType pCompareFunc, void **ppArrayCopy) {

  if (length < 2)
    return ;

  int length_1 = length/2;
  int length_2 = length - length_1;
  mergeSort(ppArray, length_1, pCompareFunc, ppArrayCopy);
  mergeSort(ppArray+length_1, length_2, pCompareFunc, ppArrayCopy);

  //        void** ppArrayCopy = new void* [length];
  for (int i=0 ; i<length ; i++) {
    ppArrayCopy[i] = ppArray[i];
  }

  int k=0;
  int j=length_1;
  int i=0;
  while (i<length_1) {
    while (j<length && (pCompareFunc(ppArrayCopy[j], ppArrayCopy[i])<0)){
      ppArray[k]=ppArrayCopy[j];
      j++;
      k++;
    }
    ppArray[k]=ppArrayCopy[i];
    k++;
    i++;
  }

  //        delete[] ppArrayCopy;
}

void mergeSort(void **ppArray, int length, CompareSortType pCompareFunc) {//TODO

  void** ppArrayCopy = new void* [length];

  //        if (length < 2)
  //            return ;
  //
  //        int length_1 = length/2;
  //        int length_2 = length - length_1;
  //        mergeSort(ppArray, length_1, pCompareFunc);
  //        mergeSort(ppArray+length_1, length_2, pCompareFunc);
  //
  //        for (int i=0 ; i<length ; i++) {
  //            ppArrayCopy[i] = ppArray[i];
  //        }
  //
  //        int k=0;
  //        int j=length_1;
  //        int i=0;
  //        while (i<length_1) {
  //            while (j<length && (pCompareFunc(ppArrayCopy[j], ppArrayCopy[i])<0)){
  //                ppArray[k]=ppArrayCopy[j];
  //                j++;
  //                k++;
  //            }
  //            ppArray[k]=ppArrayCopy[i];
  //            k++;
  //            i++;
  //        }
  mergeSort(ppArray, length, pCompareFunc, ppArrayCopy);

  delete[] ppArrayCopy;
}

void heapify(void **ppArray, int length, CompareSortType pCompareFunc, int i) {
  int l(i * 2 + 1), r(i * 2 + 2);
  auto max(ppArray[i]);
  bool l_max(false), r_max(false);

  if (l < length && (pCompareFunc(max, ppArray[l]) < 0)) {
    max = ppArray[l];
    l_max = true;
    r_max = false;
  }

  if (r < length && (pCompareFunc(max, ppArray[r]) < 0)) {
    l_max = false;
    r_max = true;
  }

  if (l_max) {
    swap(ppArray[l], ppArray[i]);
    heapify(ppArray, length, pCompareFunc, l);
  }

  if (r_max) {
    swap(ppArray[r], ppArray[i]);
    heapify(ppArray, length, pCompareFunc, r);
  }
}
void heapSort(void **ppArray, int length, CompareSortType pCompareFunc) {

  //build heap
  for (int i = length / 2 - 1; i >= 0; i--) {
    heapify(ppArray, length, pCompareFunc, i);
  }

  //sort
  for (int i = length - 1; i >= 0; i--) {
    swap(ppArray[i], ppArray[0]);
    heapify(ppArray, i, pCompareFunc, 0);
  }
}
}// namespace templates