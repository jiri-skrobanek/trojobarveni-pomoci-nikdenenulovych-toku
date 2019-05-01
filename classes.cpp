#include <cstdlib>
#include <algorithm>
#include <stack>

#include "classes.hpp"

/**
Connects two unconnected vertices that are part of the same face.
*/
void Join(Graph *g, Face *f1, Vertex *v1, Vertex *v2) {
    Edge *created_edge = new Edge;
    Face *created_face = new Face;

    //Add new elements to the graph:
    g->edges.push_back(created_edge);
    g->faces.push_back(created_face);

    //Update created edge:
    created_edge->fromVertex = v1;
    created_edge->toVertex = v2;

    //Add new elements to the vertices of the added edge:
    v1->edges.push_back(created_edge);
    v2->edges.push_back(created_edge);
    v1->faces.push_back(created_face);
    v2->faces.push_back(created_face);

    Vertex *lower_vertex;

    // Get vertices and edges for new faces:
    std::vector < Vertex * > first_vertex_set, second_vertex_set;
    std::vector < Edge * > first_edge_set, second_edge_set;

    unsigned i = 0;
    // vertices are ordered clockwise (with respect to the face)
    // starts on perimeter of the face, goes clockwise until it finds on of the v1, v2
    while (f1->vertices[i] != v1 && f1->vertices[i] != v2) {
        first_vertex_set.push_back(f1->vertices[i]);
        first_edge_set.push_back(f1->edges[i]);
        i++;
    }

    lower_vertex = f1->vertices[i];

    // Vertex on both sides:
    first_vertex_set.push_back(f1->vertices[i]);
    first_edge_set.push_back(created_edge);
    second_vertex_set.push_back(f1->vertices[i]);
    second_edge_set.push_back(f1->edges[i]);
    i++;

    while (f1->vertices[i] != v1 && f1->vertices[i] != v2) {
        second_vertex_set.push_back(f1->vertices[i]);
        second_edge_set.push_back(f1->edges[i]);
        i++;
    }

    // Vertex on both sides:
    first_vertex_set.push_back(f1->vertices[i]);
    first_edge_set.push_back(f1->edges[i]);
    second_vertex_set.push_back(f1->vertices[i]);
    second_edge_set.push_back(created_edge);
    i++;
    //complete first vertex/edge-set
    while (i < f1->vertices.size()) {
        first_vertex_set.push_back(f1->vertices[i]);
        first_edge_set.push_back(f1->edges[i]);
        i++;
    }

    created_edge->face1 = f1;
    created_edge->face2 = created_face;

    //assign these set to f1 and created_face (for better understanding, draw a picture)
    if (lower_vertex == v1) {
        created_edge->face1->edges = second_edge_set;
        created_edge->face1->vertices = second_vertex_set;

        created_edge->face2->edges = first_edge_set;
        created_edge->face2->vertices = first_vertex_set;
    } else {
        created_edge->face1->edges = first_edge_set;
        created_edge->face1->vertices = first_vertex_set;

        created_edge->face2->edges = second_edge_set;
        created_edge->face2->vertices = second_vertex_set;
    }


    //Update faces for edges in created_face:
    for (unsigned i = 0; i < created_edge->face2->edges.size(); i++) {
        if (created_edge->face2->edges[i]->face1 == f1 && created_edge->face2->edges[i] != created_edge) {
            created_edge->face2->edges[i]->face1 = created_edge->face2;
        }
        if (created_edge->face2->edges[i]->face2 == f1 && created_edge->face2->edges[i] != created_edge) {
            created_edge->face2->edges[i]->face2 = created_edge->face2;
        }
    }

    //Update faces for vertices in created_face:
    for (unsigned i = 0; i < created_edge->face2->vertices.size(); i++) {
        if (created_edge->face2->vertices[i] != v1 && created_edge->face2->vertices[i] != v2) {
            std::vector<Face *>::iterator it = std::find(created_edge->face2->vertices[i]->faces.begin(),
                                                         created_edge->face2->vertices[i]->faces.end(), f1);
            *(it) = created_face;
        }
    }
}

