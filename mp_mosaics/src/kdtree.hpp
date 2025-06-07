/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include <utility>
#include <algorithm>
#include <deque>

using namespace std;

template <int Dim>
double distance(const Point<Dim>& first, const Point<Dim>& second) {
  double distance = 0.0;
  for (int i = 0; i < Dim; i++)  {
    distance += (first[i] - second[i]) * (first[i] - second[i]);
  }
  return distance;
}

template <int Dim>
bool smallerDimVal(const Point<Dim>& first,
                                const Point<Dim>& second, int curDim)
{
    if (first[curDim] == second[curDim]) {
      return (first < second);
    }

    return (first[curDim] < second[curDim]);
}

template <int Dim>
bool shouldReplace(const Point<Dim>& target,
                                const Point<Dim>& currentBest,
                                const Point<Dim>& potential)
{
    /**
     * @todo Implement this function!
     */
    double current_distance = 0.0;

    double potential_distance = 0.0;

    int i = 0;
    while (i != Dim) {
      current_distance += (target[i] - currentBest[i])*(target[i] - currentBest[i]);
      potential_distance += (target[i] - potential[i])*(target[i] - potential[i]);
      i++;
    }

    if (current_distance == potential_distance) {
      return potential < currentBest;
    }

    return potential_distance < current_distance;
}
template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::copyTree(KDTreeNode* rhs) {
  if (rhs == nullptr) {
    return nullptr;
  }

  KDTreeNode cur = new KDTreeNode(rhs->point);
  cur->left = copyTree(rhs->left);
  cur->right = copyTree(rhs->right);
  return cur;
}

template <int Dim>
template <typename RandIter>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::buildNode(RandIter start, RandIter end, int n, int d, int k_dim) {
    int cur_d = (d + 1) % k_dim;
    if (n == 0) {
      return nullptr;
    }

    // floor((a + b) / 2)
    int middle = (n - 1) / 2;
    int left_n = middle;
    int right_n = n - middle - 1;

    // find k
    RandIter k = start + middle;
    auto cmp = [cur_d](const Point<Dim>& first, const Point<Dim>& second) {
        return smallerDimVal<Dim>(first, second, cur_d);
    };

    // call select
    select(start, end, k, cmp);
    
    KDTreeNode* cur_root = new KDTreeNode(*k);
    cur_root->left = buildNode(start, k, left_n, cur_d, k_dim);
    cur_root->right = buildNode(k + 1, end, right_n, cur_d, k_dim);
    size++;
    return cur_root;
}

template <int Dim>
void KDTree<Dim>::delete_tree(KDTreeNode*& node) {
    if (!node) {
      return;
    }
    delete_tree(node->left);
    delete_tree(node->right);

    delete node;
    node = nullptr;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>>& newPoints)
{
    /**
     * @todo Implement this function!
     */
    // auto it = newPoints.begin();
    // std::cout << "value of it is" << newPoints.size() << std::endl;

    if (newPoints.empty()) {
      root = nullptr;
    } else {
      vector<Point<Dim>> copy = newPoints;
      root = buildNode(copy.begin(), copy.end(), copy.size(), -1, Dim);
    }
}

template <int Dim>
KDTree<Dim>::KDTree(const KDTree<Dim>& other) {
  /**
   * @todo Implement this function!
   */

  root = copyTree(other.root);
  size = other.size;
}

template <int Dim>
const KDTree<Dim>& KDTree<Dim>::operator=(const KDTree<Dim>& rhs) {
  /**
   * @todo Implement this function!
   */
  // check for self assignment
  if (this == &rhs) {
    return;
  }


  if (root) {
    // clear stuff out current
    delete_tree(root);
  }

  root = copyTree(rhs.root);
  size = rhs.size;
  return *this;
}

template <int Dim>
KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
  delete_tree(root);
}


