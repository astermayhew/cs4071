#include <algorithm>
#include <cstddef>
#include <cstring>
#include <forward_list>
#include <iomanip>
#include <iostream>
#include <optional>
#include <ostream>
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

  // returns the index of the first vertex with data matching `val` if it exists
  std::optional<size_t> find_vertex(const T& val) const {
    for (size_t i = 0; i < num_vertices(); ++i) {
      if (vertex_data(i) == val) {
        return i;
      }
    }

    return {};
  }

  // adds vertex with data `val` to the graph
  //
  // returns the index at which the vertex was inserted
  size_t add_vertex(const T& val) {
    vertices.push_back(val);
    // inserted vertex will always be the last element of the vector
    return vertices.size() - 1;
  }

  // removes the vertex at index `index`
  //
  // returns the data associated with the removed vertex
  //
  // NOTE: indices pointing to the removed vertex and the last vertex are
  // invalidated
  T remove_vertex(const size_t index) {
    // to avoid shifting all vertices over, we will replace the removed vertex
    // with the last vertex in our vector
    const T return_value = vertices[index].data;
    const size_t replacement_index = vertices.size() - 1;

    for (Vertex<T>& vertex : vertices) {
      auto leader = vertex.adjacency_list.before_begin();
      // we need to keep track of the node before the one we erase for a
      // singly-linked list
      auto follower = leader++;

      // we need to erase all edges pointing to the removed vertex and adjust
      // all edges pointing to the replacement vertex
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

    // swap old vertex with replacement vertex
    std::swap(vertices[index], vertices[replacement_index]);
    // remove old vertex
    vertices.pop_back();
    return return_value;
  }

  // returns the number of vertices in the graph
  size_t num_vertices() const { return vertices.size(); }

  // returns a const reference to the data associated with the vertex at `index`
  const T& vertex_data(const size_t index) const {
    return vertices[index].data;
  }

  // returns a reference to the data associated with the vertex at `index`
  T& vertex_data(const size_t index) { return vertices[index].data; }

  // returns a pair containing the begin and (past-the-) end indices of the
  // vertices in the graph
  std::pair<size_t, size_t> vertex_range() const {
    return std::make_pair(0, num_vertices());
  }

  // adds an edge between the vertices at `from_index` and `to_index`
  void add_edge(const size_t from_index, const size_t to_index) {
    vertices[from_index].adjacency_list.push_front(to_index);
    if (from_index != to_index) {
      vertices[to_index].adjacency_list.push_front(from_index);
    }
  }

  // removes all edges between the vertices at `from_index` and `to_index`
  void remove_edge(const size_t from_index, const size_t to_index) {
    vertices[from_index].adjacency_list.remove(to_index);
    vertices[to_index].adjacency_list.remove(from_index);
  }

  // returns a boolean indicating whether an edge between the vertices at
  // `from_index` and `to_index` exists
  bool has_edge(const size_t from_index, const size_t to_index) const {
    return std::find(vertices[from_index].adjacency_list.cbegin(),
                     vertices[from_index].adjacency_list.cend(),
                     to_index) != vertices[from_index].adjacency_list.cend();
  }

  // returns a pair containing begin and (past-the-) end forward iterators
  // yielding the indices of the out edges of the vertex at `vertex_index`
  std::pair<EdgeIterator, EdgeIterator> out_edges(
      const size_t vertex_index) const {
    return std::make_pair(vertices[vertex_index].adjacency_list.cbegin(),
                          vertices[vertex_index].adjacency_list.cend());
  }

 private:
  std::vector<Vertex<T>> vertices;
};

// performs a depth-first search of `graph` starting at the vertex at `start`
//
// returns a pair containing a vector where for each index visited, that index
// of the vector is true, and a vector containing the visited indices in the
// order they were searched
template <typename T>
std::pair<std::vector<bool>, std::vector<size_t>> dfs(const Graph<T>& graph,
                                                      const size_t start) {
  std::vector<bool> visited(graph.num_vertices());
  std::vector<size_t> order;
  std::stack<size_t> stack({start});

  // while there are vertices left to search,
  while (!stack.empty()) {
    size_t index = stack.top();
    stack.pop();

    // if we haven't visited this vertex,
    if (!visited[index]) {
      // visit it,
      visited[index] = true;
      order.push_back(index);

      // and add its out edges to the stack
      auto out_edges = graph.out_edges(index);
      for (auto it = out_edges.first; it != out_edges.second; ++it) {
        stack.push(*it);
      }
    }
  }

  return std::make_pair(visited, order);
}

// computes the connected components of `graph`
//
// returns a vector one vector for each connected component, where for each
// vertex in that component, that vertex's index of that vector is true
template <typename T>
std::vector<std::vector<bool>> components(const Graph<T>& graph) {
  std::vector<std::vector<bool>> components;

  auto [start_index, end_index] = graph.vertex_range();
  // for each vertex,
  for (size_t i = start_index; i < end_index; ++i) {
    // if this vertex isn't contained in any components we've seen so far,
    if (std::find_if(components.cbegin(), components.cend(),
                     [i](std::vector<bool> component) {
                       return component[i];
                     }) == components.cend()) {
      // collect the items in this component with dfs
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
  std::cout << '\n';

  // print adjacency list
  std::cout << "adjacency list:\n";
  for (const auto i : vertex_indices) {
    std::cout << graph.vertex_data(i) << ": ";

    auto edge_range = graph.out_edges(i);
    for (auto it = edge_range.first; it != edge_range.second; ++it) {
      std::cout << graph.vertex_data(*it) << ", ";
    }
    std::cout << '\n';
  }
  std::cout << '\n';

  // print adjacency matrix
  std::cout << "adjacency matrix:\n";
  std::cout << std::setw(4) << ' ';
  for (const auto i : vertex_indices) {
    std::cout << std::setw(4) << graph.vertex_data(i);
  }
  std::cout << '\n';
  for (const auto i : vertex_indices) {
    std::cout << std::setw(4) << graph.vertex_data(i);
    for (const auto j : vertex_indices) {
      std::cout << std::setw(4) << (graph.has_edge(i, j) ? 'X' : '-');
    }
    std::cout << '\n';
  }
  std::cout << '\n';

  // print connected components
  std::cout << "connected components:\n";
  for (const auto& component : astl::components(graph)) {
    std::cout << '{';
    for (size_t i = 0; i < component.size(); ++i) {
      if (component[i]) {
        std::cout << graph.vertex_data(i) << ',';
      }
    }
    std::cout << "}\n";
  }
}
