#ifndef EDGE_HPP
#define EDGE_HPP

#include <climits>
#include <unordered_map>
#include <vector>
#include <utility>
#include <string>
#include "Node.hpp"

using namespace std;

class Node;

class Edge {

public:

  vector<Node*> actors;
  string movie;
  int year;
  int dist = 1;
  
  Edge(string name, int date, int distance) {
    movie = name;
    year = date;
    dist = distance;
  }
  
};


#endif // EDGE_HPP
