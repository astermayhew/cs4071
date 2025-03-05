#include <cstddef>
#include <forward_list>
#include <vector>

template <typename T> struct Vertex {
  T data;
  std::forward_list<size_t> adjacency_list;

  Vertex(const T& val) : data(val) {}
};

template <typename T> class Graph {
public:
  using edge_iterator = typename std::forward_list<size_t>::const_iterator;
  using vertex_index = size_t;

  vertex_index add_vertex(const T& val) {
    vertices.push_back(val);
    return vertices.size() - 1;
  }

  T remove_vertex(vertex_index index) {
    T return_value = vertices[index].data;
    vertex_index replacement_index = vertices.size() - 1;

    for (Vertex<T> &vertex : vertices) {
      auto leader = vertex.adjacency_list.before_begin();
      auto follower = leader++;

      while (leader != vertex.adjacency_list.end()) {
        if (*leader == index) {
          ++leader;
          vertex.adjacency_list.erase_after(follower);
        } else {
          if (*leader == replacement_index) {
            *leader = index;
          }

          follower = leader++;
        }
      }
    }

    std::swap(vertices[index], vertices[replacement_index]);
    vertices.pop_back();
    return return_value;
  }

  void add_edge(vertex_index from, vertex_index to) {
    vertices[from].adjacency_list.push_front(to);
    vertices[to].adjacency_list.push_front(from);
  }

  void remove_edge(vertex_index from, vertex_index to) {
    vertices[from].adjacency_list.remove(to);
    vertices[to].adjacency_list.remove(from);
  }

  std::pair<edge_iterator, edge_iterator> out_edges(vertex_index vertex) {

  }

private:
  std::vector<Vertex<T>> vertices;
};
