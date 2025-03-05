#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <list>
#include <memory>
#include <optional>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
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
public:
  bool operator==(const EdgeIndex &rhs) const {
    edge_pointer == rhs.edge_pointer;
  }

private:
  template <typename T, typename Wp> friend class Graph;
  template <typename Wp> friend class OutEdgeIterator;

  typedef typename std::list<Edge<W>>::iterator pointer;
  pointer edge_pointer;

  EdgeIndex(pointer p) : edge_pointer(p) {}
};

template <typename W> class OutEdgeIterator {
public:
  using difference_type = std::ptrdiff_t;
  using value_type = EdgeIndex<W>;

  EdgeIndex<W> operator*() const { return EdgeIndex<W>(edge_pointer); }

  OutEdgeIterator &operator++() {
    ++edge_pointer;
    return *this;
  }

  OutEdgeIterator operator++(int) {
    pointer old = edge_pointer++;
    return OutEdgeIterator(old);
  }

  OutEdgeIterator &operator--() {
    --edge_pointer;
    return *this;
  }

  OutEdgeIterator operator--(int) {
    pointer old = edge_pointer--;
    return OutEdgeIterator(old);
  }

  bool operator==(OutEdgeIterator &rhs) {
    return edge_pointer == rhs.edge_pointer;
  }

  bool operator!=(OutEdgeIterator &rhs) { return !(*this == rhs); }

private:
  template <typename T, typename Wp> friend class Graph;

  using pointer = typename std::list<Edge<W>>::iterator;
  pointer edge_pointer;

  OutEdgeIterator(pointer p) : edge_pointer(p) {}
};

template <typename T, typename W> struct Vertex {
  Vertex(T value) : data(value) {}

  T data;
  std::list<Edge<W>> adjacency_list;

  bool operator==(const T &rhs) { return data == rhs; }
};

struct VertexIndex {
private:
  template <typename T, typename W> friend class Graph;
  friend class VertexIterator;

  size_t index;
  VertexIndex(size_t idx) : index(idx) {}
};

class VertexIterator {
public:
  using difference_type = std::ptrdiff_t;
  using value_type = VertexIndex;

  value_type operator*() const { return value_type(index); }

  VertexIterator &operator++() {
    ++index;
    return *this;
  }

  VertexIterator operator++(int) {
    size_t old = index++;
    return VertexIterator(old);
  }

  VertexIterator &operator--() {
    --index;
    return *this;
  }

  VertexIterator operator--(int) {
    size_t old = index--;
    return VertexIterator(old);
  }

  bool operator==(const VertexIterator &rhs) const {
    return index == rhs.index;
  }

  bool operator!=(const VertexIterator &rhs) const { return !(*this == rhs); }

private:
  template <typename T, typename Wp> friend class Graph;

  size_t index;
  VertexIterator(size_t i) : index(i) {}
};

