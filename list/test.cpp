#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"
#include "list.h"
#include <iostream>
#include <vector>

using namespace lab618;
using namespace std;


TEST_CASE("SingleLinkedList: Creating empty list") {
  CSingleLinkedList<int> a;
  CSingleLinkedList<double> b;
  CSingleLinkedList<std::vector<float>> c;
}


TEST_CASE("SingleLinkedList: PushBack/PopFront one element test") {
  CSingleLinkedList<int> a;
  for (int i = 0; i < 1000; ++i) {
    a.pushBack(i);
    CHECK(a.getSize() == 1);
    CHECK(a.popFront() == i);
    CHECK(a.getSize() == 0);
  }
}

TEST_CASE("SingleLinkedList: PushBack/PopFront multiple elements test") {
  CSingleLinkedList<int> a;
  for (int i = 1; i < 1000; i++) {
    for (int j = 1; j <= i; j++) {
      a.pushFront(j);
      CHECK(a.getSize() == j);
    }
    for (int j = i; j > 0; j--) {
      CHECK(a.popFront() == j);
      CHECK(a.getSize() == j - 1);
    }
  }
}
TEST_CASE("SingleLinkedList: PushFront/PopFront one element test") {
  CSingleLinkedList<int> a;
  for (int i = 0; i < 1000; ++i) {
    a.pushFront(i);
    CHECK(a.getSize() == 1);
    CHECK(a.popFront() == i);
    CHECK(a.getSize() == 0);
  }
}

TEST_CASE("SingleLinkedList: PushBack/PopFront multiple elements test") {
  CSingleLinkedList<int> a;
  for (int i = 1; i < 1000; i++) {
    for (int j = 1; j <= i; j++) {
      a.pushBack(j);
      CHECK(a.getSize() == j);
    }
    for (int j = i; j > 0; j--) {
      CHECK(a.popFront() == i - j + 1);
      CHECK(a.getSize() == j - 1);
    }
  }
}

TEST_CASE("SingleLinkedList: Iterator empty list simple test") {
  CSingleLinkedList<int> a;

  auto iterator_1 = a.begin();
  auto iterator_2(iterator_1);
  auto iterator_3 = iterator_2;

  CHECK(!(iterator_1 != iterator_2));
  CHECK(!(iterator_1 != iterator_3));
  CHECK(!(iterator_3 != iterator_1));
}

TEST_CASE("SingleLinkedList: Iterator of list simple test") {
  CSingleLinkedList<int> a;
  CSingleLinkedList<int> b;
  for (int i = 0; i < 1000; i++) {
    a.pushBack(i);
    b.pushBack(i);
  }

  auto iterator_a_1 = a.begin();
  auto iterator_a_2(iterator_a_1);
  auto iterator_a_3 = iterator_a_2;

  auto iterator_b_1 = b.begin();
  auto iterator_b_2(iterator_b_1);
  auto iterator_b_3 = iterator_b_2;

  CHECK(!(iterator_a_1 != iterator_a_2));
  CHECK(!(iterator_a_1 != iterator_a_3));
  CHECK(!(iterator_a_3 != iterator_a_1));

  CHECK(!(iterator_b_1 != iterator_b_2));
  CHECK(!(iterator_b_1 != iterator_b_3));
  CHECK(!(iterator_b_3 != iterator_b_1));

  CHECK(iterator_a_1 != iterator_b_2);
  CHECK(iterator_a_1 != iterator_b_3);
  CHECK(iterator_a_2 != iterator_b_1);
  CHECK(iterator_a_2 != iterator_b_3);
  CHECK(iterator_a_3 != iterator_b_1);
  CHECK(iterator_a_3 != iterator_b_2);

  ++iterator_a_2;
  ++iterator_a_3;
  ++iterator_a_3;
  ++iterator_b_2;
  ++iterator_b_3;
  ++iterator_b_3;

  CHECK((iterator_a_1 != iterator_a_2));
  CHECK((iterator_a_1 != iterator_a_3));
  CHECK((iterator_a_3 != iterator_a_1));

  CHECK((iterator_b_1 != iterator_b_2));
  CHECK((iterator_b_1 != iterator_b_3));
  CHECK((iterator_b_3 != iterator_b_1));

  CHECK(iterator_a_1 != iterator_b_2);
  CHECK(iterator_a_1 != iterator_b_3);
  CHECK(iterator_a_2 != iterator_b_1);
  CHECK(iterator_a_2 != iterator_b_3);
  CHECK(iterator_a_3 != iterator_b_1);
  CHECK(iterator_a_3 != iterator_b_2);
}

