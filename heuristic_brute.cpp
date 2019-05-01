#include<bits/stdc++.h>

using namespace std;

typedef std::set<Vertex *> vertex_container;

bool color(vertex_container *vertices[]);

/**
Returns whether provided graph is 3-colorable. Assigns colors to vertices if colorable.
 */
bool Graph::heuristic_graph_coloring()
{
    vertex_container *vertices [3];
    vertex_container vs1, vs2, vs3;
    vertices[0] = &vs1;
    vertices[1] = &vs2;
    vertices[2] = &vs3;
    for(Vertex * v : this->vertices)
    {
        // least significant bits 0 3 6 represent possibilities of colors.
        v->color = 1+8+64;
        vs3.insert(v);
    }
    //label_edges();
    return color(vertices);
}

/**
Tries to color the next vertex with the fewest available colors recursively.
 */
bool color(vertex_container * vertices[]) {
    Vertex * next = NULL;

    // Find next vertex to color:
    for(int i = 0; i < 3; i++)
    {
        if(!vertices[i]->empty())
        {
            next = *vertices[i]->begin();
            vertices[i]->erase(vertices[i]->begin());
            break;
        }
    }
    if (next == NULL) return true;

    int old_color = next->color;
    // Try all possible colors:
    for(int i = 1; i < 65; i = i << 3)
    {
        if(next->color &= i)
        {
            // Color restrictions made by this vertex:
            std::vector<std::pair<Vertex *, int> > rollback;
            bool fail = false;
            for(Edge * e : next->edges)
            {
                Vertex * neighbor = e->toVertex;
                if (neighbor == next) neighbor = e->fromVertex;

                // Test if some color available:
                if ( !( neighbor->color ^ (next->color & neighbor->color))) {fail = true; break;}
                // Some color is eliminated:
                if (neighbor->color & next->color)
                {
                    int avail = (neighbor->color % 7) - 1;
                    vertices[avail]->erase(neighbor);
                    vertices[avail-1]->insert(neighbor);
                    rollback.push_back(std::make_pair(neighbor, neighbor->color));
                    neighbor->color &= ~next->color;
                }
            }
            if(!fail && color(vertices)) return true;
            for(const std::pair<Vertex*, int> &v : rollback)
            {
                int avail = (v.first->color % 7) - 1;
                vertices[avail]->erase(v.first);
                vertices[(v.second % 7)-1]->insert(v.first);
                v.first->color = v.second;
            }
        }
        next->color = old_color;
    }
    vertices[(old_color % 7)-1]->insert(next);
    return false;
}