/**
Splits an edge in two.
*/
void Split(Graph *g, Edge *e) {
    Edge *created_edge = new Edge;
    Vertex *created_vertex = new Vertex;
    //Add these to the graph:
    g->edges.push_back(created_edge);
    g->vertices.push_back(created_vertex);

    //Insert created_edge into edgelists of the faces:
    std::vector<Edge *>::iterator it = std::find(e->face1->edges.begin(), e->face1->edges.end(), e);
    e->face1->edges.insert(it, created_edge);

    it = std::find(e->face2->edges.begin(), e->face2->edges.end(), e);
    e->face2->edges.insert(it + 1, created_edge);

    //Insert created_vertex into verticeslists of the faces:
    std::vector<Vertex *>::iterator iter = std::find(e->face1->vertices.begin(), e->face1->vertices.end(), e->toVertex);
    e->face1->vertices.insert(iter + 1, created_vertex);

    iter = std::find(e->face2->vertices.begin(), e->face2->vertices.end(), e->fromVertex);
    e->face2->vertices.insert(iter + 1, created_vertex);

    //Update created_edge (has the same direction as e, goes from middle of the former edge e to the e->toVertex)
    created_edge->face1 = e->face1;
    created_edge->face2 = e->face2;
    created_edge->fromVertex = created_vertex;
    created_edge->toVertex = e->toVertex;

    //Update e->to.Vertex
    it = std::find(e->toVertex->edges.begin(), e->toVertex->edges.end(),
                   e); //in toVertex edges there is still the edge e, isnt it? I think we should pop it out
    *(it) = created_edge;                            //M: that's exactly what this does (changes e to created_edge)..
    // Jo, než překopírovat část toho seznamu, tak se prostě najde původní a nahradí novou inplace.

    //Update e
    e->toVertex = created_vertex;

    //Update created_vertex
    created_vertex->faces.push_back(e->face1);
    created_vertex->faces.push_back(e->face2);
    created_vertex->edges.push_back(e);
    created_vertex->edges.push_back(created_edge);
}

Graph *get_K3(bool coloring = false) {
    Graph *G = new Graph;
    Vertex *v1 = new Vertex;
    Vertex *v2 = new Vertex;
    Vertex *v3 = new Vertex;
    Edge *e1 = new Edge;
    Edge *e2 = new Edge;
    Edge *e3 = new Edge;
    Face *f1 = new Face;
    Face *f2 = new Face;

    // Builds K_3
    v1->faces.push_back(f1);
    v1->faces.push_back(f2);
    v2->faces.push_back(f1);
    v2->faces.push_back(f2);
    v3->faces.push_back(f1);
    v3->faces.push_back(f2);

    v1->edges.push_back(e1);
    v1->edges.push_back(e3);
    v2->edges.push_back(e1);
    v2->edges.push_back(e2);
    v3->edges.push_back(e2);
    v3->edges.push_back(e3);

    e1->fromVertex = v1;
    e2->fromVertex = v2;
    e3->fromVertex = v3;
    e1->toVertex = v2;
    e2->toVertex = v3;
    e3->toVertex = v1;
    e1->face1 = f1;
    e2->face1 = f1;
    e3->face1 = f1;
    e1->face2 = f2;
    e2->face2 = f2;
    e3->face2 = f2;

    f1->edges.push_back(e2);
    f1->edges.push_back(e1);
    f1->edges.push_back(e3);
    f2->edges.push_back(e1);
    f2->edges.push_back(e2);
    f2->edges.push_back(e3);

    f1->vertices.push_back(v3);
    f1->vertices.push_back(v2);
    f1->vertices.push_back(v1);
    f2->vertices.push_back(v1);
    f2->vertices.push_back(v2);
    f2->vertices.push_back(v3);

    G->edges.push_back(e1);
    G->edges.push_back(e2);
    G->edges.push_back(e3);

    G->faces.push_back(f1);
    G->faces.push_back(f2);

    G->vertices.push_back(v1);
    G->vertices.push_back(v2);
    G->vertices.push_back(v3);

    if (coloring) {
        v1->color = 0;
        v2->color = 1;
        v3->color = 2;
    }

    return G;
}

