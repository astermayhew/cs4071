#include <algorithm>
#include <forward_list>
#include <initializer_list>
#include <iostream>
#include <ostream>
#include <sstream>
#include <vector>

template <typename T, typename W> struct Edge;

template <typename T, typename W> struct Vertex;

template <typename T, typename W> class Graph;

template <typename T, typename W> struct Edge {
  W weight;
  Vertex<T, W> &from;
  Vertex<T, W> &to;
};

template <typename T, typename W> struct Vertex {
  Vertex(T value) : data(value) {}

  T data;
  std::forward_list<Edge<T, W>> adjacency_list;

  bool operator==(const T& rhs) {
    return data == rhs;
  }
};

template <typename T, typename F> bool remove_one_if(std::forward_list<T> &list, F predicate) {
  auto leader = list.begin();
  if (leader == list.end()) {
    return false;
  }

  if (predicate(*leader)) {
    list.pop_front();
    return true;
  }

  auto follower = leader++;

  while (leader != list.end()) {
    if (predicate(*leader)) {
      list.erase_after(follower);
      return true;
    }
    ++leader;
  }

  return false;
}

template <typename T, typename W> class Graph {
private:
  std::vector<Vertex<T, W>> vertices;

public:
  Graph(std::initializer_list<T> l) {
    for (auto v : l) {
      vertices.push_back(v);
    }
  }

  bool add_vertex(T value) {
    if (std::find(vertices.begin(), vertices.end(), value) != vertices.end()) {
      return false;
    } else {
      vertices.push_back(value);
      return true;
    }
  }

  void delete_vertex(const T& value) {
    auto old = vertices.end();
    for (auto it = vertices.begin(); it != vertices.end(); ++it) {
      if (*it == value) {
        old = it;
      } else {
        remove_one_if(it->adjacency_list, [value](auto edge) {
          return edge.to.data == value;
        });
      }
    }

    vertices.erase(old);
  }

  bool add_edge(const T &from, const T &to, W weight) {
    auto from_vertex = std::find(vertices.begin(), vertices.end(), from);
    if (from_vertex == vertices.end()) {
      return false;
    }

    auto to_vertex = std::find(vertices.begin(), vertices.end(), to);
    if (to_vertex == vertices.end()) {
      return false;
    }

    if (std::find_if(from_vertex->adjacency_list.begin(),
                     from_vertex->adjacency_list.end(), [to](auto edge) {
                       return edge.to == to;
                     }) != from_vertex->adjacency_list.end()) {
      return false;
    }

    from_vertex->adjacency_list.push_front(Edge<T, W>{weight, *from_vertex, *to_vertex});
    return true;
  }

  bool delete_edge(const T &from, const T &to) {
    auto from_vertex = std::find(vertices.begin(), vertices.end(), from);
    if (from_vertex == vertices.end()) {
      return false;
    }

    return remove_one_if(from_vertex->adjacency_list, [to](auto edge) {
      return edge.to == to;
    });
  }

  std::string to_string() {
    std::ostringstream out;

    for (auto vertex : vertices) {
      out << vertex.data << ": ";
      for (auto edge : vertex.adjacency_list) {
        out << edge.to.data << '(' << edge.weight << "), ";
      }
      out << '\n';
    }

    return out.str();
  }
};

int main(int argc, char **argv) {
  Graph<int, int> g({0, 1, 2, 3, 4});
  g.add_edge(1, 2, 1);
  std::cout << g.to_string() << '\n';

  g.delete_edge(1, 2);
  std::cout << g.to_string() << '\n';

  g.add_vertex(5);
  std::cout << g.to_string() << '\n';

  g.add_edge(0, 3, 2);
  std::cout << g.to_string() << '\n';

  g.add_edge(2, 3, 2);
  std::cout << g.to_string() << '\n';

  g.add_edge(2, 5, 2);
  std::cout << g.to_string() << '\n';

  g.delete_vertex(3);
  std::cout << g.to_string() << '\n';
}
