/*********************************************
 * file:	~\code4a\digraph.cpp              *
 * remark: implementation of directed graphs  *
 **********************************************/

#include <iostream>
#include <algorithm>
#include <limits>  //std::numeric_limits
#include <vector>
#include <cassert>
#include <queue>
#include <format>

#include "digraph.h"

// Note: graph vertices are numbered from 1 -- i.e. there is no vertex zero

// -- CONSTRUCTORS

Digraph::Digraph(int n)
    : table(n + 1)  // slot zero not used
    , size{n}       // number of verices
    , n_edges{0}
    , dist(n + 1)
    , path(n + 1)
    , done(n + 1) {
    assert(n >= 1);
    // Note: graph vertices are numbered from 1 -- i.e. there is no vertex zero
}

// Create a digraph with n vertices and the edges in V
Digraph::Digraph(const std::vector<Edge>& V, int n) : Digraph{n} {
    for (auto e : V) {
        insertEdge(e);
    }
}

// -- MEMBER FUNCTIONS

// insert directed edge e = (u, v, w)
// update weight w if edge (u, v) is present
void Digraph::insertEdge(const Edge& e) {
    assert(e.from >= 1 && e.from <= size);
    assert(e.to >= 1 && e.to <= size);

    // Check if edge e already exists
    if (auto it = std::find_if(begin(table[e.from]), end(table[e.from]),
                               [e](const Edge& ed) { return e.links_same_nodes(ed); });
        it == end(table[e.from])) {
        table[e.from].push_back(e);  // insert new edge e
        ++n_edges;
    } else {
        it->weight = e.weight;  // update the weight
    }
}

// remove directed edge e
void Digraph::removeEdge(const Edge& e) {
    assert(e.from >= 1 && e.from <= size);
    assert(e.to >= 1 && e.to <= size);

    auto it = std::find_if(begin(table[e.from]), end(table[e.from]),
                           [e](const Edge& ed) { return e.links_same_nodes(ed); });

    assert(it != end(table[e.from]));
    table[e.from].erase(it);
    --n_edges;
}

// construct unweighted single source shortest path-tree for start vertex s
void Digraph::uwsssp(int s) const {
    assert(s >= 1 && s <= size);

    // *** TODO ***
    // dist vector should be initialized with std::numeric_limits<int>::max()
    std::queue<int> Q;

    for (int j = 1; j <= size; ++j)
    {
        dist[j] = std::numeric_limits<int>::max();
        path[j] = 0;
    }

    dist[s] = 0;
    Q.push(s);  

    while (!Q.empty())
    {
        int v = Q.front();
        Q.pop();

        for (auto i = table[v].begin(); i != table[v].end(); ++i)
        {
            int u = i->to;

            if (dist[u] == std::numeric_limits<int>::max())
            {
                dist[u] = dist[v] + 1;
                path[u] = v;
                Q.push(u);
            }
        }
    }

}

// construct positive weighted single source shortest path-tree for start vertex s
// Dijktra�s algorithm
void Digraph::pwsssp(int s) const {
    assert(s >= 1 && s <= size);

    // *** TODO ***
    // dist vector should be initialized with std::numeric_limits<int>::max()
    for (int j = 1; j <= size; j++)
    {
        dist[j] = std::numeric_limits<int>::max();
        path[j] = 0;
        done[j] = false;
    }

    dist[s] = 0;
    done[s] = true;
    int v = s;

    while (true)
    {
        for (auto i = table[v].begin(); i != table[v].end(); ++i)
        {
            int u = i->to;
            if (done[u] == false && dist[u] > (dist[v] + i->weight))
            {
                dist[u] = dist[v] + i->weight;
                path[u] = v;
            }
        }

        int smallestDist = std::numeric_limits<int>::max();

        // ----------------- FIND SMALLEST UNDONE DISTANCE VERTEX -----------------
        // - Go to the next vertex that has not been marked done
        // - Set that dist to the smallest dist
        // - Set v to the current vertex.
        // - go to next vertex, repeat until all vertex has been visited and marked.
        for (int i = 1; i <= size; ++i)
        {
            // If it hasn't been marked and is smaller than the current smallestDist
            if (!done[i] && dist[i] < smallestDist)
            {
                smallestDist = dist[i];
                v = i;
            }
        }

        // If it could not find any distances 
        if (smallestDist == std::numeric_limits<int>::max()) break;

        // If there were no vertexes
        done[v] = true;
    }
}

// print graph
void Digraph::printGraph() const {
    std::cout << std::format("{:-<66}\n", '-');
    std::cout << "Vertex  adjacency lists\n";
    std::cout << std::format("{:-<66}\n", '-');

    for (int v = 1; v <= size; ++v) {
        std::cout << std::format("{:4} : ", v);
        for (auto const& e : table[v]) {
            std::cout << std::format("({:2}, {:2}) ", e.to, e.weight);
        }
        std::cout << "\n";
    }
    std::cout << std::format("{:-<66}\n", '-');
}

// print shortest path tree for s
void Digraph::printTree() const {
    std::cout << std::format("{:-<22}\n", '-');
    // std::cout << "----------------------\n";
    std::cout << "vertex    dist    path\n";
    std::cout << std::format("{:-<22}\n", '-');
    // std::cout << "----------------------\n";

    for (int v = 1; v <= size; ++v) {
        std::cout << std::format("{:4} : {:6} {:6}\n", v,
                                 ((dist[v] == std::numeric_limits<int>::max()) ? -1 : dist[v]),
                                 path[v]);
    }
    std::cout << std::format("{:-<22}\n", '-');
    // std::cout << "----------------------\n";
}

// print shortest path from s to t and the corresponding path length
// Hint: consider using recursion
void Digraph::printPath(int t) const {
    assert(t >= 1 && t <= size);

    printHelp(t);

    std::cout << "(" << dist[t] << ")";
}


void Digraph::printHelp(int t) const 
{

    if (path[t] == 0) {
        std::cout << " " << t << " ";

        return;
    }

    printHelp(path[t]);
  
    std::cout << " " << t << " ";

}