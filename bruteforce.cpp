#include<bits/stdc++.h>
#include "classes.cpp" 

using namespace std;


bool Graph::is_ok(Vertex* v, int c){ // checking if this vertex can be coloured with the colour c
  Vertex* _help;
  for(Edge* e: v -> edges){
    if(v == e -> fromVertex){ //check which vertex is "from" and which is "to"
      _help = e -> toVertex;
    }
    else{
      _help = e -> fromVertex;
    }
    if(_help -> color == c){
      return false;
    }
  }
  return true;
}

bool Graph::graph_coloring(int vertex_position){ //which vertex we need from the vector vertices of Graph
  if(vertex_position == (this -> vertices.size())){
    return true;
  }
  for(int c = 0; c < 3;c++){ //for each color
    if(is_ok(this -> vertices[vertex_position], c) ){ //check if it is ok to set this color
      vertices[vertex_position] -> color = c; //set that colour
      if(graph_coloring(vertex_position + 1) ){ //try to color next vertex
	  return true;
      }
      vertices[vertex_position] -> color = -1;	 //set back to the out of range number
    }
  }
  return false;
}

bool Graph::can_be_coloured(){
  return graph_coloring(0);
}

void Graph::print_solution(){
  if(graph_coloring(0)){
    printf("There is a solution:");
    for(Vertex* v: vertices){
      printf("%d", v -> color);
    }
    cout << endl;
  }
  else{
    printf("There is no solution");
  }
}

int main(){
  return 0;
}
