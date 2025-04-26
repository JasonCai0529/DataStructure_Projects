/**
 * @file heap.cpp
 * Implementation of a heap class.
 */
#include <cstddef>

template <class T, class Compare>
class heap;

template <class T, class Compare>
size_t heap<T, Compare>::root() const
{
    return 1;
}

template <class T, class Compare>
bool heap<T, Compare>::isLeaf(size_t idx) {
    // doesn't have a left child
    return (idx * 2) >= _elems.size();
}

template <class T, class Compare>
size_t heap<T, Compare>::leftChild(size_t currentIdx) const
{
    // @TODO Update to return the index of the left child.
    // always check for valid before calling this function
    size_t idx = currentIdx * 2;
    if (idx >= _elems.size()) {
        return -1;
    }
    return idx;
}

template <class T, class Compare>
size_t heap<T, Compare>::rightChild(size_t currentIdx) const
{
    // @TODO Update to return the index of the right child.
    size_t idx = (currentIdx * 2) + 1;
    if (idx >= _elems.size()) {
        return -1;
    }
    return idx;
}

template <class T, class Compare>
size_t heap<T, Compare>::parent(size_t currentIdx) const
{
    // @TODO Update to return the index of the parent.
    // will always have a child for more than two sized array
    size_t idx = currentIdx / 2;
    if (idx <= 0) {
        std::cout << "ILLEGAL INDEX IN PARENT FUNCTION" << std::endl;
        return -1;
    }
    return idx;
}

template <class T, class Compare>
bool heap<T, Compare>::hasAChild(size_t currentIdx) const
{
    // @TODO Update to return whether the given node has a child
    return (currentIdx * 2 < _elems.size());
}

template <class T, class Compare>
size_t heap<T, Compare>::maxPriorityChild(size_t currentIdx) const
{
    // @TODO Update to return the index of the child with highest priority
    ///   as defined by higherPriority()

    if (!hasAChild(currentIdx)) {
        return currentIdx;
    } else {
        // if right doesn't exist
        if ((currentIdx * 2 + 1) >= _elems.size()) {
            return currentIdx * 2;
        }
    }
    
    // determine who has the higher priority
    return higherPriority(_elems[currentIdx * 2], _elems[currentIdx * 2 + 1]) ? (currentIdx*2) : (currentIdx*2 + 1);
}

template <class T, class Compare>
void heap<T, Compare>::heapifyDown(size_t currentIdx)
{
    // check is leaf or not
    // if (hasAChild(currentIdx)) {
    //     size_t left = currentIdx * 2;
    //     size_t right = currentIdx * 2 + 1;
    //     size_t child_idx = left;
    //     if (right < _elems.size() && (higherPriority(_elems[right], _elems[left]))) {
    //         child_idx = right;
    //     }


    //     if (higherPriority(_elems[child_idx], _elems[currentIdx])) {
    //         std::swap(_elems[child_idx], _elems[currentIdx]);
    //         heapifyDown(child_idx);
    //     }
    // }


    if (hasAChild(currentIdx)) {
        size_t childIdx = maxPriorityChild(currentIdx);
        if (higherPriority(_elems[childIdx], _elems[currentIdx])) {
            std::swap(_elems[childIdx], _elems[currentIdx]);
            heapifyDown(childIdx);
        }
    }
}

// if (maxPriorityChild(currentIdx) == currentIdx || !hasAChild(currentIdx)) {
//         return;
//     } else {
//         size_t pri_idx = maxPriorityChild(currentIdx);
//         if (higherPriority(_elems[pri_idx], _elems[currentIdx])) {
//             // there is a child has the higher priority
//             std::swap(_elems[pri_idx], _elems[currentIdx]);
//             heapifyDown(pri_idx);
//         }
//     }

