#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include "classes.hpp"

#ifndef TESTER
#define TESTER

Graph * get_next_graph();

std::chrono::duration<double> time_graph(Graph * g);

void conduct_testing(int sample_size, int seed, Graph *(*graph_generator)(void));

#endif // TESTER
