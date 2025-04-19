/**
 * @file avltree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

#include <algorithm>

template <class K, class V>
V AVLTree<K, V>::find(const K& key) const
{
    return find(root, key);
}

template <class K, class V>
V AVLTree<K, V>::find(Node* subtree, const K& key) const
{
    if (subtree == NULL)
        return V();
    else if (key == subtree->key)
        return subtree->value;
    else {
        if (key < subtree->key)
            return find(subtree->left, key);
        else
            return find(subtree->right, key);
    }
}

template <class K, class V>
void AVLTree<K, V>::rotateLeft(Node*& t)
{
    functionCalls.push_back("rotateLeft"); // Stores the rotation name (don't remove this)
    // your code here
    // always have a right child?
    Node* child = t->right;
    // set the child to be the new root
    // swap operations
    t->right = child->left;
    child->left = t;

    // update the root
    t = child;
    
    // now t becomes the new root
    // calculate height of original node
    t->left->height = 1 + std::max(heightOrNeg1(t->left->left), heightOrNeg1(t->left->right));
    // height for t
    t->height = 1 + std::max(heightOrNeg1(t->left), heightOrNeg1(t->right));
}

template <class K, class V>
void AVLTree<K, V>::rotateLeftRight(Node*& t)
{
    functionCalls.push_back("rotateLeftRight"); // Stores the rotation name (don't remove this)
    // Implemented for you:
    rotateLeft(t->left);
    rotateRight(t);
}

template <class K, class V>
void AVLTree<K, V>::rotateRight(Node*& t)
{
    functionCalls.push_back("rotateRight"); // Stores the rotation name (don't remove this)
    // your code here
    // Must have a left child
    Node* child = t->left;

    t->left = child->right;
    child->right = t;

    t = child;

    // calculate height of original t node
    t->right->height = 1 + std::max(heightOrNeg1(t->right->left), heightOrNeg1(t->right->right));
    // height for t
    t->height = 1 + std::max(heightOrNeg1(t->left), heightOrNeg1(t->right));
}

template <class K, class V>
void AVLTree<K, V>::rotateRightLeft(Node*& t)
{
    functionCalls.push_back("rotateRightLeft"); // Stores the rotation name (don't remove this)
    // your code here
    rotateRight(t->right);
    rotateLeft(t);
}

template <class K, class V>
void AVLTree<K, V>::rebalance(Node*& subtree)
{
    // your code here
    if (!subtree) {
        return;
    }
    int height = heightOrNeg1(subtree->left) - heightOrNeg1(subtree->right);

    if (abs(height) > 1) {
        // needs to perform rotation
        if (height > 0) {
            // std::cout << "Height >= 2" << std::endl;
            int lsh = heightOrNeg1(subtree->left->left) - heightOrNeg1(subtree->left->right);
            if (lsh > 0) {
                rotateRight(subtree);
            } else {
                rotateLeftRight(subtree);
            }
        } else {
            // std::cout << "Height <= -2" << std::endl;
            int rsh = heightOrNeg1(subtree->right->left) - heightOrNeg1(subtree->right->right);
            if (rsh < 0) {
                rotateLeft(subtree);
            } else {
                rotateRightLeft(subtree);
            }
        }
    }
    subtree->height = 1 + std::max(heightOrNeg1(subtree->left), heightOrNeg1(subtree->right));
}

template <class K, class V>
void AVLTree<K, V>::insert(const K & key, const V & value)
{
    insert(root, key, value);
}

template <class K, class V>
void AVLTree<K, V>::insert(Node*& subtree, const K& key, const V& value)
{
    if (!subtree) {
        subtree = new Node(key, value);
    } else if (subtree->key > key) {
        insert(subtree->left, key, value);
    } else if (subtree->key < key) {
        insert(subtree->right, key, value);
    } else if (subtree->key == key) {
        // do nothing
        return;
    }
    rebalance(subtree);
}

template <class K, class V>
void AVLTree<K, V>::remove(const K& key)
{
    remove(root, key);
}

template <class K, class V>
void AVLTree<K, V>::remove(Node*& subtree, const K& key)
{
    if (subtree == NULL)
        return;

    if (key < subtree->key) {
        remove(subtree->left, key);
    } else if (key > subtree->key) {
        // your code here
        remove(subtree->right, key);
    } else {
        if (subtree->left == NULL && subtree->right == NULL) {
            /* no-child remove */
            // your code here
            delete subtree;
            subtree = nullptr;
        } else if (subtree->left != NULL && subtree->right != NULL) {
            /* two-child remove */
            // need to find iop first
            Node* iop = subtree->left;
            Node* parent = iop;
            while (iop && iop->right) {
                parent = iop;
                iop = iop->right;
            }

            swap(iop, subtree);
            if (parent->left == iop) {
                parent->left = iop->left;
            } else {
                parent->right = iop->left;
            }
            delete iop;
            return;
            // your code here
        } else {
            /* one-child remove */
            // your code here
            if (subtree->left) {
                swap(subtree, subtree->left);
                delete subtree->left;
                subtree->left = nullptr;
            } else {
                swap(subtree, subtree->right);
                delete subtree->right;
                subtree->right = nullptr;
            }
        }
        // your code here
    }
    rebalance(subtree);
}
