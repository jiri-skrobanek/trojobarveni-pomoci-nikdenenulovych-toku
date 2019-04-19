#include<bits/stdc++.h>

using namespace std;

typedef std::set<Vertex *> vertex_container;

bool color(vertex_container &ones, vertex_container &twos, vertex_container &threes);

/**
Returns whether provided graph is 3-colorable. Assigns colors to vertices if colorable.
 */
bool Graph::heuristic_graph_coloring()
{
    vertex_container vs1, vs2, vs3;
    for(Vertex * v : this->vertices)
    {
        // least significant bits 0 2 4 represent possibilities of colors.
        v->color = 1+4+16;
        vs3.insert(v);
    }

    return color(vs1, vs2, vs3);
}

/**
Tries to color the next vertex with the fewest available colors recursively.
 */
bool color(vertex_container & ones, vertex_container & twos, vertex_container & threes) {

    //printf("Called");

    Vertex * next = NULL;

    // Find next vertex to color:
    if(!ones.empty())
    {
        //printf("cau");
        next = (*ones.begin());
        ones.erase(ones.begin());
    }
    else if(!twos.empty())
    {
        //printf("hi");
        next = (*twos.begin());
        twos.erase(twos.begin());
    }
    else if(!threes.empty())
    {
        //printf("hello");
        next = (*threes.begin());
        //printf("hello");
        threes.erase(threes.begin());
        //printf("hello");
    }
    else
    {
        return true;
    }

    //printf("HERE");

    int old_color = next->color;
    // Try all possible colors:
    for(int i = 1; i < 17; i = i << 2)
    {
        if(next->color &= i)
        {
            vertex_container ones2 (ones);
            vertex_container twos2 (twos);
            vertex_container threes2 (threes);
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
                    // Sum possible bits:
                    switch(neighbor->color % 3)
                    {
                        case 2:
                            twos.erase(neighbor);
                            ones.insert(neighbor);
                            break;
                        case 0: // All three colors were available.
                            threes.erase(neighbor);
                            twos.insert(neighbor);
                            break;
                    }
                    rollback.push_back(std::make_pair(neighbor,neighbor->color));
                    neighbor->color ^= neighbor->color & next->color;
                }
            }
            if(!fail && color(ones2, twos2, threes2)) return true;
            for(const std::pair<Vertex*, int> &v : rollback)
            {
                v.first->color = v.second;
            }
        }
        next->color = old_color;
    }

    return false;
}
