template <typename T>
struct Node {
  T key;
  Node<T>* left;
  Node<T>* right;
};

template <typename T>
Node<T>* search_bst(T& key, Node<T>* root) {
  if (!root || root->key == key) {
    return root;
  }

  if (key < root->key) {
    return search_bst(key, root->left);
  } else {
    return search_bst(key, root->right);
  }
}

template <typename T>
Node<T>* insert(T& data, Node<T>* root) {
  if (!root) {
    return new Node<T>(data, nullptr, nullptr);
  }

  if (data < root->key) {
    if (!root->left) {
      root->left = new Node<T>(data, nullptr, nullptr);
      return root->left;
    } else {
      return insert(data, root->left);
    }
  } else {
    if (!root->right) {
      root->right = new Node<T>(data, nullptr, nullptr);
      return root->right;
    } else {
      return insert(data, root->right);
    }
  }
}
