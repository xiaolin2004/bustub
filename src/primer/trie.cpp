#include "primer/trie.h"
#include <memory>
#include <string_view>
#include <utility>
#include "common/exception.h"

namespace bustub {

template <class T>
auto Trie::Get(std::string_view key) const -> const T * {
  auto aux = this->GetRoot();
  for (auto bg = key.begin(); bg != key.end(); bg++) {
    auto child = aux->children_.find(*bg);
    if (child != aux->children_.end() && child->second->is_value_node_) {
      aux = child->second;
    } else {
      return nullptr;
    }
  }
  auto node = dynamic_cast<const TrieNodeWithValue<T> *>(aux.get());
  if (node == nullptr) {
    return nullptr;
  }
  return node->value_.get();
  // You should walk through the trie to find the node corresponding to the key. If the node doesn't exist, return
  // nullptr. After you find the node, you should use `dynamic_cast` to cast it to `const TrieNodeWithValue<T> *`. If
  // dynamic_cast returns `nullptr`, it means the type of the value is mismatched, and you should return nullptr.
  // Otherwise, return the value.
}

template <class T>
auto Trie::Put(std::string_view key, T value) const -> Trie {
  // Note that `T` might be a non-copyable type. Always use `std::move` when creating `shared_ptr` on that value.
  auto _new_root = this->GetRoot()->Clone();
  std::shared_ptr<TrieNode> aux = std::move(_new_root);
  auto new_root = aux;
  for (char bg : key) {
    auto child = aux->children_.find(bg);
    if (child != aux->children_.end() && child->second->is_value_node_) {
      auto new_child = child->second->Clone();
      std::shared_ptr<TrieNode> ptr = std::move(new_child);
      aux->children_[bg] = ptr;
      aux = ptr;
    } else {  // finding the first absent node
      auto new_node =
          std::make_shared<TrieNodeWithValue<T>>(TrieNodeWithValue<T>(std::make_shared<T>(std::move(value))));
      aux->children_[bg] = new_node;
      aux = new_node;
      // copy the visiting node(inherit the map to reuse the children node)
      // construct new node for remaining path
      // remake the upper node till getting to the root
    }
  }
  return Trie(new_root);

  // You should walk through the trie and create new nodes if necessary. If the node corresponding to the key already
  // exists, you should create a new `TrieNodeWithValue`.
}

auto Trie::Remove(std::string_view key) const -> Trie {
  auto _new_root = this->GetRoot()->Clone();
  std::shared_ptr<TrieNode> aux = std::move(_new_root);
  auto new_root = aux;
  for (auto bg = key.begin(); bg != key.end() - 1; bg++) {
    auto child = aux->children_.find(*bg);
    if (child != aux->children_.end() && child->second->is_value_node_) {
      auto new_child = child->second->Clone();
      std::shared_ptr<TrieNode> ptr = std::move(new_child);
      aux->children_[*bg] = ptr;
      aux = ptr;
    } else {
      return *this;
    }
  }
  auto child = aux->children_.find(*key.rbegin());
  if (child != aux->children_.end() && child->second->is_value_node_) {
    if (child->second->children_.empty()) {
      aux->children_.erase(child);
    } else {
      auto new_child = child->second->Clone();
      new_child->is_value_node_ = false;
      aux->children_[*key.rbegin()] = std::shared_ptr<TrieNode>(std::move(new_child));
    }
  }
  return Trie(new_root);
  // You should walk through the trie and remove nodes if necessary. If the node doesn't contain a value any more,
  // you should convert it to `TrieNode`. If a node doesn't have children any more, you should remove it.
}

// Below are explicit instantiation of template functions.
//
// Generally people would write the implementation of template classes and functions in the header file. However, we
// separate the implementation into a .cpp file to make things clearer. In order to make the compiler know the
// implementation of the template functions, we need to explicitly instantiate them here, so that they can be picked up
// by the linker.

template auto Trie::Put(std::string_view key, uint32_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint32_t *;

template auto Trie::Put(std::string_view key, uint64_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint64_t *;

template auto Trie::Put(std::string_view key, std::string value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const std::string *;

// If your solution cannot compile for non-copy tests, you can remove the below lines to get partial score.

using Integer = std::unique_ptr<uint32_t>;

template auto Trie::Put(std::string_view key, Integer value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const Integer *;

template auto Trie::Put(std::string_view key, MoveBlocked value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const MoveBlocked *;

}  // namespace bustub
