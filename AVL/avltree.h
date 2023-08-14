#ifndef AVL_HEAD_H_2023_03_30
#define AVL_HEAD_H_2023_03_30

#include "assert.h"
#include "mm.h"
#include "deque"
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
  CAVLTree(int defaultBlockSize)
      : m_pRoot(nullptr), m_Memory(defaultBlockSize, true) {}

  void print() { print(m_pRoot, std::deque<bool>()); }

  void print(leaf *node, std::deque<bool> path) {
    if (node) {
      auto path_copy = path;
      path_copy.push_back(false);
      print(node->pRight, path_copy);

      for (int i = 0; i < int(path.size()) - 1; ++i) {
        if (path[i] != path[i + 1]) {
          std::cout << "│   ";
        } else {
          std::cout << "    ";
        }
      }

      if (!path.empty()) {
        if (path[path.size() - 1] == true) {
          std::cout << "└── ";
        }
        if (path[path.size() - 1] == false) {
          std::cout << "┌── ";
        }
      }

      std::cout << *(node->pData) << "\t" << (node->pData) << " ("
                << node->balanceFactor << ")" << std::endl;

      path_copy = path;
      path_copy.push_back(true);
      print(node->pLeft, path_copy);
    }
  }

  int size() { return size(m_pRoot); }
  int size(leaf *root) {
    if (root == nullptr) {
      return 0;
    }
    return size(root->pRight) + size(root->pLeft) + 1;
  }

  int height(leaf *node) {
    if (node == nullptr) {
      return 0;
    }
    auto left_height = height(node->pLeft);
    auto right_height = height(node->pRight);

    if (right_height >= left_height) {
      return right_height + 1;
    } else {
      return left_height + 1;
    }
  }

  bool check() { return check(m_pRoot); }
  bool check(leaf *node) {
    if (node == nullptr) {
      return true;
    }

    auto left_height = height(node->pLeft);
    auto right_height = height(node->pRight);

    if (left_height - right_height != node->balanceFactor) {
      return false;
    }

    auto left_check = check(node->pLeft);
    auto right_check = check(node->pRight);

    if ((!left_check) || (!right_check)) {
      return false;
    } else {
      return true;
    }
  }

  enum Direction { Left, Right, None };

  bool add(T *pElement) {
    leaf *newLeaf = m_Memory.newObject();
    newLeaf->pData = pElement;
    newLeaf->pLeft = nullptr;
    newLeaf->pRight = nullptr;
    newLeaf->balanceFactor = 0;

    if (m_pRoot == nullptr) {
      m_pRoot = newLeaf;
      return true;
    }

    // найдём, куда можно сохранить вершину
    leaf *current_node = m_pRoot;
    std::deque<std::pair<leaf *, Direction>> path;

    while (current_node != nullptr) {
      int cmp = Compare(pElement, current_node->pData);
      if (cmp < 0) {
        path.push_back(std::make_pair(current_node, Left));
        current_node = current_node->pLeft;
      } else if (cmp > 0) {
        path.push_back(std::make_pair(current_node, Right));
        current_node = current_node->pRight;
      } else {
        return false;
      }
    }

    leaf *last_node = path[path.size() - 1].first;
    if (path[path.size() - 1].second == Left) {
      last_node->pLeft = newLeaf;
    } else {
      last_node->pRight = newLeaf;
    }

    path.push_back(std::make_pair(newLeaf, None));

    // Раставляем балансы
    for (int i = path.size() - 2; i >= 0; i--) {

      auto current_element = path[i].first;
      auto way = path[i].second;
      //      auto underneath_element = path[i + 1];
      leaf *overhead_element = nullptr;
      if (i > 0) {
        overhead_element = path[i - 1].first;
      }

      if (way == Right) {
        current_element->balanceFactor--;
      } else {
        current_element->balanceFactor++;
      }

      if (current_element->balanceFactor == 2 ||
          current_element->balanceFactor == -2) {
        rotate(current_element, overhead_element);

        if (i > 0) {
          auto overhead_element_way = path[i - 1].second;
          if (overhead_element_way == Left) {
            current_element = overhead_element->pLeft;
          } else {
            current_element = overhead_element->pRight;
          }
        }
      }

      if (current_element->balanceFactor == 0) {
        break;
      }
    }
    return true;
  }

  bool update(T *pElement) {
    auto res = remove(*pElement);
    add(pElement);
    return res;
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
        return current_node->pData;
    }
    return nullptr;
  }

  bool remove(const T &element) {

    // найдём вершину
    leaf *current_node = m_pRoot;
    std::deque<std::pair<leaf *, Direction>> path;

    while (current_node != nullptr) {
      int cmp = Compare(&element, current_node->pData);
      if (cmp < 0) {
        path.push_back(std::make_pair(current_node, Left));
        current_node = current_node->pLeft;
      } else if (cmp > 0) {
        path.push_back(std::make_pair(current_node, Right));
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

      // лист мы сразу удаляем
      path.push_back(std::make_pair(current_node, None));
      remove(path);

    } else {

      // находим ближайшую по значению вершину
      leaf *nearest_node = nullptr;
      if (current_node->pLeft != nullptr) {

        path.push_back(std::make_pair(current_node, Left));
        nearest_node = current_node->pLeft;

        while (nearest_node != nullptr) {
          path.push_back(std::make_pair(nearest_node, Right));
          nearest_node = nearest_node->pRight;
        }

        nearest_node = path[path.size() - 1].first;
        path[path.size() - 1].second = None;
        std::swap(nearest_node->pData, current_node->pData);

        if (nearest_node->pLeft != nullptr) {
          path[path.size() - 1].second = Left;
          path.push_back(std::make_pair(nearest_node->pLeft, None));
          std::swap(nearest_node->pLeft->pData, nearest_node->pData);
          remove(path);
        } else {
          remove(path);
        }

      } else {
        path.push_back(std::make_pair(current_node, Right));
        nearest_node = current_node->pRight;

        while (nearest_node != nullptr) {
          path.push_back(std::make_pair(nearest_node, Left));
          nearest_node = nearest_node->pLeft;
        }

        nearest_node = path[path.size() - 1].first;
        path[path.size() - 1].second = None;
        std::swap(nearest_node->pData, current_node->pData);

        if (nearest_node->pRight != nullptr) {
          path[path.size() - 1].second = Right;
          path.push_back(std::make_pair(nearest_node->pRight, None));
          std::swap(nearest_node->pRight->pData, nearest_node->pData);
          remove(path);
        } else {
          remove(path);
        }
      }
    }
    return true;
  }

  void clear() {
    m_Memory.clear();
    m_pRoot = nullptr;
  }

private:
  void remove(std::deque<std::pair<leaf *, Direction>> &path) {

    // удаляем лист
    m_Memory.deleteObject(path[path.size() - 1].first);

    // удаляем указатель на него в дереве
    if (path.size() >= 2) {
      if (path[path.size() - 2].second == Left) {
        path[path.size() - 2].first->pLeft = nullptr;
      } else if (path[path.size() - 2].second == Right) {
        path[path.size() - 2].first->pRight = nullptr;
      } else {
        throw;
      }
    } else {
      m_pRoot = nullptr;
    }

    // проставляем балансы
    for (int i = path.size() - 2; i >= 0; i--) {

      auto current_element = path[i].first;
      auto way = path[i].second;
      //      auto underneath_element = path[i + 1];
      leaf *overhead_element = nullptr;
      if (i > 0) {
        overhead_element = path[i - 1].first;
      }

      if (way == Right) {
        current_element->balanceFactor++;
      }
      if (way == Left) {
        current_element->balanceFactor--;
      }

      if (current_element->balanceFactor == 2 ||
          current_element->balanceFactor == -2) {
        rotate(current_element, overhead_element);

        // меняем так же и текущий элемент
        if (i > 0) {
          auto overhead_element_way = path[i - 1].second;
          if (overhead_element_way == Left) {
            current_element = overhead_element->pLeft;
          } else {
            current_element = overhead_element->pRight;
          }
        }
      }

      if (current_element->balanceFactor == 1 ||
          current_element->balanceFactor == -1) {
        break;
      }
    }
  }

  void rotate(leaf *node, leaf *overhead_node) {
    if (node->balanceFactor == 2) {
      // левое поддерево тяжелее, нужно правое вращение
      if (node->pLeft->balanceFactor >= 0) {
        // малое правое вращение
        small_right_rotation(node, overhead_node);
      } else {
        // большое правое вращение
        big_right_rotation(node, overhead_node);
      }
    } else if (node->balanceFactor == -2) {
      // правое поддерево тяжелее, нужно левое вращение
      if (node->pRight->balanceFactor <= 0) {
        // малое левое вращение
        small_left_rotation(node, overhead_node);
      } else {
        // большое левое вращение
        big_left_rotation(node, overhead_node);
      }
    }
  }

  void small_left_rotation(leaf *node, leaf *overhead_element) {
    assert(node->pRight != nullptr);

    // меняем ребёнка на нового
    if (overhead_element != nullptr) {
      if (overhead_element->pLeft == node) {
        overhead_element->pLeft = node->pRight;
      } else if (overhead_element->pRight == node) {
        overhead_element->pRight = node->pRight;
      }
    } else {
      m_pRoot = m_pRoot->pRight;
    }

    // поворачиваем вершины
    auto tmp = node->pRight;
    node->pRight = node->pRight->pLeft;
    tmp->pLeft = node;

    // расставляем балансы
    tmp->balanceFactor++;
    node->balanceFactor = -(tmp->balanceFactor);
  }

  void small_right_rotation(leaf *node, leaf *overhead_element) {
    assert(node->pLeft != nullptr);

    // меняем ребёнка на нового
    if (overhead_element != nullptr) {
      if (overhead_element->pLeft == node) {
        overhead_element->pLeft = node->pLeft;
      } else if (overhead_element->pRight == node) {
        overhead_element->pRight = node->pLeft;
      }
    } else {
      m_pRoot = m_pRoot->pLeft;
    }

    // поворачиваем вершины
    auto tmp = node->pLeft;
    node->pLeft = node->pLeft->pRight;
    tmp->pRight = node;

    // расставляем балансы
    tmp->balanceFactor--;
    node->balanceFactor = -(tmp->balanceFactor);
  }

  void big_left_rotation(leaf *node, leaf *overhead_element) {
    assert(node->pRight != nullptr);
    assert(node->pRight->pLeft != nullptr);

    auto c_balance_factor = node->pRight->pLeft->balanceFactor;

    auto a = node;
    auto b = node->pRight;
    auto c = node->pRight->pLeft;

    small_right_rotation(node->pRight, node);
    small_left_rotation(node, overhead_element);

    c->balanceFactor = 0;
    a->balanceFactor = (c_balance_factor == -1);
    b->balanceFactor = -(c_balance_factor == 1);
  }

  void big_right_rotation(leaf *node, leaf *overhead_element) {
    assert(node->pLeft != nullptr);
    assert(node->pLeft->pRight != nullptr);

    auto c_balance_factor = node->pLeft->pRight->balanceFactor;

    auto a = node;
    auto b = node->pLeft;
    auto c = node->pLeft->pRight;

    small_left_rotation(node->pLeft, node);
    small_right_rotation(node, overhead_element);

    c->balanceFactor = 0;
    a->balanceFactor = -(c_balance_factor == 1);
    b->balanceFactor = (c_balance_factor == -1);
  }

  leaf *m_pRoot;
  CMemoryManager<leaf> m_Memory;
};
}; // namespace lab618

#endif // #define AVL_HEAD_H_2023_03_30