#ifndef AVL_HEAD_H_2023_03_30
#define AVL_HEAD_H_2023_03_30

#include "assert.h"
#include "mm.h"
#include "queue"
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

  void print() { print(m_pRoot, std::vector<bool>()); }

  void print(leaf *node, std::vector<bool> path) {
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
    std::vector<std::pair<leaf *, Direction>> path;

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
    } else if (path[path.size() - 1].second == Right) {
      last_node->pRight = newLeaf;
    } else {
      throw; // TODO убрать когда всё заработает
    }
    //    leaf *last_node = path[path.size() - 1];
    //    int cmp = Compare(pElement, last_node->pData);
    //    if (cmp < 0) {
    //      last_node->pLeft = newLeaf;
    //    } else if (cmp > 0) {
    //      last_node->pRight = newLeaf;
    //    } else {
    //      throw; // TODO убрать когда всё заработает
    //    }

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
      } else if (way == Left) {
        current_element->balanceFactor++;
      } else {
        throw; // TODO убрать когда всё заработает
      }

      if (current_element->balanceFactor == 2 ||
          current_element->balanceFactor == -2) {
        rotate(current_element, overhead_element);
      }

      if (current_element->balanceFactor == 0) {
        break;
      }
    }
    return true;
  }

  bool update(T *pElement) {
    //    auto ret = this->remove(*pElement);
    // TODO
    return false;
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
    std::vector<std::pair<leaf *, Direction>> path;

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
      remove(current_node, path);

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
        std::swap(nearest_node->pData, current_node->pData);

        if (nearest_node->pLeft != nullptr) {
          path.push_back(std::make_pair(nearest_node->pLeft, None));
          std::swap(nearest_node->pLeft->pData, nearest_node->pData);
          remove(nearest_node->pLeft, path);
        } else {
          remove(nearest_node, path);
        }

      } else {
        path.push_back(std::make_pair(current_node, Right));
        nearest_node = current_node->pRight;

        while (nearest_node != nullptr) {
          path.push_back(std::make_pair(nearest_node, Left));
          nearest_node = nearest_node->pLeft;
        }

        nearest_node = path[path.size() - 1].first;
        std::swap(nearest_node->pData, current_node->pData);

        if (nearest_node->pRight != nullptr) {
          path.push_back(std::make_pair(nearest_node->pRight, None));
          std::swap(nearest_node->pRight->pData, nearest_node->pData);
          remove(nearest_node->pRight, path);
        } else {
          remove(nearest_node, path);
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
  void remove(leaf *element, std::vector<std::pair<leaf *, Direction>> &path) {
    assert(element->pLeft == nullptr);
    assert(element->pRight == nullptr);
    assert(path[path.size() - 1].first ==
           element); // TODO убрать element когда всё заработает

    //удаляем лист
    m_Memory.deleteObject(element);

    //удаляем указатель на него в дереве
    if (path.size() >= 2) {
      if (path[path.size() - 2].second == Left) {
        path[path.size() - 2].first->pLeft = nullptr;
      } else if (path[path.size() - 2].second == Right) {
        path[path.size() - 2].first->pRight = nullptr;
      } else {
        throw;
      }
    }

    //проставляем балансы
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
      } else if (way == Left) {
        current_element->balanceFactor--;
      } else {
        throw; // TODO убрать когда всё заработает
      }

      if (current_element->balanceFactor == 2 ||
          current_element->balanceFactor == -2) {
        rotate(current_element, overhead_element);
      }

      if (current_element->balanceFactor == 1 ||
          current_element->balanceFactor == -1) {
        break;
      }
    }
  }

  void rotate(leaf *node, leaf *overhead_node) { // TODO требует проыерки
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

    // меняем так node на новый элемент в path
    node = node->pRight;
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

    // меняем так node на новый элемент в path
    node = node->pLeft;
  }

  void big_left_rotation(leaf *node, leaf *overhead_element) {
    assert(node->pRight != nullptr);
    assert(node->pRight->pLeft != nullptr);

    small_right_rotation(node->pRight, node);
    small_left_rotation(node, overhead_element);
  }

  void big_right_rotation(leaf *node, leaf *overhead_element) {
    assert(node->pLeft != nullptr);
    assert(node->pLeft->pRight != nullptr);

    small_left_rotation(node->pLeft, node);
    small_right_rotation(node, overhead_element);
  }

  leaf *m_pRoot;
  CMemoryManager<leaf> m_Memory;
};
}; // namespace lab618

#endif // #define AVL_HEAD_H_2023_03_30
