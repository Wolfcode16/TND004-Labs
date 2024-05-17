/*********************************************
 * file:	~\code4b\graph.h                  *
 * remark:implementation of undirected graphs *
 **********************************************/

#include <iostream>
#include <algorithm>
#include <format>
#include <cassert>     // assert
#include <limits>      // std::numeric_limits
#include <algorithm>   // std::make_heap(), std::pop_heap(), std::push_heap()

#include "graph.h"
#include "dsets.h"

// Note: graph vertices are numbered from 1 -- i.e. there is no vertex zero

// -- CONSTRUCTORS

// Create a graph with n vertices and no vertices
Graph::Graph(int n) : table(n + 1), size{n}, n_edges{0} {
    assert(n >= 1);
}

Graph::Graph(const std::vector<Edge> &V, int n) : Graph{n} {
    for (auto e : V) {
        insertEdge(e);
    }
}

// -- MEMBER FUNCTIONS

// insert undirected edge e
// update weight if edge e is present
void Graph::insertEdge(const Edge &e) {
    assert(e.from >= 1 && e.from <= size);
    assert(e.to >= 1 && e.to <= size);

    auto edge_insertion = [&T = this->table, &n = this->n_edges](const Edge &e1) {
        if (auto it = std::find_if(begin(T[e1.from]), end(T[e1.from]),
                                   [e1](const Edge &ed) { return e1.links_same_nodes(ed); });
            it == end(T[e1.from])) {
            T[e1.from].push_back(e1);  // insert new edge e1
            ++n;                       // increment the counter of edges
        } else {
            it->weight = e1.weight;  // update the weight
        }
    };

    edge_insertion(e);
    edge_insertion(e.reverse());
}

// remove undirected edge e
void Graph::removeEdge(const Edge &e) {
    assert(e.from >= 1 && e.from <= size);
    assert(e.to >= 1 && e.to <= size);

    auto edgeRemoval = [&T = this->table, &n = this->n_edges](const Edge &e1) {
        auto it = std::find_if(begin(T[e1.from]), end(T[e1.from]),
                               [e1](const Edge &ed) { return e1.links_same_nodes(ed); });

        assert(it != end(T[e1.from]));
        T[e1.from].erase(it);  // remove edge e1
        --n;                   // decrement the counter of edges
    };

    edgeRemoval(e);
    edgeRemoval(e.reverse());
}

// Prim's minimum spanning tree algorithm
void Graph::mstPrim() const {
    std::vector<int> dist(size + 1, std::numeric_limits<int>::max());
    std::vector<int> path(size + 1, 0);
    std::vector<bool> done(size + 1, false);

    // *** TODO ***
    int start = 1;
    
    for (int i = 1; i <= size; ++i)
    {
        dist[i] = std::numeric_limits<int>::max();
        path[i] = 0;
        done[i] = false;
    }

    dist[start] = 0;
    done[start] = true;
    int v = start;
    int totalWeight = 0;

    while (true)
    {
        for (auto i = table[v].begin(); i != table[v].end(); ++i)
        {
            int u = i->to;
            if (done[u] == false && dist[u] > i->weight) {
                path[u] = v;
                dist[u] = i->weight;
            }
        }

        // --------- FIND SMALLEST UNDONE DISTANCE VERTEX ---------

        int smallestDist = std::numeric_limits<int>::max();

        for (int i = 1; i <= size; ++i)
        {
            if (!done[i] && dist[i] < smallestDist)
            {
                smallestDist = dist[i];
                v = i;
            }
        }

        if (smallestDist == std::numeric_limits<int>::max()) break;
        Edge edge(path[v], v, dist[v]);
        totalWeight += dist[v];

        std::cout << edge << std::endl;

        done[v] = true;

    }

    std::cout << "\nTotal weight = " << totalWeight << std::endl;
}

// Kruskal's minimum spanning tree algorithm
void Graph::mstKruskal() const {
    // *** TODO ***
    
    std::vector<Edge> EVec;    // Vector of edges
    int totalWeight = 0;
    DSets D(size);

    // Create V to store all edges in the graph
    for (int i = 1; i <= size; ++i)     // Loop i through the table
    {
        for (auto j = table[i].begin(); j != table[i].end(); ++j)
        {
            // makes sure that it only goes one way and does't duplicate the same edge the opposite direction.
            if (i < j->to)
            {
                EVec.push_back(*j); // Add edge into Edge Vector (EVec)
            }
        }
    }

    // make_heap : In this case creates a min heap (smallest element = root) due to std::greater
    std::make_heap(EVec.begin(), EVec.end(), std::greater<Edge>());

    int counter = 0;

    while (!EVec.empty() && counter < size - 1)
    {
        // reorganize the heap, ensures that the minelement is at top
        std::pop_heap(EVec.begin(), EVec.end(), std::greater<Edge>());

        Edge e = EVec.back();   // Min edge stored in e
        EVec.pop_back();        // Delete Min edge

        // As long as the two vertexes doesn't belong to the same tree
        if (D.find(e.from) != D.find(e.to))
        {
            D.join(D.find(e.from), D.find(e.to));
            totalWeight += e.weight;
            std::cout << e << std::endl;
            counter++;
        }
    }
    std::cout << "\nTotal weight: " << totalWeight << std::endl;
}

// print graph
void Graph::printGraph() const {
    std::cout << std::format("{:-<66}\n", '-');
    std::cout << "Vertex  adjacency lists\n";
    std::cout << std::format("{:-<66}\n", '-');

    for (int v = 1; v <= size; v++) {
        std::cout << std::format("{:4} : ", v);
        for (auto const &e : table[v]) {
            std::cout << std::format("({:2}, {:2}) ", e.to, e.weight);
        }
        std::cout << "\n";
    }
    std::cout << std::format("{:-<66}\n", '-');
}
