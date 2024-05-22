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
// Starts from a single vertex, selects edges based on the vertices already included in the MST
void Graph::mstPrim() const {

    // ---------------- INITIALIZE VECTORS ----------------
    std::vector<int> dist(size + 1, std::numeric_limits<int>::max());
    std::vector<int> path(size + 1, 0);
    std::vector<bool> done(size + 1, false);

    // *** TODO ***
    int start = 1;  // graph vertices are numbered from 1 -- i.e. there is no vertex zero
    dist[start] = 0;
    done[start] = true;
    int v = start;
    int totalWeight = 0;

    while (true)
    {
        // Basically prepares all vertices by updating their minimum connecting edge weight.
        // Iterates over all edges adjecent to vertex v
        for (auto i = table[v].begin(); i != table[v].end(); ++i)
        {
            int u = i->to;
            // If u has not yet been visited
            // AND the minimum weight to connect to this vertice is not found 
            // (infinity > i->weight) then set dist[u] to this i->weight
            // or that the current dist[u] is higher than the weight then a new best path has been found
            if (done[u] == false && dist[u] > i->weight) 
            {
                path[u] = v;            // Updates path to indicate best path to u is from v
                dist[u] = i->weight;    // Updates min distance to u
            }
        }

        // --------- FIND SMALLEST UNDONE DISTANCE VERTEX ---------

        int smallestDist = std::numeric_limits<int>::max();

        // Loops through all vertices to find a vertice v with the smallest distance
        for (int i = 1; i <= size; ++i)             // Loops through all vertices
        {   
            if (!done[i] && dist[i] < smallestDist) // Not yet visited && smaller than the currently known smallest
            {
                smallestDist = dist[i];             // Update currently known smallest to the smaller distance
                v = i;                              // Updates v to the i with the smallest distance
            }
        }

        if (smallestDist == std::numeric_limits<int>::max()) break; // break if all remaining vertexes have infinite distance
        Edge edge(path[v], v, dist[v]);             // Create edge that contains [from vertex, current vertex, weight of the edge]
        totalWeight += dist[v];                     // Update total weight of the MST

        std::cout << edge << std::endl;

        done[v] = true;                             // Mark vertex as done 

    }

    std::cout << "\nTotal weight = " << totalWeight << std::endl;
}

// Kruskal's minimum spanning tree algorithm
// Selects edges solely on their weight
// Sorts all edges from lowest to highest (by weight)
// joins vertices together and makes sure that there are no cycles

void Graph::mstKruskal() const {
    // *** TODO ***
    
    std::vector<Edge> EVec;     // Initialize vector for edges
    int totalWeight = 0;       
    DSets D(size);              // Initialize a Disjoint Set

    // Loop i through all vertices
    for (int i = 1; i <= size; ++i)
    {
        // loop through all adjecent vertices
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

    int counter = 0;    // keep track of how many edges we have added

    // ---------- Kruskal's Algorithm execution ----------
    while (!EVec.empty() && counter < size - 1)
    {
        // Move the smallest edge to the back of the vector
        std::pop_heap(EVec.begin(), EVec.end(), std::greater<Edge>());

        // Access the edge
        Edge e = EVec.back();   // Min edge stored in e
        EVec.pop_back();        // Delete Min edge

        // As long as the two vertexes doesn't create a cycle
        if (D.find(e.from) != D.find(e.to))
        {
            D.join(D.find(e.from), D.find(e.to));   // Join them together
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
