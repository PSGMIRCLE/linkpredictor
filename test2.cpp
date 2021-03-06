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

  graph->loadFromFile(argv[1], false);

/*
  for(Node* n : graph->actors) {
    cout << n->name << ", ";
  }
  cout << endl;*/

  vector<vector<Node*>> vec = graph->predictExisting({graph->actors[0], graph->actors[1], graph->actors[2]});

  cout << graph->actors[0]->name << ", " << graph->actors[1]->name << ", " << graph->actors[2]->name << endl;

  for(int i = 0; i < vec.size(); i++) {
    for(Node* n : vec[i]) {
      cout << n->name << endl;
    }
    cout << endl;
  }

  /*
  for(Node* n : vec) {
    cout << n->name << ", ";
  }
  cout << endl;*/

  return 0;
}