template <class T, class Compare>
void heap<T, Compare>::heapifyUp(size_t currentIdx)
{
    // if (currentIdx == root()) {
    //     return;
    // } else {
    //     size_t parentIdx = parent(currentIdx);
    //     if (higherPriority(_elems[currentIdx], _elems[parentIdx])) {
    //         std::swap(_elems[currentIdx], _elems[parentIdx]);
    //         heapifyUp(parentIdx);
    //     }
    // }


    if (currentIdx <= 1) {
        return;
    } else {
        size_t parent_idx = parent(currentIdx);

        if (higherPriority(_elems[currentIdx], _elems[parent_idx])) {
            std::swap(_elems[currentIdx], _elems[parent_idx]);
            heapifyUp(parent_idx);
        }
    }
}

template <class T, class Compare>
heap<T, Compare>::heap()
{
    // index 1 based heap
    _elems.push_back(T());
}

template <class T, class Compare>
void heap<T, Compare>::buildHeap() {
    for (size_t i = parent(_elems.size() - 1); i > 0; i--) {
        heapifyDown(i);
    }
}

template <class T, class Compare>
heap<T, Compare>::heap(const std::vector<T>& elems) 
{
    // @TODO Construct a heap using the buildHeap algorithm
    _elems.push_back(T());

    for (unsigned int i = 1; i <= elems.size(); i++) {
        _elems.push_back(elems[i - 1]);
    }
    // for (unsigned int i = _elems.size() - 1; i > 0; i--) {
    //     heapifyDown(i);
    // }
    buildHeap();
    // for (unsigned int i = 2; i < _elems.size(); i++) {
    //     heapifyUp(i);
    // }
}

template <class T, class Compare>
T heap<T, Compare>::pop()
{
    // @TODO Remove, and return, the element with highest priority
    // get rid of root and swap this with a max priority of its child
    if (_elems.size() < 2) {
        std::cout << "THERE IS NO ELEMENTS IN THE HEAP" << std::endl;
    }

    T min = _elems[1];

    _elems[1] = _elems[_elems.size() - 1];

    _elems.erase(_elems.begin() + (_elems.size() - 1));

    heapifyDown(1);

    return min;
}

template <class T, class Compare>
T heap<T, Compare>::peek() const
{
    // @TODO Return, but do not remove, the element with highest priority
    if (_elems.size() > 1) {
        return _elems[1];
    }
    return T();
}

template <class T, class Compare>
void heap<T, Compare>::push(const T& elem)
{
    // @TODO Add elem to the heap
    _elems.push_back(elem);

    heapifyUp(_elems.size() - 1);
}

template <class T, class Compare>
void heap<T, Compare>::updateElem(const size_t & idx, const T& elem)
{
    // @TODO In-place updates the value stored in the heap array at idx
    // Corrects the heap to remain as a valid heap even after update
    _elems[idx] = elem;

    // heapify up or down?

    if (hasAChild(idx)) {
        // check if child has a higher priority
        size_t max_idx = maxPriorityChild(idx);
        if (higherPriority(_elems[max_idx], _elems[idx])) {
            heapifyDown(idx);
        }
    } else {
        heapifyUp(idx);
    }
}


template <class T, class Compare>
bool heap<T, Compare>::empty() const
{
    if (_elems.size() <= 1) {
        return true;
    }
    return false;
}

template <class T, class Compare>
void heap<T, Compare>::getElems(std::vector<T> & heaped) const
{
    for (size_t i = root(); i < _elems.size(); i++) {
        heaped.push_back(_elems[i]);
    }
}


template <class T, class Compare>
void heap<T, Compare>::printHeap() {
    for (unsigned int i = 0; i < _elems.size(); i++) {
        if (i == 0) {
            std::cout << "< ";
        }
        std::cout << _elems[i] << " ";

        if (i == _elems.size() - 1) {
            std::cout << ">" << std::endl;
        }
    }
}


template <class T, class Compare>
void heap<T, Compare>::insert(const T& elem) {
    _elems.push_back(elem);
    heapifyUp(_elems.size() - 1);
}


template <class T, class Compare>
T heap<T, Compare>::remove() {
    T s = _elems[root()];

    _elems[root()] = _elems[_elems.size() - 1];

    _elems.pop_back();

    heapifyDown(root());

    return s;
}