TEST_CASE("SingleLinkedList: iterator of empty list") {
  CSingleLinkedList<int> a;
  auto iter = a.begin();
  CHECK(!(iter.isValid()));
}

TEST_CASE("SingleLinkedList: Iterator test: ++") {
  CSingleLinkedList<int> a;
  for (int i = 0; i < 1000; i++) {
    a.pushBack(i);
  }

  int i = 0;
  for (auto iterator = a.begin(); iterator.isValid(); ++iterator) {
    CHECK(i == iterator.getData());
    i++;
  }
}

TEST_CASE("SingleLinkedList: setLeafPreBegin") {
  CSingleLinkedList<int> a;
  for (int i = 0; i < 10; ++i) {
    a.pushBack(i);
  }

  CSingleLinkedList<int>::CIterator begin;
  begin = a.begin();

  CSingleLinkedList<int>::CIterator pre_begin;
  pre_begin.setLeafPreBegin(begin.getLeaf());

  CHECK(pre_begin != begin);
  CHECK(pre_begin.isValid());
  ++pre_begin;
  CHECK(!(pre_begin != begin));
}


TEST_CASE("SingleLinkedList: Erase test 1") {
  CSingleLinkedList<int> a;
  std::vector<int> array = {1, 2, 3, 4, 5, 10, 100, 1000, 10000};
  for (auto len_iter = array.begin(); len_iter<array.end(); len_iter++) {
    int len = *len_iter;
    for (int i = 0; i < len; ++i) {
      a.pushBack(i);
    }
    auto iter = a.begin();

    for (int i = 0; i < len; i++) {
      CHECK(iter.getData() == i);
      a.erase(iter);
      CHECK(a.getSize() == len - 1 - i);
      ++iter;
    }
    CHECK(a.getSize()==0);
  }
}

TEST_CASE("SingleLinkedList: Erase test 2") {
  CSingleLinkedList<int> a;
  std::vector<int> array = {1, 2, 3, 4, 5, 10, 100, 1000, 10000};
  for (auto len_iter = array.begin(); len_iter<array.end(); len_iter++) {
    int len = *len_iter;
    for (int i = 0; i < len; ++i) {
      a.pushBack(i);
    }
    auto iter = a.begin();
    while (iter.getLeaf()->pNext != nullptr) {
      ++iter;
    }
    for (int i = len - 1; i >= 0; i--) {
      CHECK(iter.getData() == i);
      a.erase(iter);
      a.getSize();
      CHECK(a.getSize() == i);
    }
    CHECK (a.getSize()==0);
  }
}

TEST_CASE("DualLinkedList: Creating empty list") {
  CDualLinkedList<int> a;
  CDualLinkedList<double> b;
  CDualLinkedList<std::vector<float>> c;
}

TEST_CASE("DualLinkedList: Iterator empty list simple test") {
  CDualLinkedList<int> a;

  auto iterator_1 = a.begin();
  auto iterator_2(iterator_1);
  auto iterator_3 = iterator_2;

  CHECK(!(iterator_1 != iterator_2));
  CHECK(!(iterator_1 != iterator_3));
  CHECK(!(iterator_3 != iterator_1));
}

TEST_CASE("DualLinkedList: Iterator of list simple test") {
  CDualLinkedList<int> a;
  CDualLinkedList<int> b;
  for (int i = 0; i < 1000; i++) {
    a.pushBack(i);
    b.pushBack(i);
  }

  auto iterator_a_1 = a.begin();
  auto iterator_a_2(iterator_a_1);
  auto iterator_a_3 = iterator_a_2;

  auto iterator_b_1 = b.begin();
  auto iterator_b_2(iterator_b_1);
  auto iterator_b_3 = iterator_b_2;

  CHECK(!(iterator_a_1 != iterator_a_2));
  CHECK(!(iterator_a_1 != iterator_a_3));
  CHECK(!(iterator_a_3 != iterator_a_1));

  CHECK(!(iterator_b_1 != iterator_b_2));
  CHECK(!(iterator_b_1 != iterator_b_3));
  CHECK(!(iterator_b_3 != iterator_b_1));

  CHECK(iterator_a_1 != iterator_b_2);
  CHECK(iterator_a_1 != iterator_b_3);
  CHECK(iterator_a_2 != iterator_b_1);
  CHECK(iterator_a_2 != iterator_b_3);
  CHECK(iterator_a_3 != iterator_b_1);
  CHECK(iterator_a_3 != iterator_b_2);

  ++iterator_a_2;
  ++iterator_a_3;
  ++iterator_a_3;
  ++iterator_b_2;
  ++iterator_b_3;
  ++iterator_b_3;

  CHECK((iterator_a_1 != iterator_a_2));
  CHECK((iterator_a_1 != iterator_a_3));
  CHECK((iterator_a_3 != iterator_a_1));

  CHECK((iterator_b_1 != iterator_b_2));
  CHECK((iterator_b_1 != iterator_b_3));
  CHECK((iterator_b_3 != iterator_b_1));

  CHECK(iterator_a_1 != iterator_b_2);
  CHECK(iterator_a_1 != iterator_b_3);
  CHECK(iterator_a_2 != iterator_b_1);
  CHECK(iterator_a_2 != iterator_b_3);
  CHECK(iterator_a_3 != iterator_b_1);
  CHECK(iterator_a_3 != iterator_b_2);
}

