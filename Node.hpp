#ifndef NODE_HPP
#define NODE_HPP

#include <climits>
#include <unordered_map>
#include <vector>
#include <utility>
#include <string>
#include "Edge.hpp"

using namespace std;

class Node {

public:

  int dist = INT_MAX;
  Node* prev = nullptr;
  bool done = false;

  string name = "";
  vector<Edge*> edges;

  Edge* prevEdge = nullptr;

  int index;

  Node() {}

  Node(string actor) {
    name = actor;
  }
  
};


#endif // NODE_HPP
