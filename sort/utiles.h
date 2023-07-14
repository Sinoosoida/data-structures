#include <cstdlib>
#include <type_traits>

template <class T> class self_destroyed_array {
private:
  T **ptr = nullptr;
  int64_t ln;

public:
  self_destroyed_array(int64_t len_in, int random_seed = 42) : ln(len_in) {
    srand(10);
    if (not std::is_constructible_v<T, int>) {
      return;
    }

    ptr = new T *[ln];
    for (int i = 0; i < ln; i++) {
      ptr[i] = new T(rand());
    }
  }
  self_destroyed_array(int64_t len_in, int64_t divider, int random_seed = 42)
      : ln(len_in) {
    srand(10);
    if (not std::is_constructible_v<T, int>) {
      return;
    }

    ptr = new T *[ln];
    for (int i = 0; i < ln; i++) {
      ptr[i] = new T(rand() % divider);
    }
  }
  self_destroyed_array(const self_destroyed_array &p) {
    ln = p.ln;
    ptr = new T *[ln];
    for (int i = 0; i < ln; i++) {
      ptr[i] = new T(*p.ptr[i]);
    }
  }

  ~self_destroyed_array() {
    for (int i = 0; i < ln; i++) {
      delete ptr[i];
    }
    delete ptr;
  }
  T **get_ptr() { return ptr; }
};

template <class T>
std::ostream &operator<<(std::ostream &os, self_destroyed_array<T> const &p) {
  for (int i = 0; i < p.ln; i++)
    os << *p.ptr[i] << ' ';
  os << std::endl;
}