TEST_CASE("DualLinkedList: iterator of empty list") {
  CDualLinkedList<int> a;
  auto iter = a.begin();
  CHECK(!(iter.isValid()));
  iter = a.end();
  CHECK(!(iter.isValid()));
}

TEST_CASE("DualLinkedList: PushBack/PopFront one element test") {
  CDualLinkedList<int> a;
  for (int i = 0; i < 1000; ++i) {
    a.pushBack(i);
    CHECK(a.getSize() == 1);
    CHECK(a.popFront() == i);
    CHECK(a.getSize() == 0);
  }
}

TEST_CASE("DualLinkedList: PushBack/PopFront multiple elements test") {
  CDualLinkedList<int> a;
  for (int i = 1; i < 1000; i++) {
    for (int j = 1; j <= i; j++) {
      a.pushFront(j);
      CHECK(a.getSize() == j);
    }
    for (int j = i; j > 0; j--) {
      CHECK(a.popFront() == j);
      CHECK(a.getSize() == j - 1);
    }
  }
}
TEST_CASE("DualLinkedList: PushFront/PopFront one element test") {
  CDualLinkedList<int> a;
  for (int i = 0; i < 1000; ++i) {
    a.pushFront(i);
    CHECK(a.getSize() == 1);
    CHECK(a.popFront() == i);
    CHECK(a.getSize() == 0);
  }
}

TEST_CASE("DualLinkedList: PushFront/PopFront multiple elements test") {
  CDualLinkedList<int> a;
  for (int i = 1; i < 1000; i++) {
    for (int j = 1; j <= i; j++) {
      a.pushFront(j);
      CHECK(a.getSize() == j);
    }
    for (int j = i; j > 0; j--) {
      CHECK(a.popFront() == j);
      CHECK(a.getSize() == j - 1);
    }
  }
}

TEST_CASE("DualLinkedList: PushBack/PopBack one element test") {
  CDualLinkedList<int> a;
  for (int i = 0; i < 1000; ++i) {
    a.pushBack(i);
    CHECK(a.getSize() == 1);
    CHECK(a.popBack() == i);
    CHECK(a.getSize() == 0);
  }
}

TEST_CASE("DualLinkedList: PushBack/PopBack multiple elements test") {
  CDualLinkedList<int> a;
  for (int i = 1; i < 1000; i++) {
    for (int j = 1; j <= i; j++) {
      a.pushBack(j);
      CHECK(a.getSize() == j);
    }
    for (int j = i; j > 0; j--) {
      CHECK(a.popFront() == i - j + 1);
      CHECK(a.getSize() == j - 1);
    }
  }
}
TEST_CASE("DualLinkedList: PushBack/PopFront one element test") {
  CDualLinkedList<int> a;
  for (int i = 0; i < 1000; ++i) {
    a.pushBack(i);
    CHECK(a.getSize() == 1);
    CHECK(a.popFront() == i);
    CHECK(a.getSize() == 0);
  }
}

TEST_CASE("DualLinkedList: PushBack/PopFront multiple elements test") {
  CDualLinkedList<int> a;
  for (int i = 1; i < 1000; i++) {
    for (int j = 1; j <= i; j++) {
      a.pushBack(j);
      CHECK(a.getSize() == j);
    }
    for (int j = i; j > 0; j--) {
      CHECK(a.popFront() == i - j + 1);
      CHECK(a.getSize() == j - 1);
    }
  }
}


