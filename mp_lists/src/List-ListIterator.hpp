#include "List.h"

class ListIterator : public std::iterator<std::bidirectional_iterator_tag, T> {
  private:
    // @TODO: graded in mp_lists part 1
    ListNode* position_;
    ListNode* last_element_;

  public:
    ListIterator() : position_(NULL) { }
    ListIterator(ListNode* x) : position_(x) { }
    ListIterator(ListNode* x, ListNode* y) : position_(x), last_element_(y) {}


    // Pre-Increment, ++iter
    ListIterator& operator++() {
        // increment -> return
        if (position_ != NULL) {
            position_ = position_->next;
        }
        return *this;
    }
    
    // Post-Increment, iter++ ????????????
    ListIterator operator++(int) {
        // @TODO: graded in mp_lists part 1
        if (position_ == NULL) {
            return NULL;
        }
        ListNode* temp = position_;
        position_ = position_->next;
        // What should I return inside?
        return ListIterator(temp);
    }

    // Pre-Decrement, --iter
    ListIterator& operator--() {
        if (position_ != NULL) {
            position_ = position_->prev;
        }
        return *this;
    }

    // Post-Decrement, iter-- ?????????????
    ListIterator operator--(int) {
        if (position_ == NULL) {
            // std::cout << last_element_->data << std::endl;
            position_ = last_element_;
            return *this;
        }
        ListNode* temp = position_;
        position_ = position_->prev;
        return ListIterator(temp);
    }

    bool operator!=(const ListIterator& rhs) {
        return (position_ != rhs.position_);
    }

    bool operator==(const ListIterator& rhs) {
        return !(*this != rhs);
    }

    const T& operator*() {
        return position_->data;
    }

    const T* operator->() {
        return &(position_->data);
    }
};
