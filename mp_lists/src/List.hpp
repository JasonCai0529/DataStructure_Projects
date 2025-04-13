/**
 * @file list.cpp
 * Doubly Linked List (MP 3).
 */

template <class T>
List<T>::List() { 
  // @TODO: graded in mp_lists part 1
    head_ = NULL;
    tail_ = NULL;
    length_ = 0;
}

/**
 * Returns a ListIterator with a position at the beginning of
 * the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::begin() const {
  return List<T>::ListIterator(head_);
}

/**
 * Returns a ListIterator one past the end of the List.
 */
template <typename T>
typename List<T>::ListIterator List<T>::end() const {
  // @TODO: graded in mp_lists part 1
  // Memory Checks
  return List<T>::ListIterator(NULL, tail_);
}

template <typename T>
void List<T>::_destroy() {
  /// @todo Graded in mp_lists part 1
  ListNode* cur = head_;
  while (cur != NULL) {
    ListNode* cur_next = cur->next;
    delete cur;
    cur = cur_next;
  }
  head_ = NULL;
  tail_ = NULL;
}

/**
 * Inserts a new node at the front of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertFront(T const & ndata) {
  /// @todo Graded in mp_lists part 1
  ListNode * newNode = new ListNode(ndata);
  if (head_ == NULL) {
    // this could go wrong
    head_ = newNode;
    tail_ = newNode;
    length_++;
    return;
  }

  // when there is only one node
  if (head_ == tail_) {
    newNode -> next = head_;
    // head prev is NULL for sure
    head_ -> prev = newNode;
    tail_ = head_;
    head_ = newNode;
    length_++;
    return;
  }

  // at least two nodes
  newNode->next = head_;
  head_->prev = newNode;
  head_ = newNode;
  length_++;
}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
template <typename T>
void List<T>::insertBack(const T & ndata) {
  /// @todo Graded in mp_lists part 1

  // if head is no found -> also no tail
  // if head and tail are the same tail exist
  // tail exist
  if (head_ == NULL) {
    insertFront(ndata);
    return;
  }
  ListNode * newNode = new ListNode(ndata);
  
  tail_ -> next = newNode;
  newNode -> prev = tail_;
  // update tail
  tail_ = tail_->next;
  // update length
  length_++;
}

/**
 * Helper function to split a sequence of linked memory at the node
 * splitPoint steps **after** start. In other words, it should disconnect
 * the sequence of linked memory after the given number of nodes, and
 * return a pointer to the starting node of the new sequence of linked
 * memory.
 *
 * This function **SHOULD NOT** create **ANY** new List or ListNode objects!
 *
 * This function is also called by the public split() function located in
 * List-given.hpp
 *
 * @param start The node to start from.
 * @param splitPoint The number of steps to walk before splitting.
 * @return The starting node of the sequence that was split off.
 */
template <typename T>
typename List<T>::ListNode * List<T>::split(ListNode * start, int splitPoint) {
  ListNode * curr = start;

  for (int i = 0; i < splitPoint && curr != NULL; i++) {
    curr = curr->next;
  }

  // Know that curr should be the split point
  // split is disconnect
  if (curr != NULL) {
    if (curr->prev != NULL) {
      curr->prev->next = NULL;
    }
      curr->prev = NULL;
  }
  // std::cout << curr->data << std::endl;
  return curr;
}

/**
 * Modifies the List using the waterfall algorithm.
 * Every other node (starting from the second one) is removed from the
 * List, but appended at the back, becoming the new tail. This continues
 * until the next thing to be removed is either the tail (**not necessarily
 * the original tail!**) or NULL.  You may **NOT** allocate new ListNodes.
 * Note that since the tail should be continuously updated, some nodes will
 * be moved more than once.
 */
template <typename T>
void List<T>::waterfall() {
  /// @todo Graded in part 1
  // only one or two nodes scenerio
  if (head_ == tail_ || head_->next == tail_) {
    return;
  }
  ListNode* cur = head_;
  while (cur->next != tail_) {
    ListNode* back = cur->next;
    // move to back and update tail
    // disconnect
    // skip one
    cur->next = cur->next->next;
    cur->next->prev = cur;

    tail_ -> next = back;
    back -> prev = tail_;
    back->next = NULL;
    tail_ = back;

    cur = cur->next;
  }
}


/**
 * Reverses the current List.
 */
template <typename T>
void List<T>::reverse() {
  reverse(head_, tail_);
}

/**
 * Helper function to reverse a sequence of linked memory inside a List,
 * starting at startPoint and ending at endPoint. You are responsible for
 * updating startPoint and endPoint to point to the new starting and ending
 * points of the rearranged sequence of linked memory in question.
 *
 * @param startPoint A pointer reference to the first node in the sequence
 *  to be reversed.
 * @param endPoint A pointer reference to the last node in the sequence to
 *  be reversed.
 */
