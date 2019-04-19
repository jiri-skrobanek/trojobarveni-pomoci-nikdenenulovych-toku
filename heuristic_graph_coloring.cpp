#include "classes.hpp"
#include <set>
using namespace std;

typedef bool checking_function(Graph *, vector<Vertex *>);

void connect_to_source_and_sink(Graph *new_graph, Vertex *source, Vertex *sink, vector<Vertex *> in_component,
                                int &total_flow, bool source_sink);

//dont forget to delete helping graph
deque<Vertex *> bfs(Graph *g);

bool check_source_and_sink(Graph *new_graph, vector<Vertex *> in_component);

Graph *create_new_part_of_graph(Graph *new_graph, vector<Vertex *> in_component);

Graph *copy_graph(Graph *g);

bool heuristic_algorithm_the_first(Graph *g);

bool do_new_step(Graph *new_graph, deque<Vertex *> result, Vertex *current_vertex,
                 vector<Vertex *> in_component, checking_function chf);

bool heuristic_algorithm_the_second(Graph *g);

bool check_the_rest_of_graph(Graph *new_graph, vector<Vertex *> in_component, Vertex *source, Vertex *sink);

void delete_edges(Graph *g, Vertex *v);

/**
Produces a list of vertices in the order a breadth first search would visit them.
 */
deque<Vertex *> bfs(Graph *g) {
    deque < Vertex * > bfs_ordered_vertices;
    queue < Vertex * > q;
    std::set < Vertex * > visited;
    visited.insert(g->vertices[0]);
    q.push(g->vertices[0]);

    while (!q.empty()) {
        Vertex *current_vertex = q.front();
        q.pop();
        bfs_ordered_vertices.push_back(current_vertex); //setting right order

        for (Edge *it : current_vertex->edges) { // bfs adding vertices
            if (!visited.count(it->toVertex)) {
                visited.insert(it->toVertex);
                q.push(it->toVertex);
            }
            if (!visited.count(it->fromVertex)) {
                visited.insert(it->fromVertex);
                q.push(it->fromVertex);
            }
        }
    }
    return bfs_ordered_vertices;
}

bool do_new_step(Graph *new_graph, deque<Vertex *> bfs_ordered_vertices, Vertex *current_vertex,
                 vector<Vertex *> in_component, checking_function chf) {
    // Keep increasing excess for as long as possible:
    if (bfs_ordered_vertices.size() == 0) {
        while (current_vertex->value <= current_vertex->get_max_excess()) {
            if (chf(new_graph, in_component)) { return true; }
            current_vertex->value += 6;
        }
        return false;
    }

    Vertex *new_vertex = bfs_ordered_vertices.front();
    bfs_ordered_vertices.pop_front();
    while (current_vertex->value <= current_vertex->get_max_excess()) {
        if (chf(new_graph, in_component)) {
            new_vertex->value = -new_vertex->get_max_excess();
            in_component.push_back(new_vertex);
            new_vertex->in_the_component = true;
            if (do_new_step(new_graph, bfs_ordered_vertices, new_vertex, in_component, chf)) return true;
            in_component.pop_back();
        }
        current_vertex->value += 6; //magic constant (CRT)
    }
    current_vertex->in_the_component = false;
    return false;
}

/**
Determines whether the given graph is colorable.
 */
bool heuristic_algorithm_the_first(Graph *g) {
    int total_flow = 0;

    deque < Vertex * > bfs_ordered_vertices;
    bfs_ordered_vertices = bfs(g);
    vector < Vertex * > excess_set;
    Vertex *current_vertex = bfs_ordered_vertices.front();
    bfs_ordered_vertices.pop_front();
    current_vertex->value = -current_vertex->get_max_excess();
    current_vertex->in_the_component = true;
    excess_set.push_back(current_vertex); // remember what is in the component
    //Actually put the flows in the given graph
    if (do_new_step(g, bfs_ordered_vertices, current_vertex, excess_set, check_source_and_sink)) {
        Vertex *source = new Vertex;
        Vertex *sink = new Vertex;
        connect_to_source_and_sink(g, source, sink, g->vertices, total_flow, 0);
        g->vertices.push_back(source);
        g->vertices.push_back(sink);
        find_maximum_flow(g, source, sink);
        delete_edges(g, source);
        delete_edges(g, sink);
        g->vertices.pop_back();
        g->vertices.pop_back();
        delete source;
        delete sink;
        return true;
    }
    return false;
}

