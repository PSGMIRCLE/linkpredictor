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
  graph->loadFromFile(argv[1], false);

  //open files to read/write
  ifstream queryFile(argv[2]);
  ofstream outputFile1(argv[3]);
  ofstream outputFile2(argv[4]);

  bool have_header = false;

  //store names of actors from input file
  vector<string> queryNames;
  
  while (queryFile) {
    string s;
    // get the next line
    if (!getline( queryFile, s )) break;

    if (!have_header) {
      // skip the header
      have_header = true;
      //write headers
      outputFile1 << "Actor1,Actor2,Actor3,Actor4" << endl;
      outputFile2 << "Actor1,Actor2,Actor3,Actor4" << endl;
      continue;
    }

    istringstream ss( s );

    while (ss) {
      string next;
      // get the names of actors from input file
      if (!getline( ss, next)) break;
      queryNames.push_back( next );
    }
  }

  //if failed to read file
  if (!queryFile.eof()) {
  cerr << "Failed to read " << argv[2] << "!\n";
  return 0;
  }

  queryFile.close();

  //vector of actors to query
  vector<Node*> queryActors;

  //change string names to actual node pointers
  for(string s : queryNames) {
    queryActors.push_back(graph->actorByName[s]);
  }

  //predict top 4 for existing relationships
  vector<vector<Node*>> vec1 = graph->predictExisting(queryActors);

  for(int i = 0; i < vec1.size(); i++) {
    int count = 0;
    for(int j = 0; j < vec1[i].size(); j++) {
      //write the name to file
      outputFile1 << vec1[i][j]->name;
      count++;
      //if we have not put all 4 names
      if(count < 4) {
        //put a tab
        outputFile1 << "\t";
      }
    }
    //if we still don't have 4 names, add NULL
    while(count < 4) {
      outputFile1 << "NULL";
      count++;
      if(count < 4) {
        outputFile1 << "\t";
      }
    }
    outputFile1 << endl;
  }

  outputFile1.close();

  //predict top 4 for new relationships
  vector<vector<Node*>> vec2 = graph->predictNew(queryActors);

  for(int i = 0; i < vec2.size(); i++) {
    int count = 0;
    for(int j = 0; j < vec2[i].size(); j++) {
      //write the name to file
      outputFile2 << vec2[i][j]->name;
      count++;
      //if we have not put all 4 names
      if(count < 4) {
        //put a tab
        outputFile2 << "\t";
      }
    }
    //if we still don't have 4 names, add NULL
    while(count < 4) {
      outputFile2 << "NULL";
      count++;
      if(count < 4) {
        outputFile2 << "\t";
      }
    }
    outputFile2 << endl;
  }

  outputFile2.close();

  delete graph;

  return 0;
}
