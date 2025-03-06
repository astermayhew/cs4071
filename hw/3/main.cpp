#include <algorithm>
#include <cstddef>
#include <cstring>
#include <forward_list>
#include <iostream>
#include <optional>
#include <stack>
#include <utility>
#include <vector>

namespace astl {
template <typename T>
struct Vertex {
 private:
  template <typename Tp>
  friend class Graph;

  T data;
  std::forward_list<size_t> adjacency_list;

  Vertex(const T& val) : data(val) {}
};

template <typename T>
class Graph {
 public:
  using EdgeIterator = typename std::forward_list<size_t>::const_iterator;

  std::optional<size_t> find_vertex(const T& val) const {
    for (size_t i = 0; i < num_vertices(); ++i) {
      if (vertex_data(i) == val) {
        return i;
      }
    }

    return {};
  }

  size_t add_vertex(const T& val) {
    vertices.push_back(val);
    return vertices.size() - 1;
  }

  T remove_vertex(const size_t index) {
    const T return_value = vertices[index].data;
    const size_t replacement_index = vertices.size() - 1;

    for (Vertex<T>& vertex : vertices) {
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

  size_t num_vertices() const { return vertices.size(); }

  const T& vertex_data(const size_t index) const {
    return vertices[index].data;
  }

  T& vertex_data(const size_t index) { return vertices[index].data; }

  std::pair<size_t, size_t> vertex_range() const {
    return std::make_pair(0, num_vertices());
  }

  void add_edge(const size_t from, const size_t to) {
    vertices[from].adjacency_list.push_front(to);
    vertices[to].adjacency_list.push_front(from);
  }

  void remove_edge(const size_t from, const size_t to) {
    vertices[from].adjacency_list.remove(to);
    vertices[to].adjacency_list.remove(from);
  }

  bool has_edge(const size_t from, const size_t to) const {
    return std::find_if(vertices[from].adjacency_list.cbegin(),
                        vertices[from].adjacency_list.cend(),
                        to) != vertices[from].adjacency_list.cend();
  }

  std::pair<EdgeIterator, EdgeIterator> out_edges(
      const size_t vertex_index) const {
    return std::make_pair(vertices[vertex_index].adjacency_list.cbegin(),
                          vertices[vertex_index].adjacency_list.cend());
  }

 private:
  std::vector<Vertex<T>> vertices;
};

template <typename T>
std::pair<std::vector<bool>, std::vector<size_t>> dfs(
    const Graph<T>& graph,
    const size_t start) {
  std::vector<bool> visited(graph.num_vertices());
  std::vector<size_t> order;
  std::stack<size_t> stack({start});

  while (!stack.empty()) {
    size_t index = stack.top();
    stack.pop();

    if (!visited[index]) {
      visited[index] = true;
      order.push_back(index);

      auto out_edges = graph.out_edges(index);
      for (auto it = out_edges.first; it != out_edges.second; ++it) {
        stack.push(*it);
      }
    }
  }

  return std::make_pair(visited, order);
}

std::vector<std::vector<bool>> components(const Graph<int>& graph) {
  std::vector<std::vector<bool>> components;

  auto vertex_range = graph.vertex_range();
  for (auto i = vertex_range.first; i < vertex_range.second; ++i) {
    if (std::find_if(components.cbegin(), components.cend(),
                     [i](std::vector<bool> component) {
                       return component[i];
                     }) == components.cend()) {
      components.push_back(dfs(graph, i).first);
    }
  }

  return components;
}
};  // namespace astl

int main() {
  astl::Graph<int> graph;

  int num_vertices = 0;
  std::cin >> num_vertices;

  std::vector<size_t> vertex_indices;
  vertex_indices.reserve(num_vertices);
  for (int i = 0; i < num_vertices; ++i) {
    vertex_indices.push_back(graph.add_vertex(i));
  }

  int vertex1 = -1;
  int vertex2 = 0;
  std::cin >> vertex1;
  while (vertex1 > -1) {
    std::cin >> vertex2;
    graph.add_edge(vertex_indices[vertex1], vertex_indices[vertex2]);
    std::cin >> vertex1;
  }

  auto vertex_range = graph.vertex_range();
  for (size_t i = vertex_range.first; i < vertex_range.second; ++i) {
    std::cout << graph.vertex_data(i) << ": ";

    auto edge_range = graph.out_edges(i);
    for (auto it = edge_range.first; it != edge_range.second; ++it) {
      std::cout << graph.vertex_data(*it) << ", ";
    }
    std::cout << '\n';
  }

  for (auto& component : astl::components(graph)) {
    std::cout << '{';
    for (size_t i = 0; i < component.size(); ++i) {
      if (component[i]) {
        std::cout << graph.vertex_data(i) << ',';
      }
    }
    std::cout << "}\n";
  }
}
