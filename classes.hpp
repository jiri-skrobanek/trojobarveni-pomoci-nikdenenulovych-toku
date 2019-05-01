#include<bits/stdc++.h>
#include <cstddef>

#ifndef CLASSES
#define CLASSES

class Edge;

class Face;

class LayeredVertex;

class LayeredEdge;

class Vertex {
public:
    std::vector<Edge *> edges;
    std::vector<Face *> faces;
    int id = 0;
    int color = -1000; // By default set color to an invalid value.
    unsigned iterating_index = 0;

    int get_max_excess();

    int value = 0;
    bool in_the_component = false;
    int edges_out = 0;

    Face *dual_face;
    LayeredVertex *layered_vertex = NULL;
};

class Edge {
public:
    Face *face1; // On the left side of from -> to
    Face *face2; // On the right side of from -> to

    Vertex *fromVertex;
    Vertex *toVertex;

    Edge *dual_edge;
    LayeredEdge *layered_edge = NULL;

    int flow = 0;
};

class Face {
public:
    std::vector<Edge *> edges;
    std::vector<Vertex *> vertices;

    Vertex *dual_vertex;
    int id = 0;
};

class Graph {
public:
    std::vector<Vertex *> vertices;
    std::vector<Edge *> edges;
    std::vector<Face *> faces;

    Graph *dual_graph;

    Graph *get_dual_graph();

    //functions for bruteforce algorithm
    bool is_ok(Vertex *v, int c);

    bool graph_coloring(unsigned vertex_position);

    bool can_be_coloured();

    void print_solution();

    void euler_path();

    void flow_dfs(Vertex *v);

    bool from_flow_to_color(bool (*)(Graph *));

    void print_graph();

    void label_edges();

    bool heuristic_graph_coloring();

    ~Graph();
};

class LayeredVertex {
public:
    std::vector<LayeredEdge *> in_edges;
    std::vector<LayeredEdge *> out_edges;

    int out_count = 0;
    int invalid_count = 0; // The number of dead-end edges from vertex

    Vertex *graph_vertex;
    int distance;
};

class LayeredEdge {
public:
    LayeredVertex *fromVertex;
    LayeredVertex *toVertex;

    int flow = 0;
    int capacity = 1;

    bool invalid = false; // Leading to dead end.

    Edge *graph_edge;
};

class LayeredNetwork {
public:
    std::vector<LayeredVertex *> vertices;
    std::vector<LayeredEdge *> edges;
};

void Join(Graph *, Face *, Vertex *, Vertex *);

void Split(Graph *, Edge *);

Graph *BuildGraph(int, bool, int);

#endif // CLASSES