//source_sink == 0, connect every edge from the component to sink
//source_sink == 1, ... to source
void connect_to_source_and_sink(Graph *partial_graph, Vertex *source, Vertex *sink, vector<Vertex *> in_component,
                                int &total_flow, bool source_sink) {

    // first algorithm where you connect just the connected component to the source and sink

    if (!source_sink) {
        for (int i = 0; i < partial_graph->vertices.size(); i++) {
            total_flow += (partial_graph->vertices[i]->value < 0) ? -(partial_graph->vertices[i]->value)
                                                                  : 0; //now correct

            for (int j = 0; j < partial_graph->vertices[i]->edges_out; j++) {
                Edge *e = new Edge();
                e->fromVertex = partial_graph->vertices[i];
                e->toVertex = sink;
                partial_graph->edges.push_back(e);
                partial_graph->vertices[i]->edges.push_back(e);
                sink->edges.push_back(e);
            }
        }
    } else {
        for (int i = 0; i < partial_graph->vertices.size(); i++) {
            total_flow += (partial_graph->vertices[i]->value > 0) ? partial_graph->vertices[i]->value : 0;

            for (int j = 0; j < partial_graph->vertices[i]->edges_out; j++) {
                Edge *e = new Edge();
                e->toVertex = partial_graph->vertices[i];
                e->fromVertex = source;
                partial_graph->edges.push_back(e);
                partial_graph->vertices[i]->edges.push_back(e);
                source->edges.push_back(e);
            }
        }
    }

    for (int i = 0; i < partial_graph->vertices.size(); i++) { // connect rest of the component to the source and sink
        if (partial_graph->vertices[i]->value > 0) {
            for (int j = 0; j < partial_graph->vertices[i]->value; j++) {
                Edge *e = new Edge();
                e->fromVertex = partial_graph->vertices[i];
                e->toVertex = sink;
                partial_graph->edges.push_back(e);
                partial_graph->vertices[i]->edges.push_back(e);
                sink->edges.push_back(e);
            }
        } else {
            for (int j = 0; j < -(partial_graph->vertices[i]->value); j++) {
                Edge *e = new Edge();
                e->toVertex = partial_graph->vertices[i];
                e->fromVertex = source;
                partial_graph->edges.push_back(e);
                partial_graph->vertices[i]->edges.push_back(e);
                source->edges.push_back(e);
            }
        }
    }
}

bool check_source_and_sink(Graph *new_graph, vector<Vertex *> in_component) {
    Vertex *source = new Vertex;
    Vertex *sink = new Vertex;
    int current_flow = 0;
    int total_flow = 0;
    Graph *partial_graph = create_new_part_of_graph(new_graph, in_component); //set edges_out
    connect_to_source_and_sink(partial_graph, source, sink, in_component, total_flow, 0); //needed to do
    partial_graph->vertices.push_back(source);
    partial_graph->vertices.push_back(sink);
    find_maximum_flow(partial_graph, source, sink);

    for (Edge *e : source->edges) {
        current_flow += e->flow;
    }
    if (current_flow < total_flow) {
        partial_graph->~Graph();
        return false;
    }

    //Destructs edges from source and sink
    delete_edges(partial_graph, source);
    delete_edges(partial_graph, sink);

    total_flow = 0;
    connect_to_source_and_sink(partial_graph, source, sink, in_component, total_flow, 1);

    find_maximum_flow(partial_graph, source, sink);

    current_flow = 0;
    for (Edge *e : source->edges) {
        current_flow += e->flow;
    }

    //Destructs partial_graph
    partial_graph->~Graph();
    if (current_flow >= total_flow) {
        return true;
    }
    return false;
}

//Create subgraph from given vertexset
//WHAT IS CREATED AND WHAT TO DELETE AFTERWARDS:
//CREATED: vertices, graph
//NOT CREATED: edges (borrowed from new_graph)
Graph *create_new_part_of_graph(Graph *new_graph, vector<Vertex *> in_component) {
    //fila : maybe create new space for edges not to overwrite the previous data
    Graph *subgraph = new Graph;
    //add desired vertices
    for (unsigned i = 0; i < in_component.size(); i++) {
        Vertex *created = new Vertex;
        created->value = in_component[i]->value;
        created->in_the_component = in_component[i]->in_the_component;
        in_component[i]->id = (int) i;
        subgraph->vertices.push_back(created);
    }
    //Add edges

    for (unsigned i = 0; i < in_component.size(); i++) {
        for (Edge *e: in_component[i]->edges) {
            if (e->toVertex == in_component[i]) {
                if (e->fromVertex->in_the_component) {
                    Edge *x = new Edge;
                    x->toVertex = subgraph->vertices[i];
                    x->fromVertex = subgraph->vertices[e->fromVertex->id];
                    subgraph->edges.push_back(x);
                    subgraph->vertices[i]->edges.push_back(x);
                    subgraph->vertices[e->fromVertex->id]->edges.push_back(x);
                } else {
                    subgraph->vertices[i]->edges_out += 1;
                }
            } else {
                if (!e->toVertex->in_the_component) {
                    subgraph->vertices[i]->edges_out += 1;
                }
            }
        }
    }
    return subgraph;
}

void delete_edges(Graph *g, Vertex *v) {
    for (int i = v->edges.size() - 1; i >= 0; i--) {
        v->edges[i]->toVertex->edges.pop_back();
        v->edges[i]->fromVertex->edges.pop_back();
        g->edges.pop_back();
        delete v->edges[i];
    }
}
/*
bool second_heuristic_algorithm(Graph* g){
  deque<Vertex*> result = bfs(g);
  vector<Vertex*> backtracking;
  vector<Vertex*> in_component;

  Graph* new_graph = copy_graph(g); //new space in memory otherwise same

  //backtracking.push_back(result.front()); it is going to be done in do_new_step
  Vertex* current_vertex = result.front(); result.pop_front();
  in_component.push_back(current_vertex); // remember what is in the component

  return do_new_step(g, new_graph, result, current_vertex, backtracking, in_component,
		     check_the_rest_of_graph); //true if colorable
}

bool check_the_rest_of_graph(Graph* new_graph, vector<Vertex*> in_component, Vertex* source, Vertex* sink);//now this is not important
*/
