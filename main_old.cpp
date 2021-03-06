#include <cstdlib>
#include <algorithm>
#include <vector>
#include "classes.cpp"
#include "coloring.cpp"
#include "tester.cpp"
#include "generator.cpp"

//#define IMPOSSIBLE_SQUARES
//#define NO_TRIANGLES
//#define MODEL1
#define SQUARES

using namespace std;

int main()
{
#ifdef NO_TRIANGLES
    // Ludicrous:
    test(3, 1, 7, 515, 20, "notr_lud_flow_old.txt", &build_no_triangles_insert_ludicrous, &time_graph_flow_old);
    test(3, 1, 24, 515, 20, "notr_lud_flow_new.txt", &build_no_triangles_insert_ludicrous, &time_graph_flow_new);
    test(3, 5, 5, 515, 20, "notr_lud_brute.txt", &build_no_triangles_insert_ludicrous, &time_graph_brute);
    test(3, 5, 20, 515, 20, "notr_lud_brute_heur.txt", &build_no_triangles_insert_ludicrous, &time_graph_brute_heuristic);
    // Many:
    //test(-2, 5, 4, 516, 20, "notr_many_flow_old.txt", &build_no_triangles_insert_many, &time_graph_flow_old);
    //test(-2, 5, 12, 516, 20, "notr_many_flow_new.txt", &build_no_triangles_insert_many, &time_graph_flow_new);
    //test(-2, 5, 20, 516, 20, "notr_many_brute.txt", &build_no_triangles_insert_many, &time_graph_brute);
    //test(-2, 5, 20, 516, 20, "notr_many_brute_heur.txt", &build_no_triangles_insert_many, &time_graph_brute_heuristic);
    // Some:
    //test(-2, 5, 6, 517, 20, "notr_some_flow_old.txt", &build_no_triangles_insert_some, &time_graph_flow_old);
    //test(-2, 5, 20, 517, 20, "notr_some_flow_new.txt", &build_no_triangles_insert_some, &time_graph_flow_new);
    //test(-2, 5, 25, 517, 20, "notr_some_brute.txt", &build_no_triangles_insert_some, &time_graph_brute);
    //test(-2, 5, 25, 517, 20, "notr_some_brute_heur.txt", &build_no_triangles_insert_some, &time_graph_brute_heuristic);
    // Few:
    //test(-2, 5, 10, 518, 20, "notr_few_flow_old.txt", &build_no_triangles_insert_few, &time_graph_flow_old);
    //test(-2, 5, 20, 518, 20, "notr_few_flow_new.txt", &build_no_triangles_insert_few, &time_graph_flow_new);
    //test(-2, 5, 25, 518, 20, "notr_few_brute.txt", &build_no_triangles_insert_few, &time_graph_brute);
    //test(-2, 5, 25, 518, 20, "notr_few_brute_heur.txt", &build_no_triangles_insert_few, &time_graph_brute_heuristic);
#endif

#ifdef MODEL1
    test(-2, 5, 12, 519, 50, "reg1_brute.txt", &build_regular_model1, &time_graph_brute);
    test(-2, 5, 12, 519, 50, "reg1_brute_heur.txt", &build_regular_model1, &time_graph_brute_heuristic);
    test(-2, 5, 8, 519, 50, "reg1_old.txt", &build_regular_model1, &time_graph_flow_old);
    test(-2, 5, 10, 519, 50, "reg1_new.txt", &build_regular_model1, &time_graph_flow_new);

    test(-2, 5, 12, 519, 50, "sparse1_brute.txt", &build_sparse_model1, &time_graph_brute);
    test(-2, 5, 12, 519, 50, "sparse1_brute_heur.txt", &build_sparse_model1, &time_graph_brute_heuristic);
    test(-2, 5, 8, 519, 50, "sparse1_old.txt", &build_sparse_model1, &time_graph_flow_old);
    test(-2, 5, 10, 519, 50, "sparse1_new.txt", &build_sparse_model1, &time_graph_flow_new);
#endif // MODEL_1

#ifdef SQUARES
    // offset, step, sets, seed, amount
    //test(-2, 100, 80, 45613, 100, "squares_old.txt", &build_quads_colorable, &time_graph_flow_old);
    //test(-2, 100, 80, 45613, 100, "squares_brute.txt", &build_quads_colorable, &time_graph_brute);
    //test(-2, 100, 10, 45613, 100, "squares_huer.txt", &build_quads_colorable, &time_graph_brute_heuristic);
    test(-2, 100, 10, 45613, 100, "squares_huer.txt", &build_quads_colorable, &time_graph_brute_heuristic);
    //test(-2, 100, 5, 45613, 100, "squares_new.txt", &build_quads_colorable, &time_graph_flow_new);
#endif

#ifdef IMPOSSIBLE_SQUARES
    //test(-2, 50, 20, 753, 100, "impossible_quads_flow_new.txt", &build_quads_uncolorable, &time_graph_flow_new);
    //test(-2, 50, 20, 753, 100, "impossible_quads_flow_old.txt", &build_quads_uncolorable, &time_graph_flow_old);
    //test(-2, 5, 6, 753, 10, "impossible_quads_brute.txt", &build_quads_uncolorable, &time_graph_brute);
    test(-2, 5, 6, 753, 10, "impossible_quads_brute_heur.txt", &build_quads_uncolorable, &time_graph_brute_heuristic);
#endif // IMPOSSIBLE_SQUARES



    /* BUG:
    printf("===before swapping:===\n");
    print_graph(g);

    modify_triangulation(g, 1, 15);

    printf("===after swapping:===\n");
    print_graph(g);

    erase_edges(g, 1, 51);

    printf("===after erasing:===\n");
    print_graph(g);
    */
    return 0;
}
