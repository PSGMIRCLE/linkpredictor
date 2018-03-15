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
    cout << "Wrong number of arguments, should be 4, is " << argc << endl;
    return -1;
  }
  ActorGraph* graph = new ActorGraph();

  bool weighted;
  if((*(argv[2])) == 'u') {
    weighted = false;
  }
  else if((*(argv[2])) == 'w') {
    weighted = true;
  }
  //wrong input for flag
  else {
    cout << "Wrong flag, choose u or w." << endl;
    return -1;
  }

  //build the graph
  graph->loadFromFile(argv[1], weighted);

  //open files to read/write
  ifstream queryFile(argv[3]);
  ofstream outputFile(argv[4]);

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

    if (record.size() != 2) {
      // we should have exactly 2 columns
      continue;
    }

    Node* actor1;
    Node* actor2;
    bool found1, found2 = false;

    //get the nodes with those actor names
    for(Node* n : graph->actors) {
      if(n->name == record[0]) {
        actor1 = n;
        found1 = true;
      }
      if(n->name == record[1]) {
        actor2 = n;
        found2 = true;
      }
    }

    //if one of the actors doesn't exist in graph
    if(!found1 || !found2) {
      //write a blank line
      outputFile << endl;
    }
    //else if both actors are in graph
    else {
      //run dijkstra's algorithm and find shortest path and write it
      outputFile << graph->dijkstras(actor1, actor2) << endl;
    }
  }

  if (!queryFile.eof()) {
    cerr << "Failed to read " << argv[3] << "!\n";
    return 0;
  }

  queryFile.close();
  outputFile.close();

  delete graph;
  
  return 0;
}