/**
Creates a random 2-connected graph by beginning with K_3 and splitting edges/connecting vertices.
If coloring = true guarantees existing 3-coloring of created graph
*/
Graph *BuildGraph(int steps, bool coloring = false, int percent_split = 66) {
    Graph * G = get_K3(coloring);

    std::vector < Face * > divisible_faces;
    int c1, c2;

    // Expand the Graph in steps:
    for (int i = 0; i < steps; i++) {
        badcoloring: //goto label

        if (divisible_faces.size() > 0 && std::rand() % 100 > percent_split) //Probability of 2/3 of Join face if possible
        {
            //Randomly choose face:
            int index = std::rand() % divisible_faces.size();
            Face *face = divisible_faces[index];

            //Randomly choose two nonadjacent vertices:
            int index_one = std::rand() % face->edges.size();
            int index_two = (index_one + 2 + std::rand() % (face->edges.size() - 3)) % face->edges.size();

            //if two chosen vertices have same color and coloring = true, try again
            if (coloring) {
                c1 = face->vertices[index_one]->color;
                c2 = face->vertices[index_two]->color;
                if (c1 == c2) {
                    goto badcoloring;
                }
            }
            //Add edge connecting these vertices:
            Join(G, face, face->vertices[index_one], face->vertices[index_two]);
            //Remove changed face from divisible_faces if necessary:
            if (divisible_faces[index]->edges.size() < 4) {
                divisible_faces.erase(divisible_faces.begin() + index);
            }
            //Add newly created face to divisible_faces if possible:
            for (unsigned i = 0; i < face->edges.size(); i++) {
                Edge *e = face->edges[i];
                if (e->face1 == face
                    && e->face2 == G->faces[G->faces.size() - 1]) {
                    if (e->face2->edges.size() > 3) {
                        divisible_faces.push_back(e->face2);
                    }
                    break;
                }
            }
        } else {
            // Randomly choose edge and split it:
            Edge *e = G->edges[rand() % G->edges.size()];
            c1 = e->toVertex->color;
            c2 = e->fromVertex->color;
            Split(G, e);

            //Color newly added vertex
            if (coloring) {
                if (c1 == c2) {
                    G->vertices.back()->color = (c1 + 1) % 3;
                } else {
                    G->vertices.back()->color = 3 - c1 - c2;
                }
            }

            // Add adjacent faces to divisible faces if not already present
            if (divisible_faces.end() == std::find(divisible_faces.begin(), divisible_faces.end(), e->face1)) {
                divisible_faces.push_back(e->face1);
            }
            if (divisible_faces.end() == std::find(divisible_faces.begin(), divisible_faces.end(), e->face2)) {
                divisible_faces.push_back(e->face2);
            }
        }
    }
    //Reset colors
    if (coloring) {
        for (Vertex *v : G->vertices) {
            v->color = -1;
        }
    }

    return G;
};

Graph *Graph::get_dual_graph() {
    Graph *dual = new Graph;

    for (unsigned i = 0; i < this->faces.size(); i++) {
        Vertex *v = new Vertex;
        v->dual_face = this->faces[i];
        this->faces[i]->dual_vertex = v;
        dual->vertices.push_back(v);
    }

    for (unsigned i = 0; i < this->vertices.size(); i++) {
        Face *f = new Face;
        f->dual_vertex = this->vertices[i];
        this->vertices[i]->dual_face = f;
        dual->faces.push_back(f);
    }

    for (unsigned i = 0; i < this->edges.size(); i++) {
        Edge *e = new Edge;
        e->dual_edge = this->edges[i];
        e->face1 = this->edges[i]->fromVertex->dual_face;
        e->face2 = this->edges[i]->toVertex->dual_face;
        e->fromVertex = this->edges[i]->face2->dual_vertex;
        e->toVertex = this->edges[i]->face1->dual_vertex;
        this->edges[i]->dual_edge = e;
        dual->edges.push_back(e);
    }

    for (unsigned i = 0; i < dual->vertices.size(); i++) {
        Vertex *v = dual->vertices[i];
        for (unsigned j = 0; j < v->dual_face->vertices.size(); j++) {
            v->faces.push_back(v->dual_face->vertices[j]->dual_face);
        }
        for (unsigned j = 0; j < v->dual_face->edges.size(); j++) {
            v->edges.push_back(v->dual_face->edges[j]->dual_edge);
        }
    }

    for (unsigned i = 0; i < dual->faces.size(); i++) {
        // Order of edges and vertices is not maintained here!!!
        Face *f = dual->faces[i];
        for (unsigned j = 0; j < f->dual_vertex->edges.size(); j++) {
            f->edges.push_back(f->dual_vertex->edges[j]->dual_edge);
        }
        for (unsigned j = 0; j < f->dual_vertex->faces.size(); j++) {
            f->vertices.push_back(f->dual_vertex->faces[j]->dual_vertex);
        }
    }
    dual_graph = dual; //store dual graph
    dual->dual_graph = this; //store this graph as dual of dual
    return dual;
}

