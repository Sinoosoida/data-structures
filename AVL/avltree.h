#ifndef AVL_HEAD_H_2023_03_30
#define AVL_HEAD_H_2023_03_30

#include "assert.h"
#include "mm.h"
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
    m_Memory = CMemoryManager<T>(defaultBlockSize, true);
  }

  virtual ~CAVLTree() { clear(); }

  bool add(T *pElement) {
    leaf* newLeaf = m_Memory.newObject();
    newLeaf->pData = pElement;
    newLeaf->pLeft = nullptr;
    newLeaf->pRight = nullptr;
    newLeaf->balanceFactor = 0;

    if (m_pRoot == nullptr) {
      m_pRoot = newLeaf;
      return true;
    }

    //найдём, куда можно сохранить вершину
    leaf *current_node = m_pRoot;
    std::vector<leaf *> path;

    while (current_node != nullptr) {
      path.push_back(current_node);
      int cmp = Compare(&pElement, current_node->pData);
      if (cmp < 0) {
        current_node = current_node->pLeft;
      } else if (cmp > 0) {
        current_node = current_node->pRight;
      } else {
        return false;
      }
    }

    auto last_node = path[path.size()-1];
    int cmp = Compare(&pElement, last_node);
    if (cmp < 0) {
      last_node->pLeft = newLeaf;
    } else if (cmp > 0) {
      last_node->pRight = newLeaf;
    } else {
      throw; //TODO убрать когда всё заработает
    }

    path.push_back(newLeaf);

    for (int i = path.size() - 2; i >= 0; i--) {//TODO дописать

      auto current_element = path[i];
      auto previous_element = path[i + 1];

      if (current_element->pRight == previous_element) {
        current_element->balanceFactor--;
      } else if (current_element->pLeft == previous_element) {
        current_element->balanceFactor++;
      } else {
        throw; // TODO убрать когда всё заработает
      }

      if (current_element->balanceFactor == 2 ||
          current_element->balanceFactor == -2) {
        rotate(current_element);
      }

      if (current_element->balanceFactor == 0) {
        break;
      }
    }
    return true;
  }

  bool update(T *pElement) {
    //TODO
  }

  T *find(const T &pElement) {
    leaf *current_node = m_pRoot;
    while (current_node != nullptr) {
      int cmp = Compare(&pElement, current_node->pData);
      if (cmp < 0) {
        current_node = current_node->pLeft;
      } else if (cmp > 0) {
        current_node = current_node->pRight;
      } else
        return *current_node;
    }
    return current_node->pData;
  }

  bool remove(const T &element) {

    // сначала найдём, где хранится элемент
    leaf *current_node = m_pRoot;
    std::vector<leaf *> path;

    while (current_node != nullptr) {
      path.push_back(current_node);

      int cmp = Compare(&element, current_node->pData);
      if (cmp < 0) {
        current_node = current_node->pLeft;
      } else if (cmp > 0) {
        current_node = current_node->pRight;
      } else {
        break;
      }
    }

    // не нашли
    if (current_node == nullptr) {
      return false;
    }

    if (current_node->pRight == nullptr && current_node->pLeft == nullptr) {
      remove(current_node, path);
    } else {

      // находим ближайшую по значению вершину
      leaf *nearest_leaf = nullptr;
      if (current_node->pLeft != nullptr) {
        nearest_leaf = current_node->pLeft;

        while (nearest_leaf->pRight != nullptr) {
          path.push_back(nearest_leaf);
          nearest_leaf = nearest_leaf->pRight;
        }

        std::swap(nearest_leaf->pData, current_node->pData);

        if (nearest_leaf->pLeft != nullptr) {
          path.push_back(nearest_leaf->pLeft);
          std::swap(nearest_leaf->pLeft->pData, nearest_leaf->pData);
        }

        remove(nearest_leaf->pLeft, path);
      } else {
        nearest_leaf = current_node->pRight;

        while (nearest_leaf->pLeft != nullptr) {
          path.push_back(nearest_leaf);
          nearest_leaf = nearest_leaf->pLeft;
        }

        std::swap(nearest_leaf->pData, current_node->pData);

        if (nearest_leaf->pRight != nullptr) {
          path.push_back(nearest_leaf->pRight);
          std::swap(nearest_leaf->pRight->pData, nearest_leaf->pData);
        }

        remove(nearest_leaf->pRight, path);
      }
    }
  }

  void clear() {
    m_Memory.clear();
    m_pRoot = nullptr;
  }

