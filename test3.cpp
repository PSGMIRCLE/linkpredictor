#include <vector>
#include <string>
#include <iostream>
#include "ActorGraph.hpp"
#include "Node.hpp"
#include "Edge.hpp"

using namespace std;

int main(int argc, char* argv[]) {
  ActorGraph* graph = new ActorGraph();

  if(argc != 4) {
    cout << "Wrong input size for tester" << endl;
    return -1;
  }

  graph->loadFromFile(argv[1], false);

  int k = atoi(argv[2]);

  vector<Node*> invitees = graph->calculate(k);

  for(Node* n : invitees) {
    cout << n->name << ": " << n->degree << endl;
  }

  return 0;
}
