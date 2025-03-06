#include <algorithm>
#include <cstddef>
#include <cstring>
#include <forward_list>
#include <iostream>
#include <optional>
#include <set>
#include <stack>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace astl {
template <typename T> struct Vertex {
private:
  template <typename Tp> friend class Graph;

  T data;
  std::forward_list<size_t> adjacency_list;

  Vertex(const T& val) : data(val) {}
};

template <typename T> class Graph {
public:
  using EdgeIterator = typename std::forward_list<size_t>::const_iterator;
  using VertexIndex = size_t;

  std::optional<VertexIndex> find_vertex(const T& val) const {
    for (VertexIndex i = 0; i < num_vertices(); ++i) {
      if (vertex_data(i) == val) {
        return i;
      }
    }

    return {};
  }

  VertexIndex add_vertex(const T& val) {
    vertices.push_back(val);
    return vertices.size() - 1;
  }

  T remove_vertex(const VertexIndex index) {
    const T return_value = vertices[index].data;
    const VertexIndex replacement_index = vertices.size() - 1;

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

  size_t num_vertices() const { return vertices.size(); }

  T& vertex_data(const VertexIndex index) const {
    return vertices[index].data;
  }

  std::pair<VertexIndex, VertexIndex> vertex_range() const {
    return std::make_pair(0, vertices.size());
  }

  void add_edge(const VertexIndex from, const VertexIndex to) {
    vertices[from].adjacency_list.push_front(to);
    vertices[to].adjacency_list.push_front(from);
  }

  void remove_edge(const VertexIndex from, const VertexIndex to) {
    vertices[from].adjacency_list.remove(to);
    vertices[to].adjacency_list.remove(from);
  }

  bool has_edge(const VertexIndex from, const VertexIndex to) const {
    return std::find_if(vertices[from].adjacency_list.cbegin(), vertices[from].adjacency_list.cend(), to) != vertices[from].adjacency_list.cend();
  }

  std::pair<EdgeIterator, EdgeIterator> out_edges(const VertexIndex vertex_index) const {
    return std::make_pair(vertices[vertex_index].adjacency_list.cbegin(), vertices[vertex_index].adjacency_list.cend());
  }

private:
  std::vector<Vertex<T>> vertices;
};

template <typename T> std::vector<bool> dfs(const Graph<T> &graph, const typename Graph<T>::VertexIndex start) {
  using VertexIndex = typename Graph<T>::VertexIndex;

  std::vector<bool> visited(graph.num_vertices());
  std::stack<VertexIndex> stack({start});

  while (!stack.empty()) {
    VertexIndex index = stack.top();
    stack.pop();

    if (!visited[index]) {
      visited[index] = true;

      auto out_edges = graph.out_edges(index);
      for (auto it = out_edges.first; it != out_edges.second; ++it) {
        stack.push(*it);
      }
    }
  }

  return visited;
}

std::vector<std::vector<bool>> components(const Graph<int> &graph) {
  std::vector<std::vector<bool>> components;

  auto vertex_range = graph.vertex_range();
  for (auto i = vertex_range.first; i < vertex_range.second; ++i) {
    if (std::find_if(components.cbegin(), components.cend(), [i](std::vector<bool> component) {
      return component[i];
    }) == components.cend()) {
      components.push_back(dfs(graph, i));
    }
  }

  return components;
}
}; // namespace astl


int main() {
  astl::Graph<int> graph;

  int num_vertices = 0;
  std::cin >> num_vertices;
  
  for (int i = 0; i < num_vertices; ++i) {
    graph.add_vertex(i);
  }

  int 

  if (argc < 2 || argc % 2 == 0) {
    return 1;
  }

  int num_vertices = std::stoi(argv[1]);
  std::vector<Graph<int>::vertex_index> vertex_indices;
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
