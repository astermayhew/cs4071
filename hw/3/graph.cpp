#include <cstddef>
#include <forward_list>
#include <vector>

template <typename T> struct Vertex {
  T data;
  std::forward_list<typename Tp><size_t> adjacency_list;

  Vertex(const T& val) : data(val) {}
};

template <typename T> class Graph {
public:
  std::vector<Vertex<T>> vertices;

  size_t add_vertex(const T& val) {
    vertices.push_back(val);
    return vertices.size() - 1;
  }

  T remove_vertex(size_t index) {
    T return_value = vertices[index].data;
    //std::swap(vertices[])
  }



  void add_edge(size_t from, size_t to) {
    vertices[from].adjacency_list.push_front(to);
    vertices[to].adjacency_list.push_front(from);
  }

  void remove_edge(size_t from, size_t to) {
    vertices[from].adjacency_list.remove(to);
    vertices[to].adjacency_list.remove(from);
  }
};
