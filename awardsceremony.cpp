#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "ActorGraph.hpp"
#include "Node.hpp"
#include "Edge.hpp"

using namespace std;

int main(int argc, char* argv[]) {

  //check number of arguments
  if(argc != 4) {
    cout << "Wrong number of arguments, should be 3, is " << argc - 1 << endl;
    return -1;
  }
  ActorGraph* graph = new ActorGraph();

  //build the graph
  graph->loadFromFile(argv[1], false);

  int k = atoi(argv[2]);

  //get list of actor nodes
  vector<Node*> invitees = graph->calculate(k);

  ofstream outputFile(argv[3]);

  //write header
  outputFile << "Actor" << endl;

  //write each actor name
  for(Node* n : invitees) {
    outputFile << n->name << endl;
  }

  outputFile.close();

  delete graph;

  return 0;
}
