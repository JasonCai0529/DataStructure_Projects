/* Your code here! */
#include "dsets.h"

void DisjointSets::addelements(int num) {
    for (int i = 0; i < num; i++) {
        dset_.push_back(-1);
    }
}

    /**
     * This function should compress paths and works as described in lecture.
     * @return the index of the root of the up-tree in which the parameter
     *  element resides.
     */
int DisjointSets::find(int elem) {
    if (dset_[elem] < 0) {
        return elem;
    }

    return find(dset_[elem]);
}

    /**
     * This function should be implemented as union-by-size. That is, when you
     * setunion two disjoint sets, the smaller (in terms of number of nodes)
     * should point at the larger. This function works as described in lecture,
     * except that you should not assume that the arguments to setunion are
     * roots of existing uptrees.
     *
     * Your setunion function SHOULD find the roots of its arguments before
     * combining the trees. If the two sets are the same size, make the tree
     * containing the second argument point to the tree containing the first.
     * (Note that normally we could break this tie arbitrarily, but in this case
     * we want to control things for grading.)
     *
     * @param a Index of the first element to union
     * @param b Index of the second element to union
     */
void DisjointSets::setunion(int a, int b) {
    // need to find who has the larger size
    // smaller to larger -->  dset_[smaller] = larger(eitehr a or b)
    int a_root = find(a);
    int b_root = find(b);
    if (dset_[a_root] < dset_[b_root]) {
        // then a is larger
        // should update its elements
        int b_size = dset_[b_root];
        dset_[b_root] = a_root;
        dset_[a_root] += b_size;
    } else if (dset_[b_root] < dset_[a_root]) {
        int a_size = dset_[a_root];
        dset_[a_root] = b_root;
        dset_[b_root] += a_size;
    } else {
        int b_size = dset_[b_root];
        dset_[b_root] = a_root;
        dset_[a_root] += b_size;
    }

}
    
    /**
     * This function should return the number of nodes in the up-tree containing 
     * the element.
     * @return number of nodes in the up-tree containing the element
     */
int DisjointSets::size(int elem) {
    return abs(dset_[find(elem)]);
}



