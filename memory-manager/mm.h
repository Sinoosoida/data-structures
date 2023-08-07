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
    // Массив данных блока
    T *pdata;
    // Адрес следующего блока
    block *pnext;
    // Первая свободная ячейка
    int firstFreeIndex;
    // Число заполненных ячеек
    int usedCount;
  };

public:
  //  class CException {
  //    // Не используется
  //  public:
  //    CException() {}
  //  };

public:
  /**
    _default_block_size - количество элементов в блоке данных
    isDeleteElementsOnDestruct - уничтожать элементы в деструкторе менеджера или
    проверять на наличие неосвобожденных функцией deleteObject элементов.
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

  // Получить адрес нового элемента из менеджера
  T *newObject() { // SGFM

    if (m_pBlocks == nullptr) { // если блоков нет - создаём
      m_pBlocks = newBlock();
      m_pCurrentBlk = m_pBlocks;
    } else { // если блоки есть, то перебираем их
      auto tmp = m_pBlocks;
      while ((tmp != nullptr) && (tmp->usedCount == m_blkSize)) {
        tmp = tmp->pnext;
      }
      if (tmp == nullptr) { // если свободного блока не нашлось, то создаём
        while (m_pCurrentBlk->pnext != nullptr) {
          m_pCurrentBlk = m_pCurrentBlk->pnext;
        }
        m_pCurrentBlk->pnext = newBlock();
        m_pCurrentBlk = m_pCurrentBlk->pnext;
      }
    }

    // теперь у нас есть свободный блок

    T *FreeSpace =
        m_pCurrentBlk->pdata + m_pCurrentBlk->firstFreeIndex * step(sizeof(T));
    int firstFreeIndex = *reinterpret_cast<int *>(
        FreeSpace); // узнаём, что хранится в свободной ячейке - позиция
                    // следующего свободного элемента
    m_pCurrentBlk->firstFreeIndex = firstFreeIndex;
    m_pCurrentBlk->usedCount++;
    ConstructElements(FreeSpace);
    return reinterpret_cast<T *>(FreeSpace);
  }

  // Освободить элемент в менеджере
  bool deleteObject(T *p) {

    // найдём блок, где лежит элемент
    block *blk = m_pBlocks;
    while (blk &&
           !(p >= blk->pdata &&
             p < blk->pdata +
                     m_blkSize * step(sizeof(T)))) { // перебираем все блоки
      blk = blk->pnext;
    }

    if (blk == nullptr) { // нужного не нашли
      return false;
    }

    if (p >= blk->pdata && p < blk->pdata + m_blkSize * step(sizeof(T)) &&
        (p - blk->pdata) % step(sizeof(T)) ==
            0) { // нашли блок, внутри которого содержится указатель

      int FreeIndex = blk->firstFreeIndex; // перебираем пустые места
      while (FreeIndex != -1) {
        if ((blk->pdata + FreeIndex * step(sizeof(T))) ==
            p) { // если указатель указывает на пустое место
          return false; // то элемент уже был удалён, повторно удалить не сможем
        }
        FreeIndex =
            *reinterpret_cast<int *>(blk->pdata + FreeIndex * step(sizeof(T)));
      }

      // Элемент в блоке, и он не удалён. Можно удалять.
      DestructElements(p); // вероятно, можно удалить
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

  // Очистка данных, зависит от m_isDeleteElementsOnDestruct
  void clear() {
    if (m_pBlocks ==
        nullptr) { // если блоки не были выделены, то и отчищать нечего
      return;
    }
    if (m_isDeleteElementsOnDestruct) { // если мы хотим удалять элементы при
                                        // отчистке
      while (m_pBlocks != nullptr) { // удаляем все элементы
        block *tmp = m_pBlocks->pnext;

        deleteBlock(m_pBlocks);
        m_pBlocks = tmp;
      }
    } else { // если удалять элементы при отчистке мы не хотим

      bool *emptyMask = new bool[m_blkSize]; // создаём маску, где обозначим все
                                             // заполненные и свободные участки

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
  // Создать новый блок данных. Применяется в newObject
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

  // Освободить память блока данных. Применяется в clear
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

  // Размер блока, сколько элементов типа T могут храниться в одном блоке
  int m_blkSize;
  // Начало односвязного списка блоков
  block *m_pBlocks;
  // Текущий блок
  block *m_pCurrentBlk;
  // Удалять ли элементы при освобождении
  bool m_isDeleteElementsOnDestruct;
};
} // namespace lab618

#endif // #define MEMORY_MANAGER_HEAD_H_2023_02_10
