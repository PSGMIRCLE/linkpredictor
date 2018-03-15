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
  if(argc != 5) {
    cout << "Wrong number of arguments, should be 4, is " << argc - 1 << endl;
    return -1;
  }
  ActorGraph* graph = new ActorGraph();

  //build the graph
  graph->loadFromFile(argv[1], weighted);

  //open files to read/write
  ifstream queryFile(argv[2]);

  bool have_header = false;

  
  while (queryFile) {
    string s;
    // get the next line
    if (!getline( queryFile, s )) break;

    if (!have_header) {
      // skip the header
      have_header = true;
      outputFile << "(actor)--[movie#@year]-->(actor)--..." << endl;
      continue;
    }

    istringstream ss( s );
    vector <string> record;

    while (ss) {
      string next;
      // get the next string before hitting a tab character and put it in 'next'
      if (!getline( ss, next, '\t' )) break;
      record.push_back( next );
    }
  

  return 0;
}
