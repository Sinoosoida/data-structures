#include "cmath"
#include <limits.h>

template <class T> unsigned int HashFunc(T *pElement){
  char* byte_array = reinterpret_cast<char*> (pElement);
  unsigned int result = 0;
  for (int i=0 ; i<sizeof(T) ; i++) {
    result += std::pow(CHAR_MAX, i)*byte_array[i];
  }
  return (result);
}