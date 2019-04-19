void test_dinic_one_edge()
{

    Graph * g = new Graph;
    Vertex * v1 = new Vertex;
    Vertex * v2 = new Vertex;
    Edge * e1 = new Edge;
    g->edges.push_back(e1);
    g->vertices.push_back(v1);

    v1->edges.push_back(e1);
    v2->edges.push_back(e1);

    e1->fromVertex = v2;
    e1->toVertex = v1;

    find_maximum_flow(g, v1, v2);

    printf("%d\n", e1->flow);
}

void test_dinic_basic()
{

    Graph * g = new Graph;
    Vertex * v1 = new Vertex;
    Vertex * v2 = new Vertex;
    Vertex * inter = new Vertex;
    Vertex * inter2 = new Vertex;
    Vertex * away = new Vertex;
    Edge * e1 = new Edge;
    Edge * e2 = new Edge;
    Edge * e3 = new Edge;
    Edge * e4 = new Edge;
    g->edges.push_back(e1);
    g->edges.push_back(e2);
    g->edges.push_back(e3);
    g->edges.push_back(e4);
    g->vertices.push_back(v1);
    g->vertices.push_back(v2);
    g->vertices.push_back(inter);
    g->vertices.push_back(inter2);
    g->vertices.push_back(away);

    v1->edges.push_back(e1);
    v1->edges.push_back(e3);
    v2->edges.push_back(e2);
    v2->edges.push_back(e4);
    inter->edges.push_back(e1);
    inter->edges.push_back(e2);
    inter2->edges.push_back(e3);
    inter2->edges.push_back(e4);

    e1->fromVertex = v1;
    e1->toVertex = inter;
    e2->fromVertex = inter;
    e2->toVertex = v2;

    e3->fromVertex = v1;
    e3->toVertex = inter2;
    e4->fromVertex = inter2;
    e4->toVertex = v2;

    find_maximum_flow(g, v1, v2);

    printf("%d\n", e1->flow);
    printf("%d\n", e3->flow);
}

void test_dinic_more_phases()
{

    Graph * g = new Graph;
    Vertex * source = new Vertex;
    Vertex * target = new Vertex;
    Vertex * h1 = new Vertex;
    Vertex * h2 = new Vertex;
    Vertex * l1 = new Vertex;
    Vertex * l2 = new Vertex;
    Vertex * i1 = new Vertex;
    Vertex * i2 = new Vertex;
    Edge * d1 = new Edge;
    d1->fromVertex = source;
    d1->toVertex = h1;
    Edge * d2 = new Edge;
    d2->fromVertex = h1;
    d2->toVertex = h2;
    Edge * d3 = new Edge;
    d3->fromVertex = h2;
    d3->toVertex = i2;
    Edge * r1 = new Edge;
    r1->fromVertex = source;
    r1->toVertex = i1;
    Edge * r2 = new Edge;
    r2->fromVertex = i1;
    r2->toVertex = i2;
    Edge * r3 = new Edge;
    r3->fromVertex = i2;
    r3->toVertex = target;
    Edge * q1 = new Edge;
    q1->fromVertex = i1;
    q1->toVertex = l1;
    Edge * q2 = new Edge;
    q2->fromVertex = l1;
    q2->toVertex = l2;
    Edge * q3 = new Edge;
    q3->fromVertex = l2;
    q3->toVertex = target;
    g->edges.push_back(d1);
    g->edges.push_back(d2);
    g->edges.push_back(d3);
    g->edges.push_back(r1);
    g->edges.push_back(r2);
    g->edges.push_back(r3);
    g->edges.push_back(q1);
    g->edges.push_back(q2);
    g->edges.push_back(q3);
    g->vertices.push_back(h1);
    g->vertices.push_back(h2);
    g->vertices.push_back(i1);
    g->vertices.push_back(i2);
    g->vertices.push_back(l1);
    g->vertices.push_back(l2);
    g->vertices.push_back(source);
    g->vertices.push_back(target);

    source->edges.push_back(d1);
    source->edges.push_back(r1);

    target->edges.push_back(r3);
    target->edges.push_back(q3);

    h1->edges.push_back(d1);
    h1->edges.push_back(d2);

    h2->edges.push_back(d2);
    h2->edges.push_back(d3);

    l1->edges.push_back(q1);
    l1->edges.push_back(q2);

    l2->edges.push_back(q2);
    l2->edges.push_back(q3);

    i1->edges.push_back(r1);
    i1->edges.push_back(r2);
    i1->edges.push_back(q1);

    i2->edges.push_back(r2);
    i2->edges.push_back(r3);
    i2->edges.push_back(d3);

    find_maximum_flow(g, source, target);

    printf("r2->flow: %d\n", r2->flow);
}

void test_dinic_long_path(int length)
{
    Graph * g = new Graph;
    Vertex * v1 = new Vertex;
    g->vertices.push_back(v1);

    for(int i = 0; i < length; i++)
    {

        Vertex * v2 = new Vertex;
        Edge * e1 = new Edge;

        e1->fromVertex = g->vertices[i];
        e1->toVertex = v2;

        g->vertices[i]->edges.push_back(e1);
        v2->edges.push_back(e1);

        g->edges.push_back(e1);
        g->vertices.push_back(v2);
    }

    find_maximum_flow(g, v1, g->vertices[length]);

    printf("%d\n", g->edges[0]->flow);
}