TEST_CASE("DualLinkedList: Iterator test: ++") {
  CDualLinkedList<int> a;
  for (int i = 0; i < 1000; i++) {
    a.pushBack(i);
  }

  int i = 0;
  for (auto iterator = a.begin(); iterator.isValid(); ++iterator) {
    CHECK(i == iterator.getData());
    i++;
  }
}

TEST_CASE("DualLinkedList: setLeafPreBegin") {
  CDualLinkedList<int> a;
  for (int i = 0; i < 10; ++i) {
    a.pushBack(i);
  }

  CDualLinkedList<int>::CIterator begin;
  begin = a.begin();

  CDualLinkedList<int>::CIterator pre_begin;
  pre_begin.setLeafPreBegin(begin.getLeaf());

  CHECK(pre_begin != begin);
  CHECK(pre_begin.isValid());
  ++pre_begin;
  CHECK(!(pre_begin != begin));
}

TEST_CASE("DualLinkedList:  setLeafPostEnd") {
  CDualLinkedList<int> a;
  for (int i = 0; i < 10; ++i) {
    a.pushBack(i);
  }

  CDualLinkedList<int>::CIterator end;
  end = a.end();

  CDualLinkedList<int>::CIterator post_end;
  post_end.setLeafPostEnd(end.getLeaf());

  CHECK(post_end != end);
  CHECK(post_end.isValid());
  --post_end;
  CHECK(!(post_end != end));
}

TEST_CASE("DualLinkedList: Erase test 1") {
  CDualLinkedList<int> a;
  std::vector<int> array = {1, 2, 3, 4, 5, 10, 100, 1000, 10000};
  for (auto len_iter = array.begin(); len_iter<array.end(); len_iter++) {
    int len = *len_iter;
    for (int i = 0; i < len; ++i) {
      a.pushBack(i);
    }
    auto iter = a.begin();

    for (int i = 0; i < len; i++) {
      CHECK(iter.getData() == i);
      a.erase(iter);
      CHECK(a.getSize() == len - 1 - i);
      ++iter;
    }
    CHECK(a.getSize() == 0);
  }
}

TEST_CASE("DualLinkedList: Erase test 2") {
  CDualLinkedList<int> a;
  std::vector<int> array = {1, 2, 3, 4, 5, 10, 100, 1000, 10000};
  for (auto len_iter = array.begin(); len_iter<array.end(); len_iter++) {
    int len = *len_iter;
    for (int i = 0; i < len; ++i) {
      a.pushBack(i);
    }
    auto iter = a.begin();
    while (iter.getLeaf()->pNext != nullptr) {
      ++iter;
    }
    for (int i = len - 1; i >= 0; i--) {
      CHECK(iter.getData() == i);
      a.erase(iter);
      CHECK(a.getSize() == i);
    }
    CHECK(a.getSize() == 0);
  }
}

TEST_CASE("DualLinkedList: eraseAndNext test 1") {
  CDualLinkedList<int> a;
  std::vector<int> array = {1, 2, 3, 4, 5, 10, 100, 1000, 10000, 100000};
  for (auto len_iter = array.begin(); len_iter<array.end(); len_iter++) {
    int len = *len_iter;
    for (int i = 0; i < len; ++i) {
      a.pushBack(i);
    }
    auto iter = a.begin();

    for (int i = 0; i < len; i++) {
      CHECK(iter.getData() == i);
      a.eraseAndNext(iter);
      CHECK(a.getSize() == len - 1 - i);
    }
    CHECK(a.getSize() == 0);
  }
}

TEST_CASE("DualLinkedList: eraseAndNext test 2") {
  CDualLinkedList<int> a;
  std::vector<int> array = {1, 2, 3, 4, 5, 10, 100, 1000, 10000, 100000};
  for (auto len_iter = array.begin(); len_iter<array.end(); len_iter++) {
    int len = *len_iter;
    for (int i = 0; i < len; ++i) {
      a.pushBack(i);
    }
    auto iter = a.begin();
    while (iter.getLeaf()->pNext != nullptr) {
      ++iter;
    }

    for (int i = len - 1; i >= 0; i--) {
      CHECK(iter.getData() == i);
      a.eraseAndNext(iter);
      CHECK(a.getSize() == i);
      if (a.getSize() > 0)
        --iter;
    }
    CHECK(a.getSize() == 0);
  }
}