void Graph::euler_path() {
    for (Vertex *v: vertices) v->iterating_index = 0;
    for (Vertex *v: vertices) flow_dfs(v);
}

/**
DFS for connecting edges without flow into closed trails.
 */
void Graph::flow_dfs(Vertex *v) {
    for (; v->iterating_index < v->edges.size(); v->iterating_index++) {
        if (v->edges[v->iterating_index]->flow) continue;
        Vertex *next = (v->edges[v->iterating_index]->fromVertex == v) ?
                       v->edges[v->iterating_index]->toVertex : v->edges[v->iterating_index]->fromVertex;
        v->edges[v->iterating_index]->flow = (v->edges[v->iterating_index]->fromVertex == v) ? 1 : -1;
        while (next != v) { // Keep setting flows until the trail is closed
            for (; next->iterating_index < next->edges.size(); next->iterating_index++) {
                if (next->edges[next->iterating_index]->flow == 0) {
                    next->edges[next->iterating_index]->flow =
                            (next->edges[next->iterating_index]->fromVertex == next) ? 1 : -1;
                    next = (next->edges[next->iterating_index]->fromVertex == next) ?
                           next->edges[next->iterating_index]->toVertex :
                           next->edges[next->iterating_index]->fromVertex;
                    break;
                }
            }
        }
    }
}

Graph::~Graph() {
    for (auto &e : this->edges) {
        delete e;
    }

    for (auto &v : this->vertices) {
        delete v;
    }

    for (auto &f : this->faces) {
        delete f;
    }
}

bool Graph::is_ok(Vertex *v, int c) { // checking if this vertex can be coloured with the colour c
    Vertex *_help;
    for (Edge *e: v->edges) {
        if (v == e->fromVertex) { //check which vertex is "from" and which is "to"
            _help = e->toVertex;
        } else {
            _help = e->fromVertex;
        }
        if (_help->color == c) {
            return false;
        }
    }
    return true;
}

bool Graph::graph_coloring(unsigned vertex_position) { //which vertex we need from the vector vertices of Graph
    if (vertex_position == (this->vertices.size())) {
        return true;
    }
    for (int c = 0; c < 3; c++) { //for each color
        if (is_ok(this->vertices[vertex_position], c)) { //check if it is ok to set this color
            vertices[vertex_position]->color = c; //set that colour
            if (graph_coloring(vertex_position + 1)) { //try to color next vertex
                return true;
            }
            vertices[vertex_position]->color = -1;     //set back to the out of range number
        }
    }
    return false;
}

bool Graph::can_be_coloured() {
    return graph_coloring(0);
}

/* dfs for setting the flows with recursion */
//void Graph::flow_dfs(Vertex* v)
//{
//  for(Edge* e: v->edges)
//  {
//      if(e->flow == 0)
//      {
//          if(v == e->fromVertex)
//          {
//              e->flow = 1; //right direction
//              flow_dfs(e->toVertex);
//          }
//          else
//          {
//              e->flow = -1; //opposite direction
//              flow_dfs(e->fromVertex);
//          }
//      }
//  }
//}

int Vertex::get_max_excess() {
    int x = this->edges.size() - this->edges.size() % 3;
    return (x - this->edges.size()) % 2 ? x - 3 : x;
}


