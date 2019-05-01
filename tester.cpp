#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include "classes.hpp"
#include "tester.hpp"
#include "heuristic_graph_coloring.cpp"
#include "heuristic_brute.cpp"

typedef double color_assigner(Graph *, bool &);

double time_graph_flow_old(Graph * g, bool &colorable)
{
    auto start = std::chrono::system_clock::now();

    g->get_dual_graph();
    colorable = g->from_flow_to_color(&initiate_dinitz);

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> dur = (end-start);
    g->dual_graph->~Graph();
    g->dual_graph = NULL;
    //printf("GOT SOLUTION\t");
    return dur.count();
}

double time_graph_flow_new(Graph * g, bool &colorable)
{
    auto start = std::chrono::system_clock::now();

    g->get_dual_graph();
    colorable = g->from_flow_to_color(&heuristic_algorithm_the_first);

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> dur = (end-start);
    g->dual_graph->~Graph();
    g->dual_graph = NULL;
    //printf("GOT SOLUTION %f\t", dur.count());
    return dur.count();
}

double time_graph_brute(Graph * g, bool &colorable)
{
    auto start = std::chrono::system_clock::now();

    colorable = g->can_be_coloured();

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> dur = (end-start);
    //printf("GOT SOLUTION\t");

    /*for(Edge * e : g->edges)
    {
        printf("%d->%d\t", e->fromVertex->color, e->toVertex->color);
    }*/

    return dur.count();
}

double time_graph_brute_heuristic(Graph * g, bool &colorable)
{
    auto start = std::chrono::system_clock::now();

    colorable = g->heuristic_graph_coloring();

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> dur = (end-start);
    //printf("GOT SOLUTION\t");
    return dur.count();
}

/**
Returns an array of times for specified algorithm and graph types.
 */
double * conduct_testing(int sample_size, int size, int seed, Graph *(*graph_generator)(int), color_assigner fn)
{
    srand(seed);
    double * res = (double*)malloc(sizeof(double)*sample_size);
    printf("RUNNING TESTS... SIZE: %d SAMPLE SIZE:%d\n", size, sample_size);

    int colorable = 0;
    bool ok = false;

    for(int i = 0; i < sample_size; i++)
    {
        Graph * g = graph_generator(size);
        //printf("GENERATED GRAPH %d of %d\t", i+1, sample_size);

        //fprintf(result_file, "%.9f,", time_graph_flow_old(g));
        //for(auto &v : g->vertices) v->color = -1;


        res[i] = fn(g, ok);
        colorable += ok;
        //for(auto &v : g->vertices) v->color = -1;

        //fprintf(result_file, "%.9f,", time_graph_brute_heuristic(g));
        //for(auto &v : g->vertices) v->color = -1;
	    //time_graph_brute(g);

        g->~Graph();
        //printf("TESTED GRAPH\n");
    }

    printf("TESTS OK. %d COLORABLE\n", colorable);
    return res;
}

void write_result(int size, double *times, int amount, FILE *file)
{
    double avg = 0;
    for(int i = 0; i < amount; i++)
    {
        avg += times[i];
    }
    //printf("sum: %f ", avg);
    avg /= amount;
    std::sort(times, times + amount);

    printf("%d, %f, %f, %f\n", size, avg, times[amount/20], times[(19*amount)/20]);
    fprintf(file, "%d, %f, %f, %f\n", size, avg, times[amount/20], times[(19*amount)/20]);
    free(times);
}

void test(int offset, int step, int sets, int seed, int amount, char *filename, Graph *(*graph_generator)(int), color_assigner fn)
{
    FILE *file = fopen(filename, "w");
    for(int i = 1; i <= sets; i++)
    {
        double * res = conduct_testing(amount, step*i + offset, seed, graph_generator, fn);
        write_result(step*i, res, amount, file);
        seed = 7*seed + 5;
        if(0==i%5){fflush(file);}
    }
    fclose(file);
}
