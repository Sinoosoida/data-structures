#include "vector"

namespace lab618 {
template<class T>
class CMemoryManager {

public:

  explicit CMemoryManager(int _default_block_size, bool isDeleteElementsOnDestruct = false){}

  virtual ~CMemoryManager() {
    clear();
  }

  T *newObject() {
    T* out = new T;
    data.push_back(out);
    return out;
  }

  bool deleteObject(T *p) {
    for (auto i = data.begin() ; i<data.end(); i++){
      if (i == p) {
        data.erase(i);
      }
    }
  }
  void clear() {
    for (auto i:data){
      delete i;
    }
    data.clear();
  }

private:
  std::vector<T*> data;
};
}; // namespace lab618