/**
Creates a canonical triangulation of a graph with a given number of vertices.
*/
Graph *build_triangulation(unsigned vertices) {
    Graph *g = BuildGraph(0);

    unsigned splitting_index = 2;
    for (unsigned i = 3; i < vertices; i++) {
        unsigned end = g->vertices.size() - 1;
        Split(g, g->edges[splitting_index]);
        //printf("split.");
        end++;
        Join(g, g->faces[0], g->vertices[0], g->vertices[end - 1]);
        //printf("join1.");
        Join(g, g->faces[1], g->vertices[end], g->vertices[1]);
        //printf("join2.");
        splitting_index = end * 3 - 6;
        //printf("%d", splitting_index);
    }

    return g;
}

/**
Swaps an edge in a triangulation.
*/
bool make_swap_along_edge(Graph *g, Edge *e) {
    //printf("swapping...");
    //g->print_graph();
    //printf("edge: %d->%d", e->fromVertex->id, e->toVertex->id);

    Vertex *v_in_F1;
    Vertex *v_in_F2;
    for (Vertex *v : e->face1->vertices) {
        if (v != e->fromVertex && v != e->toVertex) {
            v_in_F1 = v;
        }
    }
    for (Vertex *v : e->face2->vertices) {
        if (v != e->fromVertex && v != e->toVertex) {
            v_in_F2 = v;
        }
    }

    for(Edge *e : v_in_F1->edges){ //check if this swap doesnt make a multigraph
        if((v_in_F1 == e->fromVertex && v_in_F2 == e->toVertex) || (v_in_F1 == e->toVertex && v_in_F2 == e->fromVertex)){
            return false;
        }
    }
    //rotating left : IMPORTANT

    auto it = std::find(e->fromVertex->faces.begin(), e->fromVertex->faces.end(), e->face2);
    e->fromVertex->faces.erase(it);
    it = std::find(e->toVertex->faces.begin(), e->toVertex->faces.end(), e->face1);
    e->toVertex->faces.erase(it);
    v_in_F1->faces.push_back(e->face2);
    v_in_F2->faces.push_back(e->face1);
    v_in_F1->edges.push_back(e);
    v_in_F2->edges.push_back(e);
    auto edge_it = std::find(e->fromVertex->edges.begin(), e->fromVertex->edges.end(), e);
    e->fromVertex->edges.erase(edge_it);
    edge_it = std::find(e->toVertex->edges.begin(), e->toVertex->edges.end(), e);
    e->toVertex->edges.erase(edge_it);

    //erased from the former two points
    for (unsigned i = 0; i < e->face1->vertices.size(); i++) {
        if (e->face1->vertices[i] == e->toVertex) {
            e->face1->vertices[i] = v_in_F2;
        }
    }
    for (unsigned i = 0; i < e->face2->vertices.size(); i++) {
        if (e->face2->vertices[i] == e->fromVertex) {
            e->face2->vertices[i] = v_in_F1;
        }
    }

    int edge_to_swap;
    for (unsigned i = 0; i < e->face1->edges.size(); i++) {
        auto edge = e->face1->edges[i];
        if (edge != e) {
            if (edge->toVertex == e->toVertex ||
                edge->fromVertex == e->toVertex) { //the only edge is not the e and it is connected with to Vertex
                edge_to_swap = i;
                //printf("%d->%d faces-pre: %d %d\n", edge->fromVertex->id, edge->toVertex->id, edge->face1->id, edge->face2->id);
                if (edge->face1 == e->face1) edge->face1 = e->face2;
                else edge->face2 = e->face2;
                //printf("%d->%d faces-post: %d %d\n", edge->fromVertex->id, edge->toVertex->id, edge->face1->id, edge->face2->id);
                //break;
            }
        }
    }
    for (unsigned i = 0; i < e->face2->edges.size(); i++) {
        auto edge = e->face2->edges[i];
        if (edge != e) {
            if (edge->fromVertex == e->fromVertex || edge->toVertex == e->fromVertex) {
                if (edge->face1 == e->face2) edge->face1 = e->face1;
                else edge->face2 = e->face1;
                //printf("swapping %d->%d and %d->%d\t", e->face1->edges[edge_to_swap]->fromVertex->id, e->face1->edges[edge_to_swap]->toVertex->id, edge->fromVertex->id, edge->toVertex->id);
                std::swap(e->face1->edges[edge_to_swap], e->face2->edges[i]);
            }
        }
    }
    e->fromVertex = v_in_F2;
    e->toVertex = v_in_F1;
    //printf("swapped\n");
    return true;
}

