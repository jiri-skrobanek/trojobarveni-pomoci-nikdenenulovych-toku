#include <queue>
#include <cstddef>
#include "classes.hpp"

/**
Will set flows on edges to maximize flow from source to target. Uses Dinic's algorithm.
*/
void find_maximum_flow(Graph * g, Vertex * source, Vertex * target)
{
    for(auto &e : g->edges)
    {
        e->flow = 0;
        e->layered_edge = nullptr;
    }

    std::queue<LayeredEdge *> edges;

    while(true)
    {
        /*
        printf("NEW PHASE, flows:\n");
        for(auto &e : g->edges)
        {
            printf("%d ", e->flow);
        }
        */
        for(auto &e : g->edges)
        {
            e->layered_edge = nullptr;
        }
        for(auto &v : g->vertices)
        {
            v->layered_vertex = nullptr;
        }

        LayeredNetwork ln;
        LayeredVertex * s = new LayeredVertex;
        LayeredVertex * t = new LayeredVertex;
        s->graph_vertex = source;
        t->graph_vertex = target;
        source->layered_vertex = s;
        target->layered_vertex = t;
        s->distance = 0;
        t->distance = 1e9;
        t->out_count = 1e9;

        ln.vertices.push_back(s);
        ln.vertices.push_back(t);
        //printf("found source fine...\n");

        // Add edges incident with source to queue:
        for(auto &e : source->edges)
        {
            if((e->fromVertex == source && e->flow == 1) ||
               (e->toVertex == source && e->flow == -1))
            {
                e->layered_edge = (LayeredEdge*)0x1;
                //printf("found saturated edge from source\n");
            }
            else
            {
                LayeredEdge * le = new LayeredEdge;
                le->graph_edge = e;
                le->fromVertex = s;
                e->layered_edge = le;
                edges.push(le);
                //printf("found unsaturated edge from source\n");
            }
        }

        if(!edges.size()) break;

        // Add forward edges:
        while(edges.size())
        {
            LayeredEdge * le = edges.front();
            edges.pop();
            //printf("got another edge from queue...\n");

            // Add missing vertices:
            if(le->graph_edge->fromVertex->layered_vertex == NULL)
            {
                //printf("adding missing from...\n");
                LayeredVertex * lv = le->graph_edge->fromVertex->layered_vertex = new LayeredVertex;
                lv->graph_vertex = le->graph_edge->fromVertex;
                lv->distance = le->graph_edge->toVertex->layered_vertex->distance + 1;
                ln.vertices.push_back(lv);

                for(auto &e: lv->graph_vertex->edges)
                {
                    if(e->layered_edge == NULL)
                    {
                        if((e->flow == 1 && e->fromVertex == lv->graph_vertex)
                           || (e->flow == -1 && e->toVertex == lv->graph_vertex))
                        {
                            // The capacity of this edge is exhausted.
                            // We must skip it from the other side however.
                            //printf("found saturated edge with flow %d\n", e->flow);
                            e->layered_edge = (LayeredEdge*)0x1;
                            continue;
                        }

                        //printf("found unsaturated edge with flow %d\n", e->flow);
                        LayeredEdge * new_edge = new LayeredEdge;
                        e->layered_edge = new_edge;
                        new_edge->graph_edge = e;
                        edges.push(new_edge);
                    }
                }
            }
            if(le->graph_edge->toVertex->layered_vertex == NULL)
            {
                //printf("adding missing to... ");
                LayeredVertex * lv = le->graph_edge->toVertex->layered_vertex = new LayeredVertex;
                lv->graph_vertex = le->graph_edge->toVertex;
                lv->distance = le->graph_edge->fromVertex->layered_vertex->distance + 1;
                ln.vertices.push_back(lv);

                //printf("OK\n");

                for(auto &e: lv->graph_vertex->edges)
                {
                    //printf("flow: %d\n", e->flow);
                    if(e->layered_edge == NULL)
                    {
                        if((e->flow == 1 && e->fromVertex == lv->graph_vertex)
                           || (e->flow == -1 && e->toVertex == lv->graph_vertex))
                        {
                            // The capacity of this edge is exhausted.
                            // We must skip it from the other side however.
                            //printf("found saturated edge with flow %d\n", e->flow);
                            e->layered_edge = (LayeredEdge*)0x1;
                            continue;
                        }

                        //printf("found unsaturated edge with flow %d\n", e->flow);
                        LayeredEdge * new_edge = new LayeredEdge;
                        new_edge->graph_edge = e;
                        e->layered_edge = new_edge;
                        edges.push(new_edge);
                    }
                }
            }

            // Set target distance:
            if(le->graph_edge->fromVertex == target)
            {
                if(t->distance > le->graph_edge->toVertex->layered_vertex->distance)
                {
                    t->distance = le->graph_edge->toVertex->layered_vertex->distance + 1;
                }
            }
            if(le->graph_edge->toVertex == target)
            {
                if(t->distance > le->graph_edge->fromVertex->layered_vertex->distance)
                {
                    t->distance = le->graph_edge->fromVertex->layered_vertex->distance + 1;
                }
            }

            // Add edge and set orientation:
            //printf("adding edge...");
            int dto = le->graph_edge->toVertex->layered_vertex->distance;
            int dfrom = le->graph_edge->fromVertex->layered_vertex->distance;
            if(dto == dfrom)
            {
                // This edge may not be a part of a shortest path.
                //printf("not part of shortest path\n");
                delete le;
                continue;
            }
            else if(dto > dfrom)
            {
                le->toVertex = le->graph_edge->toVertex->layered_vertex;
                le->fromVertex = le->graph_edge->fromVertex->layered_vertex;
            }
            else
            {
                le->toVertex = le->graph_edge->fromVertex->layered_vertex;
                le->fromVertex = le->graph_edge->toVertex->layered_vertex;
            }

            if(le->fromVertex == t) { delete le; continue;}

            le->toVertex->in_edges.push_back(le);
            le->fromVertex->out_edges.push_back(le);
            le->fromVertex->out_count++;
            if(le->graph_edge->flow != 0) le->capacity = 2; // The edge is being used in the other direction.
            ln.edges.push_back(le);
            //printf("added to layer %d\n", le->fromVertex->distance);
        }

        // Remove dead ends:
        std::queue<LayeredVertex *> to_remove;
        for(auto &v : ln.vertices)
        {
            if(v != t && v->out_count == 0)
            {
                to_remove.push(v);
            }
        }

        while(to_remove.size())
        {
            //printf("removing dead end\n");
            LayeredVertex * lv = to_remove.front();
            to_remove.pop();

            for(auto &e : lv->in_edges)
            {
                e->fromVertex->invalid_count++;
                e->invalid = true;
                if(e->fromVertex->out_count == e->fromVertex->invalid_count)
                {
                    to_remove.push(e->fromVertex);
                }
            }
        }
        /*
        std::vector<LayeredVertex *> new_vertices;
        for(auto &v : ln.vertices)
        {
            if(v->out_count)
            {
                new_vertices.push_back(v);
            }
        }
        ln.vertices = new_vertices;
        */

        bool improvement_made = false;

        // Find shortest paths:
        while(s->out_count)
        {
            //printf("remaining from source: %d, finding path...", s->out_edges.size());
            // We assume the flow will be 1:
            std::vector<LayeredEdge *> path;
            if(s->out_edges[s->out_count-1]->invalid) {s->out_count--; /*printf("wrong edge\t");*/ continue;}
            path.push_back(s->out_edges[--(s->out_count)]);

            while(path[path.size()-1]->toVertex != t)
            {
                //printf("extending path, count: %d\t", path[path.size()-1]->toVertex->out_count);
                LayeredVertex * current_vertex = path[path.size()-1]->toVertex;
                if(current_vertex->out_count)
                {
                    current_vertex->out_count--;
                    if(current_vertex->out_edges[current_vertex->out_count]->invalid) continue;
                    path.push_back(current_vertex->out_edges[current_vertex->out_count]);
                }
                else
                {
                    //printf("backtracking path\t");
                    path.erase(path.end()-1);
                }

                if(path.size() == 0) break;
            }

            if(path.size()) improvement_made = true;

            //printf("found path\n");
            for(auto &e : path)
            {
                //printf("increased flow\n");
                e->flow++;
                if(e->flow < e->capacity)
                {
                    // This edge is not yet saturated, set to be used again:
                    e->fromVertex->out_count++;
                    // This might happen if the initial flow was -1.
                }
            }
        }


        // Add flow from layered network.
        //printf("adding layered network...");
        for(auto &e : ln.edges)
        {
            if(e->invalid) continue;
            e->graph_edge->flow += e->flow * (e->graph_edge->fromVertex->layered_vertex == e->fromVertex ? 1 : -1);
        }
        //printf("added\n");

        // Dispose:
        for(auto &v : ln.vertices)
        {
            delete v;
        }
        for(auto &e : ln.edges)
        {
            delete e;
        }

        // No improving path was found during this phase. Terminate.
        if(!improvement_made) break;
    }
}
