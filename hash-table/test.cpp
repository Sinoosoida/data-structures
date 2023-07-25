#include "doctest.h"
#include "utiles.h"

TEST_CASE("Hash function test") {
  size_t ln = 10000;

  int *a = new int[ln * 2];
  char *b = new char[ln * 2];
  double *c = new double[ln * 2];

  for (int i = 0; i < ln * 2; i++) {
    a[i] = i % ln;
    b[i] = i % ln;
    c[i] = i % ln;
  }

  for (int i = 0; i < ln; i++) {
    CHECK(HashFunc(a + i) == HashFunc(a + i + ln));
    CHECK(HashFunc(b + i) == HashFunc(b + i + ln));
    CHECK(HashFunc(c + i) == HashFunc(c + i + ln));
  }
  CHECK(HashFunc(a) != HashFunc(a + 1));
  CHECK(HashFunc(b) != HashFunc(b + 1));
  CHECK(HashFunc(c) != HashFunc(c + 1));

  delete[] a;
  delete[] b;
  delete[] c;
}