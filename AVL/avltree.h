#ifndef AVL_HEAD_H_2023_03_30
#define AVL_HEAD_H_2023_03_30

#include "mm.h"
#include "assert.h"
namespace lab618 {

template <class T, int (*Compare)(const T *pElement, const T *pElement2)>
class CAVLTree {
private:
  struct leaf {
    T *pData;
    leaf *pLeft;
    leaf *pRight;
    int balanceFactor;
  };

public:
  class CException {
  public:
    CException() {}
  };

public:
  CAVLTree(int defaultBlockSize) {
    m_Memory = CMemoryManager<T> (defaultBlockSize, true);
  }

  virtual ~CAVLTree()
  {
    clear();
  }

  bool add(T *pElement) {

  }

  bool update(T *pElement) {}

  T *find(const T &pElement) {
    leaf* current_node = m_pRoot;
    while (current_node != nullptr) {
      int cmp = Compare(&pElement, current_node);
      if (cmp < 0) {
        current_node = current_node->pLeft;
      } else if (cmp > 0) {
        current_node = current_node->pRight;
      } else return *current_node;
    }

  }

  bool remove(const T &element) {}

  void clear()
  {
    m_Memory.clear();
    m_pRoot = nullptr;
  }

private:
  void small_left_rotation(leaf *node) {
    assert(node->pRight!= nullptr);
  }
  void small_right_rotation(leaf *node) {
    assert(node->pLeft!= nullptr);
  }
  void big_left_rotation(leaf *node) {
    assert(node->pRight!= nullptr);
    assert(node->pRight->pLeft!= nullptr);
  }
  void big_right_rotation(leaf *node) {
    assert(node->pLeft!= nullptr);
    assert(node->pLeft->pRight!= nullptr);
  }
  leaf *m_pRoot;
  CMemoryManager<leaf> m_Memory;
};

}; // namespace lab618

#endif // #define AVL_HEAD_H_2023_03_30
