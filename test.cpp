#include <vector>
#include <string>
#include <iostream>
#include "ActorGraph.hpp"
#include "Node.hpp"
#include "Edge.hpp"

using namespace std;

int main(int argc, char* argv[]) {
  ActorGraph* graph = new ActorGraph();

  if(argc != 2) {
    cout << "Wrong input size for tester" << endl;
    return -1;
  }

  graph->loadFromFile(argv[1], true);

  //print list of actors in graph
  cout << endl;
  for(Node* n : graph->actors) {
    cout << n->name << endl;
  }
  cout << endl;

  cout << graph->movies.size() << endl;

  //print list of movies in graph
  for(Edge* e : graph->movies) {
    cout << e->movie << ", " << e->year << ", " << e->dist << endl;
  }
  cout << endl;

  //for each actor, print his associated movies
  for(Node* n : graph->actors) {
    for(Edge* e : n->edges) {
      cout << n->name << " in " << e->movie << endl;
    }
  }
  cout << endl;

  for(Edge* e : graph->movies) {
    for(Node* n : e->actors) {
      cout << e->movie << " stars " << n->name << endl;
    }
  }

  cout << endl << endl << graph->dijkstras(graph->actors[0], graph->actors[1]) << endl;

  
  return 0;
}