/**
Randomly swaps edges in given triangulation.
*/
void modify_triangulation(Graph *g, unsigned steps) {
    unsigned size = g->edges.size();

    for (unsigned i = 0; i < steps; i++) {
        while(!make_swap_along_edge(g, g->edges[rand() % size])) {}
    }
}

/**
Deletes an edge (with one face) updating faces and vertices. Does not maintain the order along faces.
*/
void delete_edge(Graph *g, Edge *e) {

    //g->print_graph();
    // Remove references to e and face2:

    // Vertices of e are not incident with face2 anymore:
    auto it_f = std::find(e->toVertex->faces.begin(), e->toVertex->faces.end(), e->face2);
    e->toVertex->faces.erase(it_f);
    it_f = std::find(e->fromVertex->faces.begin(), e->fromVertex->faces.end(), e->face2);
    e->fromVertex->faces.erase(it_f);

    auto it_e = std::find(g->edges.begin(), g->edges.end(), e);
    g->edges.erase(it_e);
    it_e = std::find(e->toVertex->edges.begin(), e->toVertex->edges.end(), e);
    e->toVertex->edges.erase(it_e);
    it_e = std::find(e->fromVertex->edges.begin(), e->fromVertex->edges.end(), e);
    e->fromVertex->edges.erase(it_e);
    it_e = std::find(e->face1->edges.begin(), e->face1->edges.end(), e);
    e->face1->edges.erase(it_e);

    it_f = std::find(g->faces.begin(), g->faces.end(), e->face2);
    g->faces.erase(it_f);

    for (Edge *e2 : e->face2->edges) {
        if (e2 == e) continue;
        if (e2->face1 == e->face2) e2->face1 = e->face1;
        if (e2->face2 == e->face2) e2->face2 = e->face1;
        // Add to face1:
        e->face1->edges.push_back(e2);
    }

    for (Vertex *v : e->face2->vertices) {
        if (v == e->fromVertex || v == e->toVertex) continue;
        it_f = std::find(v->faces.begin(), v->faces.end(), e->face2);
        (*it_f) = e->face1;
        // Add to face1:
        e->face1->vertices.push_back(v);
    }

    delete e->face2;
    delete e;

   // g->print_graph();
}

/**
Finds edges and removes them, preserving 2-connectivity.
Abyssal running time.
*/
void erase_edges(Graph *g, unsigned amount) {
    g->label_edges();
    if (amount >= g->edges.size() - g->vertices.size()) {
        printf("Graph has %d edges, cannot delete %d", g->edges.size(), amount);
        exit(13);
    }

    for (unsigned i = 0; i < amount; i++) {
        Edge *e;

        // Remove next edge:
        while (true) {
            e = g->edges[rand() % g->edges.size()];

            // Test if removing breaks 2-connectedness
            // i.e. Faces share more than this edge
            bool fail = false;
            for (unsigned j = 0; !fail && j < e->face1->edges.size(); j++) {
                for (unsigned k = 0; !fail && k < e->face2->edges.size(); k++) {
                    if (e->face1->edges[j] == e->face2->edges[k]) {
                        fail = e->face1->edges[j] != e;
                    }
                }
            }
            if (!fail) break;
        }
        //printf("deleting %d->%d\n", e->fromVertex->id, e->toVertex->id);
        delete_edge(g, e);
    }
}

void Graph::print_graph() {
    for(Edge * e : this->edges)
    {
        printf("%d(%d)->%d(%d) faces: %d %d\n", e->fromVertex->id, e->fromVertex->color, e->toVertex->id, e->toVertex->color, e->face1->id, e->face2->id);
    }

    for(Face * f : this->faces)
    {
        printf("%d:", f->id);
        for(Vertex * v : f->vertices)
            printf("%d\t", v->id);
        for(Edge *e : f->edges)
            printf("%d->%d\t", e->fromVertex->id, e->toVertex->id);
        printf("\n");
    }
}

void Graph::label_edges() {
    int id = 0;
    for(Vertex *v: this->vertices)
    {
        v->id = id++;
    }
    id = 0;
    for(Face *f: this->faces)
    {
        f->id = --id;
    }
}