template <typename T, typename W> class Graph {
private:
  std::vector<Vertex<T, W>> m_vertices;

public:
  typedef EdgeIndex<W> edge_index;
  typedef VertexIndex vertex_index;
  using out_edge_iterator = OutEdgeIterator<W>;
  using vertex_iterator = VertexIterator;

  std::optional<vertex_index> find_vertex(const T &value) {
    for (size_t i = 0; i < m_vertices.size(); ++i) {
      if (m_vertices[i] == value) {
        return VertexIndex(i);
      }
    }

    return {};
  }

  vertex_index add_vertex(T value) {
    m_vertices.push_back(value);
    return m_vertices.size() - 1;
  }

  T &vertex_data(vertex_index vertex_index) {
    return m_vertices[vertex_index.index].data;
  }

  T remove_vertex(vertex_index edge_index) {
    size_t removal_index = edge_index.index;
    size_t replacement_index = m_vertices.size() - 1;

    for (auto &vertex : m_vertices) {
      if (std::addressof(vertex) != m_vertices.data() + removal_index) {
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

    T return_value = m_vertices[removal_index].data;
    if (removal_index != replacement_index) {
      std::swap(m_vertices[removal_index], m_vertices[replacement_index]);
    }
    m_vertices.pop_back();
    return return_value;
  }

  std::pair<vertex_iterator, vertex_iterator> vertices() {
    return std::make_pair(vertex_iterator(0),
                          vertex_iterator(m_vertices.size()));
  }

  std::optional<edge_index> find_edge(const vertex_index from,
                                      const vertex_index to) {
    auto edge_iterator =
        std::find_if(m_vertices[from.index].adjacency_list.begin(),
                     m_vertices[from.index].adjacency_list.end(),
                     [to](auto edge) { return edge.to == to; });

    if (edge_iterator == m_vertices[from.index].end()) {
      return {};
    } else {
      return edge_index(edge_iterator);
    }
  }

  edge_index add_edge(const vertex_index from, const vertex_index to,
                      W weight) {
    m_vertices[from.index].adjacency_list.push_front(
        Edge<W>{weight, from.index, to.index});
    m_vertices[to.index].adjacency_list.push_front(
        Edge<W>{weight, to.index, from.index});
    return edge_index(m_vertices[from.index].adjacency_list.begin());
  }

  W &edge_weight(edge_index edge_index) {
    return edge_index.edge_pointer->weight;
  }

  vertex_index edge_source(edge_index edge_index) {
    return edge_index.edge_pointer->from;
  }

  vertex_index edge_target(edge_index edge_index) {
    return edge_index.edge_pointer->to;
  }

  W remove_edge(edge_index edge_index) {
    W return_value = edge_index.edge_pointer->weight;
    m_vertices[edge_index.edge_pointer->from].adjacency_list.erase(
        edge_index.edge_pointer);
    return return_value;
  }

  std::pair<out_edge_iterator, out_edge_iterator>
  out_edges(vertex_index vertex_index) {
    return std::make_pair(
        out_edge_iterator(
            m_vertices[vertex_index.index].adjacency_list.begin()),
        out_edge_iterator(m_vertices[vertex_index.index].adjacency_list.end()));
  }

  std::string to_string() {
    std::ostringstream out;

    for (auto &vertex : m_vertices) {
      out << vertex.data << ": ";
      for (auto &edge : vertex.adjacency_list) {
        out << m_vertices[edge.to].data << '(' << edge.weight << "), ";
      }
      out << '\n';
    }

    return out.str();
  }
};

std::set<int> dfs(Graph<int, int> &graph, Graph<int, int>::vertex_index start) {
  using vertex_index = Graph<int, int>::vertex_index;

  std::set<int> vertex_set({graph.vertex_data(start)});
  std::vector<bool> visited(graph.vertex_data(start) + 1);
  visited[graph.vertex_data(start)] = true;
  std::vector<vertex_index> to_visit;

  auto first_edges = graph.out_edges(start);
  for (auto it = first_edges.first; it != first_edges.second; ++it) {
    to_visit.push_back(graph.edge_target(*it));
  }

  while (!to_visit.empty()) {
    vertex_index current_vertex = to_visit.back();
    int current_data = graph.vertex_data(current_vertex);

    to_visit.pop_back();

    if (current_data >= visited.size()) {
      visited.resize(current_data + 1);
    }

    if (!visited[current_data]) {
      visited[current_data] = true;
      vertex_set.insert(current_data);

      auto out_edges = graph.out_edges(current_vertex);
      for (auto it = out_edges.first; it != out_edges.second; ++it) {
        to_visit.push_back(graph.edge_target(*it));
      }
    }
  }

  return vertex_set;
}

std::vector<std::set<int>> components(Graph<int, int> &graph) {
  std::vector<std::set<int>> components;

  auto vertices = graph.vertices();
  for (auto it = vertices.first; it != vertices.second; ++it) {
    auto vertex_data = graph.vertex_data(*it);
    if (std::find_if_not(components.begin(), components.end(),
                     [vertex_data](std::set<int> component) {
                       return component.count(vertex_data) == 0;
                     }) == components.end()) {
      components.push_back(dfs(graph, *it));
    }
  }

  return components;
}

int main(int argc, char **argv) {
  Graph<int, int> g;

  if (argc < 2 || argc % 2 == 0) {
    return 1;
  }

  int num_vertices = std::stoi(argv[1]);
  std::vector<Graph<int, int>::vertex_index> vertex_indices;
  for (int i = 0; i < num_vertices; ++i) {
    vertex_indices.push_back(g.add_vertex(i));
  }

  for (int i = 2; strcmp(argv[i], "-1"); i += 2) {
    g.add_edge(vertex_indices[std::stoi(argv[i])],
               vertex_indices[std::stoi(argv[i + 1])], 1);
  }

  std::cout << g.to_string();

  std::cout << '[';
  for (auto v : dfs(g, vertex_indices[1])) {
    std::cout << v << ',';
  }
  std::cout << "]\n";
}
