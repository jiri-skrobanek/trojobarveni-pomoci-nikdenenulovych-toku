#include <vector>
#include <algorithm>
#include <cstdio>
#include "dinic.cpp"

/**
Constructs minimal possible assignment of excesses for vertices on indices <left;right>
*/
void minimal_assignment(Graph *G, std::vector<Vertex *> &vertex_ptrs, std::vector<int> &excesses, int left, int right) {
    int m;
    while (left < right) {
        int lsize = vertex_ptrs[left]->edges.size(); //because vector.size() is unsigned
        int rsize = vertex_ptrs[right]->edges.size();
        //maximum which can be simultaneously subtracted from left and added to right
        m = std::min(excesses[left] + lsize, rsize - excesses[right]);
        m = (m / 6) * 6;
        excesses[left] -= m;
        excesses[right] += m;
        if (excesses[left] - 6 < -lsize) {
            left += 1;
        }
        if (excesses[right] + 6 > rsize) {
            right -= 1;
        }
    }
}

/**
Tries to find next_assignment of excesses and change it in excesses, if not possible returns false
*/
bool next_assignment(Graph *G, std::vector<Vertex *> &vertices_ptrs, std::vector<int> &excesses) {
    //Try to find element to increase
    int increase_element = -1;
    bool can_be_decreased = false;

    for (int i = (excesses.size() - 1); i >= 0; i--) {
        int isize = vertices_ptrs[i]->edges.size();

        if (((excesses[i] + 6) <= isize) && can_be_decreased) {
            increase_element = i;
            break;
        }
        if (excesses[i] - 6 >= -isize) {
            can_be_decreased = true;
        }
    }
    //If such element was not found, the assignment of excesses is maximal
    if (increase_element == -1) { return false; }

    //Otherwise construct next assignment
    excesses[increase_element] += 6;
    excesses[increase_element + 1] -= 6;
    minimal_assignment(G, vertices_ptrs, excesses, increase_element + 1, excesses.size() - 1);
    return true;
}

/**
Generate all possible assignments and run Dinitz algorithm with them
*/
bool initiate_dinitz(Graph *G) {
    std::vector < Vertex * > vertices_ptrs; //contains pointers to ambiguous vertices
    std::vector<int> excesses; //contains current excesses of ambiguous vertices, f.e. vertices_ptr[2] has current excess equal to excesses[2]
    int exc = 3;
    unsigned degree;
    //Find which excesses are changeable and create basic assignment
    for (unsigned i = 0; i < G->vertices.size(); i++) {
        degree = G->vertices[i]->edges.size();
        if (degree & 1) {
            //if  degree is odd, set basic as value as +-3 (alternately)
            excesses.push_back(exc);
            exc *= -1;
            vertices_ptrs.push_back(G->vertices[i]);
        } else {
            if (degree > 4) {
                excesses.push_back(0);
                vertices_ptrs.push_back(G->vertices[i]);
            }
        }
    }

    //set assignment to smallest
    minimal_assignment(G, vertices_ptrs, excesses, 0, excesses.size() - 1);
    //Add source and sink vertices
    Vertex *source = new Vertex;
    Vertex *sink = new Vertex;
    Edge *help_edge;
    bool flow_successful;

    G->vertices.push_back(source);
    G->vertices.push_back(sink);
    //Test flow for all possible assignments
    while (true) {
        for (unsigned i = 0; i < excesses.size(); i++) {
            if (excesses[i] >= 0) {
                for (int j = 0; j < excesses[i]; j++) {
                    Edge *created_edge = new Edge;
                    created_edge->fromVertex = vertices_ptrs[i];
                    created_edge->toVertex = sink;
                    sink->edges.push_back(created_edge);
                    vertices_ptrs[i]->edges.push_back(created_edge);
                    G->edges.push_back(created_edge);
                }
            } else {
                for (int j = 0; j < (-excesses[i]); j++) {
                    Edge *created_edge = new Edge;
                    created_edge->toVertex = vertices_ptrs[i];
                    created_edge->fromVertex = source;
                    source->edges.push_back(created_edge);
                    vertices_ptrs[i]->edges.push_back(created_edge);
                    G->edges.push_back(created_edge);
                }
            }
        }

        /*
        // Debug graph structure:
        for(int i = 0; i < G->vertices.size(); i++)
        {
            G->vertices[i]->id = i;
        }
        printf("source: %d, sink: %d\n", source->id, sink->id);
        for(int i = 0; i < G->edges.size(); i++)
        {
            printf("%d->%d\n", G->edges[i]->fromVertex->id, G->edges[i]->toVertex->id);
        }

        */
        find_maximum_flow(G, source, sink);
        /*
		for(int i = 0; i < G->edges.size(); i++)
        {
            printf("%d->%d: %d\n", G->edges[i]->fromVertex->id, G->edges[i]->toVertex->id, G->edges[i]->flow);
        }
        */
        flow_successful = true;
        for (Edge *e : source->edges) {
            if (e->flow == 1) continue;
            flow_successful = false;
            break;
        }
        // Remove added edges:
        while (source->edges.size()) {
            help_edge = source->edges.back();
            source->edges.pop_back();
            help_edge->toVertex->edges.pop_back();
            G->edges.pop_back();
            delete help_edge;
        }
        while (sink->edges.size()) {
            help_edge = sink->edges.back();
            sink->edges.pop_back();
            help_edge->fromVertex->edges.pop_back();
            G->edges.pop_back();
            delete help_edge;
        }

        if (flow_successful) break;

        // Find next assignment
        if (!next_assignment(G, vertices_ptrs, excesses)) break;
    }

    // Remove source and sink:
    G->vertices.pop_back();
    G->vertices.pop_back();
    delete source;
    delete sink;

    return flow_successful;
}