template <typename T>
void List<T>::reverse(ListNode *& startPoint, ListNode *& endPoint) {
  /// @todo Graded in mp_lists part 2
  ListNode* end_next = endPoint->next;
  ListNode* start_prev = startPoint->prev;
  if (startPoint == endPoint) {
    // startpoint - prev
    return;
  }
  ListNode* cur = endPoint;
  while (cur != start_prev) {
    ListNode* previous = cur->prev;
    ListNode* next = cur->next;
    // exchange prev and next for a current node
    cur->next = previous;

    cur->prev = next;

    // update cur
    cur = previous;
  }


  // start from the back and work the way to front
  endPoint->prev = start_prev;

  if (start_prev != NULL) {
    start_prev->next = endPoint;
  }
  startPoint->next = end_next;
  if (end_next != NULL) {
    end_next->prev = startPoint;
  }


  if (head_ == startPoint) {
    head_ = endPoint;
  }

  if (tail_ == endPoint) {
    tail_ = startPoint;
  }
}

/**
 * Reverses blocks of size n in the current List. You should use your
 * reverse( ListNode * &, ListNode * & ) helper function in this method!
 *
 * @param n The size of the blocks in the List to be reversed.
 */
template <typename T>
void List<T>::reverseNth(int n) {
  /// @todo Graded in mp_lists part 2
  ListNode* original_tail_ = tail_;
  ListNode* cur = head_;
  while (cur->next != NULL) {
    ListNode* start = cur;
    ListNode* end = cur;
    for (int i = 0; i < n - 1 && cur->next != NULL; i++) {
      cur = cur->next;
    }
    end = cur;
    if (cur->next != NULL) {
      cur = cur->next;
    }
    reverse(start, end);
    if (end == original_tail_) {
      break;
    }
  }
}


/**
 * Merges the given sorted list into the current sorted list.
 *
 * @param otherList List to be merged into the current list.
 */
template <typename T>
void List<T>::mergeWith(List<T> & otherList) {
    // set up the current list
    head_ = merge(head_, otherList.head_);
    tail_ = head_;

    // make sure there is a node in the new list
    if (tail_ != NULL) {
        while (tail_->next != NULL)
            tail_ = tail_->next;
    }
    length_ = length_ + otherList.length_;

    // empty out the parameter list
    otherList.head_ = NULL;
    otherList.tail_ = NULL;
    otherList.length_ = 0;
}

/**
 * Helper function to merge two **sorted** and **independent** sequences of
 * linked memory. The result should be a single sequence that is itself
 * sorted.
 *
 * This function **SHOULD NOT** create **ANY** new List objects.
 *
 * @param first The starting node of the first sequence.
 * @param second The starting node of the second sequence.
 * @return The starting node of the resulting, sorted sequence.
 */
template <typename T>
typename List<T>::ListNode * List<T>::merge(ListNode * first, ListNode* second) {
  /// @todo Graded in mp_lists part 2
  ListNode* head;
  if (first == nullptr) {
    return second;
  }
  if (second == nullptr) {
    return first;
  }
  if (first->data < second->data) {
    head = first;
  } else {
    head = second;
  }

  while (first && second) {
    if (first->data < second->data) {
      ListNode* first_next = first->next;
      ListNode* second_prev = second->prev;

      first->next = second;
      second->prev = first;

      if (second_prev) {
        first->prev = second_prev;
        second_prev->next = first;
      }
      first = first_next;
      if (first_next) {
        first_next->prev = nullptr;
      }
    } else {
      ListNode* second_next = second->next;
      ListNode* first_prev = first->prev;

      second->next = first;
      first->prev = second;
      if (first_prev) {
        first_prev->next = second;
        second->prev = first_prev;
      }
      second = second_next;
      if (second_next) {
        second_next->prev = nullptr;
      }
    }
  }

  return head;
}

/**
 * Sorts a chain of linked memory given a start node and a size.
 * This is the recursive helper for the Mergesort algorithm (i.e., this is
 * the divide-and-conquer step).
 *
 * Called by the public sort function in List-given.hpp
 *
 * @param start Starting point of the chain.
 * @param chainLength Size of the chain to be sorted.
 * @return A pointer to the beginning of the now sorted chain.
 */
template <typename T>
typename List<T>::ListNode* List<T>::mergesort(ListNode * start, int chainLength) {
  /// @todo Graded in mp_lists part 2
  if (chainLength <= 1) {
    return start;
  } else {
    int split_point = chainLength / 2;
    int second_chainLength = chainLength - split_point;
    ListNode* first = start;
    ListNode* second = split(start, split_point);
    return merge(mergesort(start, split_point), mergesort(second, second_chainLength));
  }
}