// recurse function that only goes down to leaf node
template <int Dim>
typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::find_node(KDTreeNode* cur_root, const Point<Dim>& query, int d) const {
  d = (d + 1) % Dim;
  // leaf
  if (!(cur_root->left) && !(cur_root->right)) {
    return cur_root;
  }
  
  KDTreeNode* best_node = cur_root;

  bool went_left = false;
  // determine which subtree to search based on dimention
  if (smallerDimVal(query, cur_root->point, d)) {
    // should go left
    // CHECK FOR EQUALITY
    if (cur_root->left) {
      best_node = find_node(cur_root->left, query, d);
      went_left = true;
    }
  } else {
    if (cur_root->right) {
      best_node = find_node(cur_root->right, query, d);
    }
  }

  // back recursion steps
  if (shouldReplace(query, best_node->point, cur_root->point)) {
    best_node = cur_root;
  }


  double radius = distance(best_node->point, query);

  double potential_search = ((cur_root->point)[d] - query[d]) * ((cur_root->point)[d] - query[d]);


  // there could be node in current best_node's child that is closer
  // don't know which dimension this is
  if (potential_search <= radius) {
    KDTreeNode* potential_best = nullptr;
    if (!went_left && cur_root->left) {
      // if previous has gone left
      potential_best = find_node(cur_root->left, query, d);
    } else if (went_left && cur_root->right) {
      potential_best = find_node(cur_root->right, query, d);
    }

    if (potential_best && shouldReplace(query, best_node->point, potential_best->point)) {
        best_node = potential_best;
    }
  }
  return best_node;
}


// // base case is null
// template <int Dim>
// typename KDTree<Dim>::KDTreeNode* KDTree<Dim>::find_node(KDTreeNode* cur_root, const Point<Dim>& query, int d) const {

//   d = (d + 1) % Dim;

//   if (!cur_root)  {
//     return nullptr;
//   }

//   KDTreeNode* best_node;

//   bool gone_left = false;

//   // root node's point is less than query in specified dimension
//   if (smallerDimVal(query, cur_root->point, d)) {
//     best_node = find_node(cur_root->left, query, d);
//     gone_left = true;
//   } else {
//     best_node = find_node(cur_root->right, query, d);
//   }


//   if (!best_node) {
//     best_node = cur_root;
//   }

//   // recursing back, and check if the recursed back node is better
//   if (shouldReplace(query, best_node->point, cur_root->point)) {
//     best_node = cur_root;
//   }
//   double radius = distance(best_node->point, query);

//   double potential_radius = (cur_root->point[d] - query[d]) * (cur_root->point[d] - query[d]);

//   // potential update to best when traversing back

//   if (potential_radius <= radius) {
//     KDTreeNode* potential_node;

//     if (gone_left) {
//       potential_node = find_node(cur_root->right, query, d);
//     } else {
//       potential_node = find_node(cur_root->left, query, d);
//     }

//     if (potential_node && shouldReplace(query, best_node->point, potential_node->point)) {
//       best_node = potential_node;
//     }
//   }

//   return best_node;
// }

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim>& query) const
{
    /**
     * @todo Implement this function!
     */
    // need to have a dimension
   KDTreeNode* best = find_node(root, query, -1);
   return best->point;
}

template <typename RandIter>
void my_swap(RandIter& first, RandIter& second) {
  // swapping values of each iterator
  auto first_val = *first;
  *first = *second;
  *second = first_val;
}

template <typename RandIter, typename Comparator>
RandIter my_partition(RandIter start, RandIter end, Comparator cmp) {
  RandIter pivot = --end;
  RandIter i = start;
  for (RandIter current = start; current != pivot; current++) {
    if (cmp(*current, *pivot)) {
      // current is less than or equal pivot value, swap
      my_swap(i, current);
      i++;
    }
  }
  my_swap(i, pivot);
  return i;
}

template <typename RandIter, typename Comparator>
void select(RandIter start, RandIter end, RandIter k, Comparator cmp)
{
    /**
     * @todo Implement this function!
     */
    if (start == end) {
      return;
    }
    RandIter pivot_iter = my_partition(start, end, cmp);
    if (k == pivot_iter) {
      return;
    } else if (k < pivot_iter) {
      select(start, pivot_iter, k, cmp);
    } else {
      select(pivot_iter + 1, end, k, cmp);
    }
}

