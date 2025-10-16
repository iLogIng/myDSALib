// BTreeNode.hpp
#pragma once
#include <vector>
#include <memory>
#include <algorithm>

namespace myDSALib {
namespace Tree {

/**
 * This is a class for B-Tree
 */

template<typename Ty, int Order>
class BTreeNode {
    static_assert(Order >= 3, "B-Tree order must be at least 3");

public:
    static constexpr int MIN_KEYS = Order - 1;      // minimum keys
    static constexpr int MAX_KEYS = 2 * Order - 1;  // maximum keys
    static constexpr int MIN_CHILDREN = Order;      // minimum children
    static constexpr int MAX_CHILDREN = 2 * Order;  // maximum children
    
    using KeyType = Ty;
    using NodePtr = std::shared_ptr<BTreeNode>;
    using KeyVector = std::vector<KeyType>;
    using ChildVector = std::vector<NodePtr>;

private:
    KeyVector keys_;        // the keys
    ChildVector children_;  // the children
    bool is_leaf_;          // if is leaf
    size_t key_count_;      // key count

public:
    explicit BTreeNode(bool is_leaf = true) 
        : is_leaf_(is_leaf), key_count_(0) {
        keys_.reserve(MAX_KEYS);
        if (!is_leaf_) {
            children_.reserve(MAX_CHILDREN);
        }
    }

public:
    // if the key full
    bool is_full() const noexcept { return key_count_ >= MAX_KEYS; }
    // if the node is leaf
    bool is_leaf() const noexcept { return is_leaf_; }
    // keys count
    size_t key_count() const noexcept { return key_count_; }

    // return key const.Ver.
    const KeyType& key_at(size_t index) const { return keys_[index]; }
    // return key
    KeyType& key_at(size_t index) { return keys_[index]; }

    // return child
    NodePtr child_at(size_t index) const { 
        return (index < children_.size()) ? children_[index] : nullptr; 
    }

    // insert key
    void insert_key(size_t index, const KeyType& key) {
        keys_.insert(keys_.begin() + index, key);
        ++key_count_;
    }

    // insert child by index
    void insert_child(size_t index, NodePtr child) {
        children_.insert(children_.begin() + index, child);
    }

    // remove key by index
    KeyType remove_key(size_t index) {
        KeyType key = keys_[index];
        keys_.erase(keys_.begin() + index);
        --key_count_;
        return key;
    }

    // remove child by index
    NodePtr remove_child(size_t index) {
        NodePtr child = children_[index];
        children_.erase(children_.begin() + index);
        return child;
    }

    // find key by key's reference
    size_t find_key(const KeyType& key) const {
        // find the first not less than key's elem
        auto it = std::lower_bound(keys_.begin(), keys_.begin() + key_count_, key);
        return std::distance(keys_.begin(), it);    // the quantity of elem keys before it
    }

    // split the node from middle, return pair<new_node_key,new_node>
    std::pair<KeyType, NodePtr> split() {
        // build a new node
        NodePtr new_node = std::make_shared<BTreeNode>(is_leaf_);
        size_t mid_index = this->key_count_ / 2;    // the split position
        KeyType mid_key = this->keys_[mid_index];

        // keys: move the half right part to new node
        new_node->keys_.assign(this->keys_.begin() + mid_index + 1, this->keys_.end());
        new_node->key_count_ = this->key_count_ - mid_index - 1;  // new node's key count

        // children: if not leaf node, move the child node
        if (!this->is_leaf_) {
            new_node->children_.assign(this->children_.begin() + mid_index + 1, this->children_.end());
            new_node->is_leaf_ = false;
        }

        // operate this node
        this->keys_.resize(mid_index);    // shrink this node
        this->key_count_ = mid_index;
        if (!this->is_leaf_) {
            this->children_.resize(mid_index + 1);
        }

        return {mid_key, new_node};
    }
};

} // namespace Tree
} // namespace myDSALib