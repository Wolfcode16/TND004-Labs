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
    if (V[r] < V[s])
    {
        V[r] += V[s];   // Add s to r
        V[s] = r;       // Make V[s] point to r
    }
    else
    {
        V[s] += V[r];
        V[r] = s;
    }

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
