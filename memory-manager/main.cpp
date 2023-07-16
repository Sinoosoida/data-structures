//#include <iostream>
//#include "mm.h"
////#include "doctest.h"
////#include <cmath>
//#include "vector"
//using namespace lab618;
//
//using namespace std;
//
//int main () {
//    CMemoryManager mm = CMemoryManager<char>(10, true);
//    std::vector<char*> vec;
//    for (int i=0 ; i<1000 ; i++) {
//        vec.push_back(mm.newObject());
//    }
//    for (int i=0 ; i<1000 ; i++) {
//        *vec[i] = i;
//    }
//    for (int i=0 ; i<1000 ; i++) {
//        std::cout << (*vec[i]==i);
//
//    }
////    for (int i=0 ; i<1000 ; i++) {
////        *(mm.newObject())=i;
////    }
////    auto a = mm.newObject();
//    return 0;
//}
