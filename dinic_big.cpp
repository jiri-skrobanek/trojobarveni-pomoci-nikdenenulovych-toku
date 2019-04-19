void add_edge(Graph * g, int from, int to)
{
    Edge * e = new Edge;
    e->fromVertex = g->edges[from];
    e->toVertex = g->edges[to];
    g->edges.push_back(e);
    g->vertices[from]->edges.push_back(e);
    g->vertices[to]->edges.push_back(e);
}

void test_dinic_big()
{
    Graph * g = new Graph;

    for(int i = 0; i < 11; i++)
    {
        g->vertices.push_back(new Vertex);
    }

    add_edge(g, 1,8);
    add_edge(g, 6,5);
    add_edge(g, 1,8);
    add_edge(g, 2,7);
    add_edge(g, 2,1);
    add_edge(g, 3,0);
    add_edge(g, 6,5);
    add_edge(g, 4,2);
    add_edge(g, 5,7);
    add_edge(g, 6,5);
    add_edge(g, 1,8);
    add_edge(g, 4,2);
    add_edge(g, 6,5);
    add_edge(g, 6,4);
    add_edge(g, 3,0);
    add_edge(g, 7,0);
    add_edge(g, 8,3);
    add_edge(g, 1,8);
    add_edge(g, 9,0);
    add_edge(g, 9,0);
    add_edge(g, 9,0);
    add_edge(g, 9,1);
    add_edge(g, 9,1);
    add_edge(g, 9,1);
    add_edge(g, 9,3);
    add_edge(g, 9,3);
    add_edge(g, 9,3);
    add_edge(g, 9,4);
    add_edge(g, 9,4);
    add_edge(g, 9,4);
    add_edge(g, 5,10);
    add_edge(g, 5,10);
    add_edge(g, 5,10);
    add_edge(g, 6,10);
    add_edge(g, 6,10);
    add_edge(g, 6,10);
    add_edge(g, 7,10);
    add_edge(g, 7,10);
    add_edge(g, 7,10);
    add_edge(g, 8,10);
    add_edge(g, 8,10);
    add_edge(g, 8,10);

    find_maximum_flow(g, g->vertices[9], g->vertices[10]);
}
