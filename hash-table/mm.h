#ifndef MEMORY_MANAGER_HEAD_H_2023_02_10
#define MEMORY_MANAGER_HEAD_H_2023_02_10

#include "vector"
#include <cstring>
#include <iostream>
#include <new>

// Цвета для вывода в консоль
#define FREE_COLOR "\33[38;5;0;48;5;255m"
#define ALLOCATED_COLOR "\33[38;5;255;48;5;0m"
#define RESET_COLOR "\33[m"

// Вызов конструктора для элемента
template <class TYPE> inline void ConstructElements(TYPE *pElements) {
  memset(reinterpret_cast<void *>(pElements), 0, sizeof(TYPE));
  ::new (reinterpret_cast<void *>(pElements)) TYPE;
}

// Вызов деструктора для элемента
template <class TYPE> inline void DestructElements(TYPE *pElements) {
  pElements->~TYPE();
  memset(reinterpret_cast<void *>(pElements), 0, sizeof(TYPE));
}

/*
 * В моей реализации, пока элемент не был выделен менеджером памяти, он хранит
 * значение типа int, указывающее на индекс слдующего свободного элемента в
 * блоке. Поэтому, если тип данных T меньше чем тип данных int, менеджер памяти
 * не может работать коректно. Для решения этой проблемы используется эта
 * функция, которая указывает, сколько элементов типа T нужно выделить на один
 * элемент типа int, чтобы последний мог туда поместиться.
 */
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
  class CException {
    // Не используется
  public:
    CException() {}
  };

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

  // Вывод информации о блоках в консоль
  void print() {
    std::string separator = "|";
    std::string used = "X";
    std::string space = " ";
    size_t elements_per_line = 20;

    std::cout << "\nMemory Manager Status\n";

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
            printf(FREE_COLOR);
            int out =
                *(reinterpret_cast<int *>(block->pdata + j * step(sizeof(T))));
            for (int k = std::to_string(out).length();
                 k < std::to_string(m_blkSize - 1).length(); k++) {
              std::cout << space;
            }
            std::cout << out;
            printf(RESET_COLOR);
            std::cout << separator;
          } else {
            printf(ALLOCATED_COLOR);
            for (int k = 0; k < std::to_string(m_blkSize - 1).length(); k++) {
              std::cout << used;
            }
            printf(RESET_COLOR);
            std::cout << separator;
          }
        }
        std::cout << std::endl;
      }
    }
  }

  virtual ~CMemoryManager() { clear(); }

  T *newObject() {

    // Если блоков еще нет, создадим первый
    if (m_pBlocks == nullptr) {
      m_pBlocks = newBlock();
      m_pCurrentBlk = m_pBlocks;
    }

    // Ищем незаполненный блок
    else if (m_pCurrentBlk->usedCount == m_blkSize) {

      // Перебираем все блоки в поисках незаполненного
      block *last_block = nullptr;
      block *tmp_block = m_pCurrentBlk->pnext;

      if (!tmp_block) {
        tmp_block = m_pBlocks;
        last_block = m_pCurrentBlk;
      }

      while (tmp_block->usedCount == m_blkSize && tmp_block != m_pCurrentBlk) {

        if (!tmp_block->pnext) {
          last_block = tmp_block;
          tmp_block = m_pBlocks;
        } else {
          tmp_block = tmp_block->pnext;
        }
      }

      // Если нашли незаполненный, сохраняем его
      if (tmp_block != m_pCurrentBlk) {
        m_pCurrentBlk = tmp_block;

        // Иначе создаем новый блок
      } else {
        last_block->pnext = newBlock();
        m_pCurrentBlk = last_block->pnext;
      }
    }

    // Берем первый свободный элемент из текущего блока
    T *FreeSpace =
        m_pCurrentBlk->pdata + m_pCurrentBlk->firstFreeIndex * step(sizeof(T));
    int firstFreeIndex = *(reinterpret_cast<int *>(FreeSpace));

    // Обновляем индекс первого свободного
    m_pCurrentBlk->firstFreeIndex = firstFreeIndex;
    m_pCurrentBlk->usedCount++;

    // Вызываем конструктор элемента
    ConstructElements(FreeSpace);

    return FreeSpace;
  }

  // Поиск блока по указателю на элемент
  block *findBlock(T *elementPtr) {

    block *currentBlock = m_pBlocks;

    while (currentBlock &&
           !(elementPtr >= currentBlock->pdata &&
             elementPtr < currentBlock->pdata + m_blkSize * step(sizeof(T)))) {

      currentBlock = currentBlock->pnext;
    }

    return currentBlock;
  }

  bool deleteObject(T *elementPtr) {

    // Поиск блока
    block *currentBlock = findBlock(elementPtr);

    // Если блок не найден - возвращаем ошибку
    if (!currentBlock) {
      return false;
    }

    // Проверка валидности указателя
    if ((reinterpret_cast<std::byte *>(elementPtr) -
         reinterpret_cast<std::byte *>(currentBlock->pdata)) %
            (sizeof(T) * step(sizeof(T))) !=
        0) {

      return false;
    }

    // Поиск в списке свободных элементов
    int freeIndex = currentBlock->firstFreeIndex;

    while (freeIndex != -1) {

      if (currentBlock->pdata + freeIndex * step(sizeof(T)) == elementPtr) {
        return false;
      }

      freeIndex = *(reinterpret_cast<int *>(currentBlock->pdata +
                                            freeIndex * step(sizeof(T))));
    }

    // Вызов деструктора
    DestructElements(elementPtr);

    // Добавление в список свободных
    auto secondFreeIndex = currentBlock->firstFreeIndex;
    currentBlock->firstFreeIndex =
        (elementPtr - currentBlock->pdata) / step(sizeof(T));

    *(reinterpret_cast<int *>(currentBlock->pdata +
                              currentBlock->firstFreeIndex * step(sizeof(T)))) =
        secondFreeIndex;

    // Уменьшаем счетчик использованных ячеек
    currentBlock->usedCount--;

    return true;
  }

  // Очистка данных, зависит от m_isDeleteElementsOnDestruct
  void clear() {
    if (m_pBlocks == nullptr) {
      return;
    }

    if (m_isDeleteElementsOnDestruct) { // если мы хотим удалять элементы

      bool *empty_mask = new bool[m_blkSize];

      while (m_pBlocks != nullptr) {
        block *tmp = m_pBlocks->pnext;

        for (int i = 0; i < m_blkSize; i++) {
          empty_mask[i] = true;
        }

        // запоминаем свободные ячейки
        int FreeSpaceIndex = m_pBlocks->firstFreeIndex;
        while (FreeSpaceIndex != -1) {
          empty_mask[FreeSpaceIndex] = false;
          FreeSpaceIndex = *(reinterpret_cast<int *>(
              m_pBlocks->pdata + FreeSpaceIndex * step(sizeof(T))));
        }

        // проходимся по занятым элементам
        for (int i = 0; i < m_blkSize; i++) {
          if (empty_mask[i]) {
            auto ptr = m_pBlocks->pdata + i * step(sizeof(T));
            DestructElements(ptr);
          }
        }

        // теперь удаляем блок
        deleteBlock(m_pBlocks);
        m_pBlocks = tmp;
      }

    } else {
      while (m_pBlocks != nullptr) {
        block *tmp = m_pBlocks->pnext;
        deleteBlock(m_pBlocks);
        m_pBlocks = tmp;
      }
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

    // Выделение памяти без вызова конструктора элемента
    auto len = m_blkSize * sizeof(T) * step(sizeof(T));
    new_block->pdata = reinterpret_cast<T *>(::operator new(len));

    // Заполнение индексами свободных элементов
    for (int i = 0; i < m_blkSize; i++) {
      auto ptr = new_block->pdata + i * step(sizeof(T));
      *(reinterpret_cast<int *>(ptr)) = i + 1;
    }

    auto ptr = new_block->pdata + (m_blkSize - 1) * step(sizeof(T));
    *(reinterpret_cast<int *>(ptr)) = -1;

    return new_block;
  }

  // Освободить память блока данных. Применяется в clear
  void deleteBlock(block *p) {
    ::operator delete(p->pdata);
    delete p;
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
