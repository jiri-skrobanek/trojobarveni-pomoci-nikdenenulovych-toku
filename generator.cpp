#include "classes.hpp"

Graph *get_without_triangles();

/**
Generates graph with faces with 4 edges
*/
Graph *BuildQuads(unsigned steps, bool assure_uncolorable) {
    Graph *G = BuildGraph(1, true);
    Face *outer_face = G->faces[0];
    int probability;
    int index_one, index_two;
    for (unsigned j = 0; j < steps; j++) {
        // Randomly choose edge to join and split
        index_one = std::rand() % outer_face->vertices.size();
        probability = std::rand() % 3;
        if (probability == 0 && outer_face->vertices.size() > 3) {
            index_two = (index_one + 3) % outer_face->vertices.size();
        } else if (probability < 2 && outer_face->vertices.size() > 2) {
            index_two = (index_one + 2) % outer_face->vertices.size();
            probability = 1;
        } else {
            index_two = (index_one + 1) % outer_face->vertices.size();
            probability = 2;
        }
        Join(G, outer_face, outer_face->vertices[index_one], outer_face->vertices[index_two]);
        // Split the newly added edge to create quadrilateral
        for (int i = 0; i < probability; i++) {
            Split(G, G->edges.back());
        }

        // Find new outer face:
        for (Face *f: G->faces) {
            if (f->vertices.size() != 4) {
                outer_face = f;
                break;
            }
        }
    }

    if (assure_uncolorable) {
        // Create uncolorable K_4
        for (Face *f: G->faces) {
            if (f->vertices.size() == 4) {
                outer_face = f;
                break;
            }
        }
        Vertex *v1 = outer_face->vertices[0];
        Vertex *v2 = outer_face->vertices[1];
        Vertex *v3 = outer_face->vertices[2];
        Vertex *v4 = outer_face->vertices[3];

        Join(G, outer_face, v1, v3);
        Edge *e = G->edges.back();
        Split(G, e);
        if (e->face1 == outer_face) {
            Join(G, G->faces.back(), v4, G->vertices.back());
        } else {
            Join(G, G->faces.back(), v2, G->vertices.back());
        }
        Join(G, outer_face, v1, v3);
    }

    //printf("Size: %lu edges, %lu faces\n", G->edges.size(), G->faces.size());
    return G;
}

Graph *build_with_no_triangles(unsigned steps, unsigned extensions) {
    Graph *g = BuildQuads(steps, false);
    //printf("qb ");
    for(unsigned i = 0; i < extensions; i++)
    {
        Face * f = g->faces[rand() % g->faces.size()];
        Edge * e = f->edges[rand() % f->edges.size()];
        Split(g, e);
    }

    //printf("%f\n", ((double)g->edges.size()) / g->vertices.size());

    return g;
}

Graph *build_model2(unsigned vertices, unsigned swaps, unsigned deletions)
{
    Graph *g = build_triangulation(vertices);
    //g->label_edges();
    modify_triangulation(g, swaps);
    erase_edges(g, deletions);
    return g;
}

Graph *build_model2_dense(int steps)
{
    return build_model2(steps, steps*10, steps/20);
}

Graph *build_model2_med(int steps)
{
    return build_model2(steps, steps*10, steps - 3);
}

Graph *build_model2_sparse(int steps)
{
    return build_model2(steps, steps*10, (3*steps)/2 - 5);
}

Graph *build_no_triangles_insert_ludicrous(int steps)
{
    return build_with_no_triangles(steps, 3*steps);
}

Graph *build_no_triangles_insert_many(int steps)
{
    return build_with_no_triangles(steps, steps/2);
}

Graph *build_no_triangles_insert_some(int steps)
{
    return build_with_no_triangles(steps, steps/5);
}

Graph *build_no_triangles_insert_few(int steps)
{
    return build_with_no_triangles(steps, steps/20);
}

Graph *build_quads_colorable(int steps)
{
    return BuildQuads(steps, false);
}

Graph *build_quads_uncolorable(int steps)
{
    return BuildQuads(steps, true);
}

Graph *build_sparse_model1(int steps)
{
   return BuildGraph(steps, false, 90);
}

Graph *build_regular_model1(int steps)
{
   return BuildGraph(steps, false, 66);
}

Graph *build_dense_model1(int steps)
{
    return BuildGraph(steps, false, 50);
}
