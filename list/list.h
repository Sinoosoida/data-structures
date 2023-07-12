#ifndef TEMPLATES_LIST_2023_02_02
#define TEMPLATES_LIST_2023_02_02

namespace lab618 {
template <class T> class CSingleLinkedList {
private:
  struct leaf {
    // Данные
    T data;
    // Указатель на следующий лист списка
    leaf *pNext;
    leaf(T &_data, leaf *_pNext) : data(_data), pNext(_pNext) {}
  };

public:
  class CIterator {
  public:
    CIterator() : m_pCurrent(0), m_pBegin(0) {}

    CIterator(leaf *p) : m_pCurrent(p), m_pBegin(p) {}

    CIterator(const CIterator &src)
        : m_pCurrent(src.m_pCurrent), m_pBegin(src.m_pBegin) {}

    ~CIterator() = default;

    CIterator &operator=(const CIterator &src) {
      m_pCurrent = src.m_pCurrent;
      m_pBegin = src.m_pBegin;
      return *this;
    }

    bool operator!=(const CIterator &it) const {
      return (m_pCurrent != it.m_pCurrent);
    }

    void operator++() {
      if (!(this->isValid())) {
        throw "Impossible to pop empty list";
      }

      if (m_pCurrent + 1 == m_pBegin) {
        m_pCurrent = m_pBegin;
      } else {
        m_pCurrent = m_pCurrent->pNext;
      }
    }

    T &getData() { return m_pCurrent->data; }

    T &operator*() { return m_pCurrent->data; }

    leaf *getLeaf() { return m_pCurrent; }

    void setLeaf(leaf *p) { m_pCurrent = p; }

    void setLeafPreBegin(leaf *p) {
      // p is a begin
      m_pBegin = p;
      m_pCurrent = m_pBegin - 1;
    }

    bool isValid() { return (m_pCurrent != nullptr); }

  private:
    // храним голову списка, если мы находимся перед началом
    leaf *m_pBegin;
    // храним текущее положение
    leaf *m_pCurrent;
  };

public:
  CSingleLinkedList() : m_pBegin(0), m_pEnd(0) {}

  virtual ~CSingleLinkedList() {
    while (m_pBegin != m_pEnd) {
      auto p = m_pBegin;
      m_pBegin = p->pNext;
      delete p;
    }
    delete m_pBegin;
    m_pBegin = m_pEnd = nullptr;
  }

  void pushBack(T &data) {
    leaf *p = new leaf(data, nullptr);
    if (m_pEnd == nullptr) {
      m_pEnd = m_pBegin = p;
    } else {
      m_pEnd->pNext = p;
      m_pEnd = p;
    }
  }

  void pushFront(T &data) {
    leaf *p = new leaf(data, m_pBegin);
    m_pBegin = p;
    if (m_pEnd == nullptr) {
      m_pEnd = m_pBegin;
    }
  }

  T popFront() {
    if (m_pBegin == nullptr) {
      throw "Impossible to pop empty list";
    }

    auto leaf = m_pBegin;
    m_pBegin = m_pBegin->pNext;
    if (m_pBegin == nullptr) {
      m_pEnd = nullptr;
    }
    auto data = leaf->data;
    delete leaf;
    return data;
  }

  // изменяет состояние итератора. выставляет предыдущую позицию.
  void erase(CIterator &it) {
    if (!it.isValid()) {
      return;
    }

    if (m_pBegin == nullptr) {
      throw "Impossible to pop empty list";
    }

    if (it.getLeaf() != m_pBegin) {

      auto pre_iterator = begin();
      while (pre_iterator.getLeaf()->pNext != it.getLeaf()) {
        ++pre_iterator;
      }

      pre_iterator.getLeaf()->pNext = pre_iterator.getLeaf()->pNext->pNext;

      if (it.getLeaf() == m_pEnd) {
        m_pEnd = pre_iterator.getLeaf();
      }

      delete it.getLeaf();
      it = pre_iterator;
    } else {
      popFront();
      it.setLeafPreBegin(m_pBegin);
    }
  }

  int getSize() {
    if ((m_pBegin == nullptr) || (m_pEnd == nullptr)) {
      return 0;
    }
    int size = 1;
    leaf *p = m_pBegin;
    while (p != m_pEnd) {
      p = p->pNext;
      size++;
    }
    return size;
  }

  void clear() {
    while (m_pBegin != m_pEnd) {
      auto p = m_pBegin;
      m_pBegin = p->pNext;
      delete p;
    }
    delete m_pBegin;
    m_pBegin = m_pEnd = nullptr;
  }

  CIterator begin() { return CIterator(m_pBegin); }

private:
  // храним голову и хвост списка
  leaf *m_pBegin, *m_pEnd;
};

template <class T> class CDualLinkedList {
private:
  struct leaf {
    // Данные
    T data;
    // Указатель на предыдущий / следующий лист списка
    leaf *pNext, *pPrev;
    leaf(T &_data, leaf *_pPrev, leaf *_pNext)
        : data(_data), pPrev(_pPrev), pNext(_pNext) {}
  };

public:
  class CIterator {
  public:
    CIterator() : m_pBegin(nullptr), m_pCurrent(nullptr), m_pEnd(nullptr) {}

    CIterator(leaf *p) {
      m_pCurrent = p;
      m_pBegin = p;
      m_pEnd = p;

      if (m_pCurrent == nullptr)
        return;

      while (m_pBegin->pPrev != nullptr) {
        m_pBegin = m_pBegin->pPrev;
      }
      while (m_pEnd->pNext != nullptr) {
        m_pEnd = m_pEnd->pNext;
      }
    }

    CIterator(const CIterator &src)
        : m_pBegin(src.m_pBegin), m_pCurrent(src.m_pCurrent),
          m_pEnd(src.m_pEnd) {}

    ~CIterator() {}

    CIterator &operator=(const CIterator &src) {
      m_pBegin = src.m_pBegin;
      m_pCurrent = src.m_pCurrent;
      m_pEnd = src.m_pEnd;
      return *this;
    }

    bool operator!=(const CIterator &it) const {
      return m_pCurrent != it.m_pCurrent;
    }

    void operator++() {
      if (!(this->isValid())) {
        throw "Impossible to pop empty list";
      }

      if (m_pCurrent + 1 == m_pBegin) {
        m_pCurrent = m_pBegin;
      } else {
        m_pCurrent = m_pCurrent->pNext;
      }
    }

    void operator--() {
      if (!(this->isValid())) {
        throw "Impossible to pop empty list";
      }
      if (m_pCurrent - 1 == m_pEnd) {
        m_pCurrent = m_pEnd;
      } else {
        m_pCurrent = m_pCurrent->pPrev;
      }
    }

    T &getData() { return m_pCurrent->data; }

    T &operator*() { return m_pCurrent->data; }

    leaf *getLeaf() { return m_pCurrent; }

    // применяется в erase и eraseAndNext
    void setLeaf(leaf *p) { m_pCurrent = p; }

    // применяется в erase и eraseAndNext
    void setLeafPreBegin(leaf *p) {
      m_pBegin = p;
      m_pCurrent = m_pBegin - 1;
    }

    // применяется в erase и eraseAndNext
    void setLeafPostEnd(leaf *p) {
      m_pEnd = p;
      m_pCurrent = m_pEnd + 1;
    }

    bool isValid() { return (m_pCurrent != nullptr); }

  private:
    // храним голову списка, если мы находимся перед началом
    leaf *m_pBegin;
    // храним текущее положение
    leaf *m_pCurrent;
    // храним конец списка, если мы находимся после конца
    leaf *m_pEnd;
  };

public:
  CDualLinkedList() : m_pBegin(nullptr), m_pEnd(nullptr){};

  virtual ~CDualLinkedList() { clear(); };

  void pushBack(T &data) {
    leaf *p = new leaf(data, m_pEnd, nullptr);

    if (m_pEnd == nullptr) {
      m_pBegin = m_pEnd = p;
    } else {
      m_pEnd->pNext = p;
      m_pEnd = p;
    }
  }

  T popBack() {
    if (m_pBegin == nullptr) {
      throw "Impossible to pop empty list";
    }

    auto p = m_pEnd;
    m_pEnd = p->pPrev;

    if (m_pEnd == nullptr) {
      m_pBegin = nullptr;
    } else {
      m_pEnd->pNext = nullptr;
    }

    auto data = p->data;
    delete p;

    return data;
  }

  void pushFront(T &data) {
    leaf *p = new leaf(data, nullptr, m_pBegin);

    if (m_pBegin == nullptr) {
      m_pBegin = m_pEnd = p;
    } else {
      m_pBegin->pPrev = p;
      m_pBegin = p;
    }
  }

  T popFront() {
    if (m_pBegin == nullptr) {
      throw "Impossible to pop empty list";
    }

    auto p = m_pBegin;
    m_pBegin = p->pNext;

    if (m_pBegin == nullptr) {
      m_pEnd = nullptr;
    } else {
      m_pBegin->pPrev = nullptr;
    }

    auto data = p->data;
    delete p;

    return data;
  }

  // изменяет состояние итератора. выставляет предыдущую позицию.
  void erase(CIterator &it) {
    if (!it.isValid()) {
      return;
    }

    if (m_pBegin == nullptr || m_pEnd == nullptr) {
      throw "Impossible to erase empty list";
    }

    if (it.getLeaf() == m_pBegin) {
      popFront();
      it.setLeafPreBegin(m_pBegin);
      return;
    }
    if (it.getLeaf() == m_pEnd) {
      popBack();
      it.setLeaf(m_pEnd);
      return;
    }

    it.getLeaf()->pNext->pPrev = it.getLeaf()->pPrev;
    it.getLeaf()->pPrev->pNext = it.getLeaf()->pNext;

    auto to_delete = it.getLeaf();
    --it;
    delete to_delete;
  }

  // изменяет состояние итератора. выставляет следующую позицию.
  void eraseAndNext(CIterator &it) {
    if (!it.isValid()) {
      return;
    }

    if (m_pBegin == nullptr || m_pEnd == nullptr) {
      throw "Impossible to erase empty list";
    }

    if (it.getLeaf() == m_pBegin) {
      popFront();
      it.setLeaf(m_pBegin);
      return;
    }
    if (it.getLeaf() == m_pEnd) {
      popBack();
      it.setLeafPostEnd(m_pEnd);
      return;
    }

    it.getLeaf()->pNext->pPrev = it.getLeaf()->pPrev;
    it.getLeaf()->pPrev->pNext = it.getLeaf()->pNext;

    auto to_delete = it.getLeaf();
    ++it;
    delete to_delete;
  }

  int getSize() {
    if ((m_pBegin == nullptr) && (m_pEnd == nullptr)) {
      return 0;
    }
    if ((m_pBegin == nullptr) || (m_pEnd == nullptr)) {
      throw "Something goes wrong";
    }
    int size = 1;
    leaf *p = m_pBegin;
    while (p != m_pEnd) {
      p = p->pNext;
      size++;
    }
    return size;
  }

  void clear() {
    while (m_pBegin != m_pEnd) {
      auto p = m_pBegin;
      m_pBegin = p->pNext;
      delete p;
    }
    delete m_pBegin;
    m_pBegin = m_pEnd = nullptr;
  }

  CIterator begin() { return CIterator(m_pBegin); }

  CIterator end() { return CIterator(m_pEnd); }

private:
  // храним голову и хвост списка
  leaf *m_pBegin, *m_pEnd;
};
};     // namespace lab618
#endif // #ifndef TEMPLATES_LIST_2022_02_03
