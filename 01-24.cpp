#include <cstddef>
#include <cstdint>
#include <optional>
#include <vector>
#include <iostream>

template <typename T>
std::optional<size_t> binary_search(T&, T*, size_t, size_t begin = 0);

template <typename T>
std::optional<size_t> binary_search2(T&, T*, size_t, size_t begin = 0);

int main() {
  std::vector v{1, 3, 4, 6, 7, 9, 10, 11, 18};

  for (auto i : {0, 1, 5, 9, 18, 19}) {
    std::cout << i << " at position " << binary_search(i, v.data(), v.size()).value_or(SIZE_MAX) << "\n";
  }
}

template <typename T>
std::optional<size_t> binary_search(T& key, T* arr, size_t size, size_t begin) {
  if (size <= begin) {
    return {};
  }

  size_t mid = (begin + size) / 2;

  if (key == arr[mid]) {
    return mid;
  } else if (key < arr[mid]) {
    return binary_search(key, arr, mid);
  } else {
    return binary_search(key, arr, size, mid + 1);
  }
}

template <typename T>
std::optional<size_t> binary_search2(T& key, T* arr, size_t size, size_t begin) {
  if (size <= begin) {
    return {};
  }

  size_t mid = (begin + size) / 2;

  if (key < arr[mid]) {
    return binary_search2(key, arr, mid);
  } else if (key > arr[mid]) {
    return binary_search2(key, arr, size, mid + 1);
  } else {
    return mid;
  }
}
