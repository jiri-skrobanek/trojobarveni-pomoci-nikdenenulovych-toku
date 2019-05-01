#include "dinitz_initiator.cpp"


using namespace std;

typedef bool flow_function(Graph *);

bool Graph::from_flow_to_color(flow_function flowf) {
    // Get flow in dual:
    if (!flowf(dual_graph)) return false;
    dual_graph->euler_path();

    // Set coloring:
    std::unordered_set < Vertex * > visited;
    Edge *starting_edge = vertices[0]->edges[0];
    Vertex *starting_vertex = starting_edge->fromVertex;
    std::vector <std::pair<Vertex *, Edge *>> stack;
    stack.push_back(std::make_pair(starting_vertex, starting_edge));
    starting_vertex->color = 0;
    while (!stack.empty()) {
        Vertex *current_node = stack.back().first;
        Edge *current_edge = stack.back().second;
        stack.pop_back();
        if (visited.count(current_node)) continue;
        visited.insert(current_node);
        if (current_edge->dual_edge == NULL) {}
        else if (current_edge->toVertex == current_node) {
            current_node->color = (current_edge->fromVertex->color + current_edge->dual_edge->flow + 3) % 3;
        } else {
            current_node->color = (current_edge->fromVertex->color - current_edge->dual_edge->flow + 3) % 3;
        }
        for (Edge *e: current_node->edges) { //push next edges
            if (current_node == e->fromVertex && !visited.count(e->toVertex)) {
                stack.push_back(std::make_pair(e->toVertex, e));
            } else if (current_node == e->toVertex && !visited.count(e->fromVertex)) {
                stack.push_back(std::make_pair(e->fromVertex, e));
            }
        }
    }
    return true;
}