private:

  void remove(leaf *element, std::vector<leaf *> path) {
    assert(element->pLeft == nullptr);
    assert(element->pRight == nullptr);
    assert(path[path.size() - 1] ==
           element); // TODO убрать element когда всё заработает

    m_Memory.deleteObject(element);

    for (int i = path.size() - 2; i >= 0; i--) {//TODO дописать

      auto current_element = path[i];
      auto previous_element = path[i + 1];

      if (current_element->pRight == previous_element) {
        current_element->balanceFactor++;
      } else if (current_element->pLeft == previous_element) {
        current_element->balanceFactor--;
      } else {
        throw; // TODO убрать когда всё заработает
      }

      if (current_element->balanceFactor == 2 ||
          current_element->balanceFactor == -2) {
        rotate(current_element);
      }

      if (current_element->balanceFactor == 1 ||
          current_element->balanceFactor == -1) {
        break;
      }
    }
  }

  void rotate(leaf *node, leaf *previous_node) {//TODO требует проыерки
    if (node->balanceFactor == -2) {
      // левое поддерево тяжелее, нужно правое вращение
      if (node->pLeft->balanceFactor <= 0) {
        // малое правое вращение
        small_right_rotation(node, previous_node);
      } else {
        // большое правое вращение
        big_right_rotation(node, previous_node);
      }
    }
    else if (node->balanceFactor == 2) {
      // правое поддерево тяжелее, нужно левое вращение
      if (node->pRight->balanceFactor >= 0) {
        // малое левое вращение
        small_left_rotation(node, previous_node);
      } else {
        // большое левое вращение
        big_left_rotation(node, previous_node);
      }
    }
  }

  void small_left_rotation(leaf *node, leaf *previous_node) {
    assert(node->pRight != nullptr);

    //меняем ребёнка на нового
    if (previous_node != nullptr) {
      if (previous_node->pLeft == node) {
        previous_node->pLeft = node->pRight;
      } else if (previous_node->pRight == node) {
        previous_node->pRight = node->pRight;
      }
    }

    //поворачиваем вершины
    auto tmp = node->pRight;
    node->pRight = node->pRight->pLeft;
    tmp->pLeft = node;

    //расставляем балансы
    tmp->balanceFactor++;
    node->balanceFactor = -(tmp->balanceFactor);
  }

  void small_right_rotation(leaf *node, leaf *previous_node) {
    assert(node->pLeft != nullptr);

    // меняем ребёнка на нового
    if (previous_node != nullptr) {
      if (previous_node->pLeft == node) {
        previous_node->pLeft = node->pLeft;
      } else if (previous_node->pRight == node) {
        previous_node->pRight = node->pLeft;
      }
    }

    // поворачиваем вершины
    auto tmp = node->pLeft;
    node->pLeft = node->pLeft->pRight;
    tmp->pRight = node;

    // расставляем балансы
    tmp->balanceFactor--;
    node->balanceFactor = -(tmp->balanceFactor);
  }

  void big_left_rotation(leaf *node, leaf *previous_node) {
    assert(node->pRight != nullptr);
    assert(node->pRight->pLeft != nullptr);

    small_right_rotation(node->pRight, node);
    small_left_rotation(node, previous_node);
  }

  void big_right_rotation(leaf *node, leaf *previous_node) {
    assert(node->pLeft != nullptr);
    assert(node->pLeft->pRight != nullptr);

    small_left_rotation(node->pLeft, node);
    small_right_rotation(node, previous_node);
  }

  leaf *m_pRoot;
  CMemoryManager<leaf> m_Memory;
};

}; // namespace lab618

#endif // #define AVL_HEAD_H_2023_03_30
