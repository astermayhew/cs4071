#include <iostream>

template <typename T>
void visit(T& item) {
  std::cout << item << '\n';
}

template <typename T>
struct Node {
  T item;
  Node<T>* left;
  Node<T>* right;
};

template <typename T>
void preorder(Node<T>& node) {
  visit(node.item);

  if (node.left) {
    preorder(*node.left);
  }

  if (node.right) {
    preorder(*node.right);
  }
}

template <typename T>
void inorder(Node<T>& node) {
  if (node.left) {
    inorder(*node.left);
  }

  visit(node.item);

  if (node.right) {
    inorder(*node.right);
  }
}

template <typename T>
void postorder(Node<T>& node) {
  if (node.left) {
    postorder(*node.left);
  }

  if (node.right) {
    postorder(*node.right);
  }

  visit(node.item);
}

int main() {
  Node<int> root = {10, nullptr, nullptr};
  root.left = new Node<int>{20, nullptr, nullptr};
  root.left->left = new Node<int>{88, nullptr, nullptr};
  root.left->left->left = new Node<int>{11, nullptr, nullptr};
  root.left->left->right = new Node<int>{-4, nullptr, nullptr};
  root.left->right = new Node<int>{1, nullptr, nullptr};
  root.left->right->left = new Node<int>{3, nullptr, nullptr};
  root.right = new Node<int>{60, nullptr, nullptr};
  root.right->left = new Node<int>{2, nullptr, nullptr};
  root.right->right = new Node<int>{7, nullptr, nullptr};

  preorder(root);
  std::cout << '\n';

  inorder(root);
  std::cout << '\n';

  postorder(root);
  std::cout << '\n';
}
