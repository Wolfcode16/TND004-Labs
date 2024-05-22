/*********************************************
 * file:	~\code4b\dsets.cpp                *
 * remark: implementation of disjoint sets    *
 **********************************************/

#include <iostream>
#include <format>
#include <cassert>

#include "dsets.h"

// -- CONSTRUCTORS

DSets::DSets(int theSize) : V(theSize + 1) {  // slot zero is not used
    assert(theSize > 0);
    init();
}

// -- MEMBER FUNCTIONS

// create initial sets
void DSets::init() {
    for (auto i = 1; i < std::ssize(V); ++i) {
        V[i] = -1;  // every disjoint set has one node -- singleton
    }
}

// join sets named r and s where r != s
// i.e. join trees with roots r and s
void DSets::join(int r, int s) {
    assert(r != s);
    assert(r >= 1 && r <= std::ssize(V) - 1);
    assert(s >= 1 && s <= std::ssize(V) - 1);
    assert(V[r] < 0);
    assert(V[s] < 0);

    // simple union
    // V[r] = s;

    // *** TODO ***
    // Do Union by Size
    // Smaller set is merged with larger set
    if (V[r] < V[s])    // of V[r] has a bigger size of set than V[s] (negative)
    {
        V[r] += V[s];   // Add s to r, the size of the set increases
        V[s] = r;       // Make V[s] point to r
    }
    else
    {
        V[s] += V[r];
        V[r] = s;
    }

     /*
        Example: 
        V = [-1, 0, -1, -1] :   Vector of elements 
                                - Negative: size of set (if its a root of a set)
                                - Positive: Parent of the element (if it's a part of a set) 


        r = 0, s = 1        (two different roots of sets we want to union)
        
        V[r] += V[s]; --> -1 + (-1) = -2 = V[r]
        V[s] = r;     --> V[s] = 0
        
        V = [-2, 0, -1, -1]

        "s" (1) now points at 0 indicating that it is its parent.
        0 is the root of the new set containing 2 elements.
     */
}

// return name of current set for x
// i.e. return root of tree for x
int DSets::find(int x) {
    assert(x >= 1 && x <= std::ssize(V) - 1);

    // simple find
    if (V[x] < 0)                   // If negative then x is the root 
    {
        return x;                   // Return the root
    } 
    else 
    {
        return V[x] = find(V[x]);   // Recursive func that will run until root found
    }

    // *** TODO ***
    // find with path compression
    // See above, recursive function runs until root is found
    // Sets each node along the way to have their parent as the root.
    // Result: Tree is flattened whereby each nodes parent is the root.

    /*
        Example:
        1 
         \
          2 
           \
            3
        V = [-1, 1, 2]

        find(3) --> V[3] = find(V[3]) = find(2) 
        find(2) --> V[2] = find(V[2]) = find(1)
        find(1) --> V[1] = find(V[1]) = find(1)
        find(1) --> V[1] = -1 < 0 --> return 1
        V[1] = find(1) = V[2] = find(2) = V[3] = find(3) = 1  (1 is the root)

        V = [-1, 1, 1]
    */
}

// just in case ...
void DSets::print() const {
    std::cout << "\n";

    for (auto i = 1; i < std::ssize(V); i++) {
        std::cout << std::format("{:4}", i);
    }
    std::cout << "\n";

    for (auto u : V) {
        std::cout << std::format("{:4}", u);
    }
    std::cout << "\n";
}
