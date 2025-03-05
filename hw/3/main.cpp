#include <algorithm>
#include <cstddef>
#include <list>
#include <iostream>
#include <memory>
#include <optional>
#include <ostream>
#include <sstream>
#include <vector>

template <typename W> struct Edge;

template <typename T, typename W> struct Vertex;

template <typename T, typename W> class Graph;

template <typename W> struct Edge {
  W weight;
  size_t from;
  size_t to;
};

template <typename W> struct EdgeIndex {
private:
  template <typename T, typename Wp> friend class Graph;
  typedef typename std::list<Edge<W>>::iterator iterator;
  iterator edge_iterator;

  EdgeIndex(iterator it) : edge_iterator(it) {}
};

template <typename W> class Edges {
  using difference_type = ptrdiff_t;
  using value_type = EdgeIndex<W>;

  EdgeIndex<W> operator*() const = 0;
  Edges& operator++() = 0;
  Edges operator++()
};

template <typename W> class OutEdges {
private:
  std::list<Edge<W>> &adjacency_list;

  OutEdges(std::list<Edge<W>> &list) : adjacency_list(list) {}
};

template <typename T, typename W> struct Vertex {
  Vertex(T value) : data(value) {}

  T data;
  std::list<Edge<W>> adjacency_list;

  bool operator==(const T& rhs) {
    return data == rhs;
  }
};

struct VertexIndex {
private:
  template <typename Tp, typename Wp> friend class Graph;
  size_t index;

  VertexIndex(size_t idx) : index(idx) {}
};

template <typename T, typename W> class Graph {
private:
  std::vector<Vertex<T, W>> vertices;

public:
  typedef EdgeIndex<W> edge_index;
  typedef VertexIndex vertex_index;

  std::optional<vertex_index> find_vertex(const T& value) {
    for (size_t i = 0; i < vertices.size(); ++i) {
      if (vertices[i] == value) {
        return VertexIndex(i);
      }
    }

    return {};
  }

  vertex_index add_vertex(T value) {
    vertices.push_back(value);
    return vertices.size() - 1;
  }

  T& vertex_data(vertex_index vertex_index) {
    return vertices[vertex_index.index].data;
  }

  T remove_vertex(vertex_index edge_index) {
    size_t removal_index = edge_index.index;
    size_t replacement_index = vertices.size() - 1;

    for (auto &vertex : vertices) {
      if (std::addressof(vertex) != vertices.data() + removal_index) {
        auto iterator = vertex.adjacency_list.begin();
        while (iterator != vertex.adjacency_list.end()) {
          if (iterator->to == removal_index) {
            vertex.adjacency_list.erase(iterator++);
          } else if (iterator->to == replacement_index) {
            iterator->to = removal_index;
            ++iterator;
          } else {
            ++iterator;
          }
        }
      }
    }

    T return_value = vertices[removal_index].data;
    if (removal_index != replacement_index) {
      std::swap(vertices[removal_index], vertices[replacement_index]);
    }
    vertices.pop_back();
    return return_value;
  }

  std::optional<edge_index> find_edge(const vertex_index from, const vertex_index to) {
    auto edge_iterator = std::find_if(vertices[from.index].adjacency_list.begin(), vertices[from.index].adjacency_list.end(), [to](auto edge) {
      return edge.to == to;
    });

    if (edge_iterator == vertices[from.index].end()) {
      return {};
    } else {
      return edge_index(edge_iterator);
    }
  }

  edge_index add_edge(const vertex_index from, const vertex_index to, W weight) {
    vertices[from.index].adjacency_list.push_front(Edge<W>{weight, from.index, to.index});
    return edge_index(vertices[from.index].adjacency_list.begin());
  }

  W& edge_weight(edge_index edge_index) {
    return edge_index.edge_iterator->weight;
  }

  vertex_index edge_source(edge_index edge_index) {
    return edge_index.edge_iterator->from;
  }

  vertex_index edge_target(edge_index edge_index) {
    return edge_index.edge_iterator->to;
  }

  W remove_edge(edge_index edge_index) {
    W return_value = edge_index.edge_iterator->weight;
    vertices[edge_index.edge_iterator->from].adjacency_list.erase(edge_index.edge_iterator);
    return return_value;
  }

  std::string to_string() {
    std::ostringstream out;

    for (auto &vertex : vertices) {
      out << vertex.data << ": ";
      for (auto &edge : vertex.adjacency_list) {
        out << vertices[edge.to].data << '(' << edge.weight << "), ";
      }
      out << '\n';
    }

    return out.str();
  }
};

int main(int argc, char **argv) {
  Graph<int, int> g;
  std::vector<Graph<int, int>::vertex_index> indices;
  for (size_t i = 0; i < 5; ++i) {
    indices.push_back(g.add_vertex(i));
  }

  auto to_remove = g.add_edge(indices[1], indices[2], 1);
  std::cout << g.to_string() << '\n';

  g.remove_edge(to_remove);
  std::cout << g.to_string() << '\n';

  g.add_vertex(5);
  std::cout << g.to_string() << '\n';

  g.add_edge(indices[0], indices[3], 2);
  std::cout << g.to_string() << '\n';

  g.add_edge(indices[2], indices[3], 2);
  std::cout << g.to_string() << '\n';

  g.add_edge(indices[2], indices[4], 2);
  std::cout << g.to_string() << '\n';

  g.remove_vertex(indices[3]);
  std::cout << g.to_string() << '\n';
}
