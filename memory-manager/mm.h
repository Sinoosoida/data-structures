#ifndef MEMORY_MANAGER_HEAD_H_2023_02_10
#define MEMORY_MANAGER_HEAD_H_2023_02_10

#include "vector"
#include <cstring>
#include <iostream>
#include <new>

#define free_color "\33[38;5;0;48;5;255m"
#define allocated_color "\33[38;5;255;48;5;0m"
#define reset "\33[m"

template <class TYPE> inline void ConstructElements(TYPE *pElements) {
  memset(reinterpret_cast<void *>(pElements), 0, sizeof(TYPE));
  ::new (reinterpret_cast<void *>(pElements)) TYPE;
}

template <class TYPE> inline void DestructElements(TYPE *pElements) {
  pElements->~TYPE();
  memset(reinterpret_cast<void *>(pElements), 0, sizeof(TYPE));
}

size_t step(size_t T_size, size_t int_size = sizeof(int)) {
  if (int_size > T_size) {
    return (int_size - 1) / T_size + 1;
  } else {
    return 1;
  }
}

namespace lab618 {
template <class T> class CMemoryManager {
private:
  struct block {
    // ������ ������ �����
    T *pdata;
    // ����� ���������� �����
    block *pnext;
    // ������ ��������� ������
    int firstFreeIndex;
    // ����� ����������� �����
    int usedCount;
  };

public:
  //  class CException {
  //    // �� ������������
  //  public:
  //    CException() {}
  //  };

public:
  /**
    _default_block_size - ���������� ��������� � ����� ������
    isDeleteElementsOnDestruct - ���������� �������� � ����������� ��������� ���
    ��������� �� ������� ��������������� �������� deleteObject ���������.
  */
  CMemoryManager(int _default_block_size,
                 bool isDeleteElementsOnDestruct = false)
      : m_blkSize(_default_block_size), m_pBlocks(nullptr),
        m_pCurrentBlk(nullptr),
        m_isDeleteElementsOnDestruct(isDeleteElementsOnDestruct) {}

  void print() {

    std::string separator = "|";
    std::string used = "X";
    std::string space = " ";
    size_t elements_per_line = 20;

    for (auto block = m_pBlocks; block != nullptr; block = block->pnext) {
      std::cout << "Block:\t" << block << "\tpoints to:\t" << block->pnext
                << std::endl;
      std::cout << "Used:\t" << block->usedCount << "/" << m_blkSize
                << "\tFirst free index:\t" << block->firstFreeIndex
                << std::endl;

      std::vector<bool> mask(m_blkSize, false);

      auto i = block->firstFreeIndex;
      while (i != -1) {
        mask[i] = true;
        //        std::cout << (reinterpret_cast<int *>(block->pdata +
        //                                              i * step(sizeof(T))))
        //                  << std::endl;
        i = *(reinterpret_cast<int *>(block->pdata + i * step(sizeof(T))));
      }

      for (int i = 0; i < m_blkSize; i += elements_per_line) {

        std::cout << separator;
        for (int j = i; j < i + elements_per_line && j < m_blkSize; j++) {
          for (int k = std::to_string(j).length();
               k < std::to_string(m_blkSize - 1).length(); k++) {
            std::cout << space;
          }
          std::cout << j << separator;
        }
        std::cout << std::endl;

        std::cout << separator;
        for (int j = i; j < i + elements_per_line && j < m_blkSize; j++) {
          if (mask[j]) {
            printf(free_color);
            int out =
                *(reinterpret_cast<int *>(block->pdata + j * step(sizeof(T))));
            for (int k = std::to_string(out).length();
                 k < std::to_string(m_blkSize - 1).length(); k++) {
              std::cout << space;
            }
            std::cout << out;
            printf(reset);
            std::cout << separator;
          } else {
            printf(allocated_color);
            for (int k = 0; k < std::to_string(m_blkSize - 1).length(); k++) {
              std::cout << used;
            }
            printf(reset);
            std::cout << separator;
          }
        }
        std::cout << std::endl;
      }
    }
  }

  virtual ~CMemoryManager() { clear(); }

  // �������� ����� ������ �������� �� ���������
  T *newObject() { // SGFM

    if (m_pBlocks == nullptr) { // ���� ������ ��� - ������
      m_pBlocks = newBlock();
      m_pCurrentBlk = m_pBlocks;
    } else { // ���� ����� ����, �� ���������� ��
      auto tmp = m_pBlocks;
      while ((tmp != nullptr) && (tmp->usedCount == m_blkSize)) {
        tmp = tmp->pnext;
      }
      if (tmp == nullptr) { // ���� ���������� ����� �� �������, �� ������
        while (m_pCurrentBlk->pnext != nullptr) {
          m_pCurrentBlk = m_pCurrentBlk->pnext;
        }
        m_pCurrentBlk->pnext = newBlock();
        m_pCurrentBlk = m_pCurrentBlk->pnext;
      }
    }

    // ������ � ��� ���� ��������� ����

    T *FreeSpace =
        m_pCurrentBlk->pdata + m_pCurrentBlk->firstFreeIndex * step(sizeof(T));
    int firstFreeIndex = *reinterpret_cast<int *>(
        FreeSpace); // �����, ��� �������� � ��������� ������ - �������
                    // ���������� ���������� ��������
    m_pCurrentBlk->firstFreeIndex = firstFreeIndex;
    m_pCurrentBlk->usedCount++;
    ConstructElements(FreeSpace);
    return reinterpret_cast<T *>(FreeSpace);
  }

  // ���������� ������� � ���������
  bool deleteObject(T *p) {

    // ����� ����, ��� ����� �������
    block *blk = m_pBlocks;
    while (blk &&
           !(p >= blk->pdata &&
             p < blk->pdata +
                     m_blkSize * step(sizeof(T)))) { // ���������� ��� �����
      blk = blk->pnext;
    }

    if (blk == nullptr) { // ������� �� �����
      return false;
    }

    if (p >= blk->pdata && p < blk->pdata + m_blkSize * step(sizeof(T)) &&
        (p - blk->pdata) % step(sizeof(T)) ==
            0) { // ����� ����, ������ �������� ���������� ���������

      int FreeIndex = blk->firstFreeIndex; // ���������� ������ �����
      while (FreeIndex != -1) {
        if ((blk->pdata + FreeIndex * step(sizeof(T))) ==
            p) { // ���� ��������� ��������� �� ������ �����
          return false; // �� ������� ��� ��� �����, �������� ������� �� ������
        }
        FreeIndex =
            *reinterpret_cast<int *>(blk->pdata + FreeIndex * step(sizeof(T)));
      }

      // ������� � �����, � �� �� �����. ����� �������.
      DestructElements(p); // ��������, ����� �������
      auto second_free_index = blk->firstFreeIndex;
      blk->firstFreeIndex = (p - blk->pdata) / step(sizeof(T));
      *(reinterpret_cast<int *>((blk->pdata) +
                                (blk->firstFreeIndex) * step(sizeof(T)))) =
          second_free_index;
      blk->usedCount--;
      return true;
    } else {
      return false;
    }
  }

  // ������� ������, ������� �� m_isDeleteElementsOnDestruct
  void clear() {
    if (m_pBlocks ==
        nullptr) { // ���� ����� �� ���� ��������, �� � �������� ������
      return;
    }
    if (m_isDeleteElementsOnDestruct) { // ���� �� ����� ������� �������� ���
                                        // ��������
      while (m_pBlocks != nullptr) { // ������� ��� ��������
        block *tmp = m_pBlocks->pnext;

        deleteBlock(m_pBlocks);
        m_pBlocks = tmp;
      }
    } else { // ���� ������� �������� ��� �������� �� �� �����

      bool *emptyMask = new bool[m_blkSize]; // ������ �����, ��� ��������� ���
                                             // ����������� � ��������� �������

      while (m_pBlocks != nullptr) {
        block *tmp = m_pBlocks->pnext;
        for (int i = 0; i < m_blkSize; i++) {
          emptyMask[i] = false;
        }
        int FreeSpaceIndex = m_pBlocks->firstFreeIndex;
        while (FreeSpaceIndex != -1) {
          emptyMask[FreeSpaceIndex] = true;
          FreeSpaceIndex = *(reinterpret_cast<int *>(
              m_pBlocks->pdata + FreeSpaceIndex * step(sizeof(T))));
        }
        deleteBlock(m_pBlocks, emptyMask);
        m_pBlocks = tmp;
      }

      delete[] emptyMask;
    }
    m_pBlocks = nullptr;
    m_pCurrentBlk = nullptr;
  }

private:
  // ������� ����� ���� ������. ����������� � newObject
  block *newBlock() {
    auto *new_block = new block;
    new_block->usedCount = 0;
    new_block->firstFreeIndex = 0;
    new_block->pnext = nullptr;
    new_block->pdata = new T[m_blkSize * step(sizeof(T))];
    for (int i = 0; i < m_blkSize; i++) {
      *(reinterpret_cast<int *>(new_block->pdata + i * step(sizeof(T)))) =
          i + 1;
    }
    *(reinterpret_cast<int *>(new_block->pdata +
                              (m_blkSize - 1) * step(sizeof(T)))) = -1;

    return new_block;
  }

  // ���������� ������ ����� ������. ����������� � clear
  void deleteBlock(block *p, bool *emptyMask = nullptr) {
    //            std::cout << p.pdata << stD::endl;
    if (m_isDeleteElementsOnDestruct) {
      delete[] p->pdata;
      delete p;
    } else {
      for (int i = 0; i < m_blkSize; i++) {
        if (!emptyMask[i]) {
          DestructElements(p->pdata + i * step(sizeof(T)));
        }
      }

      delete[] p->pdata;
      delete p;
    }
  }

  // ������ �����, ������� ��������� ���� T ����� ��������� � ����� �����
  int m_blkSize;
  // ������ ������������ ������ ������
  block *m_pBlocks;
  // ������� ����
  block *m_pCurrentBlk;
  // ������� �� �������� ��� ������������
  bool m_isDeleteElementsOnDestruct;
};
} // namespace lab618

#endif // #define MEMORY_MANAGER_HEAD_H_2023_02_10
