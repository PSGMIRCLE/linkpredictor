/*
 * ActorGraph.hpp
 * Author: <Austin Moon, Eric Gunn>
 * Date:   <3/6/18>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#ifndef ACTORGRAPH_HPP
#define ACTORGRAPH_HPP

#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <utility>
#include <unordered_map>
#include "Node.hpp"
#include "Edge.hpp"
#include "MatrixMultiply.hpp"
#include <algorithm>

// Maybe include some data structures here

using namespace std;

class Compare {
public:
    bool operator()(const Node* x, const Node* y) const {
        return x->dist > y->dist;
    }
};

class ActorGraph {
protected:

    // Maybe add class data structure(s) here

public:
    vector<Node*> actors;
    unordered_map<string, Edge*> movies;

    ActorGraph(void);

    ~ActorGraph() {
        for(Node* n : actors) {
            delete n;
        }
        for(pair<string, Edge*> p : movies) {
            delete p.second;
        }
    }

    // Maybe add some more methods here

  
    /** You can modify this method definition as you wish
     *
     * Load the graph from a tab-delimited file of actor->movie relationships.
     *
     * in_filename - input filename
     * use_weighted_edges - if true, compute edge weights as 1 + (2018 - movie_year), otherwise all edge weights will be 1
     *
     * return true if file was loaded sucessfully, false otherwise
     */
    bool loadFromFile(const char* in_filename, bool use_weighted_edges);

    //performs Dijkstra's algorithm to find the shortest weighted or unweighted path
    //between two actors
    string dijkstras(Node* start, Node* end) {

        priority_queue<Node*, vector<Node*>, Compare> queue;

        //initialize stuff
        for(Node* n : actors) {
            n->dist = INT_MAX;
            n->prev = nullptr;
            n->done = false;
            n->prevEdge = nullptr;
        }

        start->dist = 0;
        //push the start node
        queue.push(start);

        Node* currNode;

        bool foundPath = false;

        //while the priority queue is not empty
        while(!queue.empty()) {
            //pop the top node
            currNode = queue.top();
            queue.pop();

            //if we have reached the end node
            if(currNode->name == end->name) {
                //break because we must have found shortest path
                foundPath = true;
                break;
            }

            //if current node has not been checked
            if(!currNode->done) {
                //change its flag
                currNode->done = true;
                //and check it/add all required edges
                for(Edge* e : currNode->edges) {
                    for(Node* n : e->actors) {
                        //calculate distance from previous node through edge
                        if(n->done) {
                            continue;
                        }
                        //calculate distance to this neighbor
                        int distance = currNode->dist + e->dist;
                        if(distance < n->dist) {
                            n->prev = currNode;
                            n->dist = distance;
                            n->prevEdge = e;
                            
                            //enqueue neighbor node
                            queue.push(n);
                        }
                    }
                }
            }
        }

        //if path not found between two nodes
        if(!foundPath) {
            //return empty line
            return "\n";
        }

        Node* traceback = end;
        //make pairs of nodes with their edges that they traveled through
        vector<pair<Edge*, Node*>> path;
        //while node has a prev node set
        while(traceback->prev != nullptr) {
            //push pairs into vector
            path.push_back(make_pair(traceback->prevEdge, traceback));
            traceback = traceback->prev;
        }

        //make a pair for the first starting actor
        path.push_back(make_pair(nullptr, traceback));

        string returnString = "";

        //proper formatting for output
        for(int i = path.size() - 1; i >= 0; i--) {
            if(i == path.size() - 1) {
                returnString += "(";
                returnString += path[i].second->name;
                returnString += ")";
                continue;
            }
            returnString += "--[";
            returnString += path[i].first->movie;
            returnString += "#@";
            returnString += to_string(path[i].first->year);
            returnString += "]-->(";
            returnString += path[i].second->name;
            returnString += ")";
        }

        return returnString;
    }

    //check if two nodes are related by at least one movie
    bool isRelated(Node* one, Node* two) {
        //for each of actor one's movies
        for(Edge* e : one->edges) {
            //for each of the actors in those movies
            for(Node* n : e->actors) {
                //if connection is found
                if(n == two) {
                    return true;
                }
            }
        }
        return false;
    }


    //create the full adjacency matrix
    vector<vector<int>> createMatrix() {
        //initialize with actors.size() as dimensions
        vector<vector<int>> am(actors.size(), vector<int>(actors.size()));

        //for each index
        for(int i = 0; i < actors.size(); i++) {
            for(int j = 0; j < actors.size(); j++) {
                //if not comparing same actor and there is a relation between
                //actors
                if(i != j && isRelated(actors[i], actors[j])) {
                    //index is 1
                    am[i][j] = 1;
                }
                else {
                    //else index is 0
                    am[i][j] = 0;
                }
            }
        }

        //DEBUG PRINT, REMOVE LATER NIGGER
        for(int i = 0; i < actors.size(); i++) {
            for(int j = 0; j < actors.size(); j++) {
                cout << " " << am[i][j];
            }
            cout << endl;
        }
        cout << endl;

        return am;
    }


    //create a 2D array to represent an actor's row in the adjacency matrix
    //will be 1 x N dimensions
    vector<vector<int>> createMatrix(int index) {

        //initialize with actors.size() as dimensions
        vector<vector<int>> am(1, vector<int>(actors.size()));

        //for each index
        for(int j = 0; j < actors.size(); j++) {
            //if not comparing same actor and there is a relation between
            //actors
            if(index != j && isRelated(actors[index], actors[j])) {
                //index is 1
                am[0][j] = 1;
            }
            else {
                //else index is 0
                am[0][j] = 0;
            }
        }

        //DEBUG PRINT, REMOVE LATER NIGGER
        for(int i = 0; i < am.size(); i++) {
            for(int j = 0; j < actors.size(); j++) {
                cout << " " << am[i][j];
            }
            cout << endl;
        }
        cout << endl;

        return am;
    }

    vector<Node*> predictExisting(Node* actor) {

        vector<vector<int>> adjMatrix = createMatrix(actor->index);
        vector<vector<int>> totalMatrix = createMatrix();
        vector<vector<int>> resMatrix;
        
        MatrixOperations<int> m(adjMatrix, totalMatrix);

        resMatrix = m.matrixMultiply();
        
        for(vector<int> v : resMatrix){
            for(int i : v){
                cout << i << " ";
            }
            cout << endl;
        }   
        
        vector<pair<int, Node*>> copy;


        for(int i = 0; i < actors.size(); i++) {
            if(actor->index != i) {
                copy.push_back(make_pair(resMatrix[0][i], actors[i]));
            }
        }   

        sort(copy.begin(), copy.end(), [] (const pair<int, Node*> &p1, const pair<int, Node*> &p2) {
        if(p1.first == p2.first) {
            return p1.second->name > p2.second->name;
        }
        return p1.first < p2.first;
        });
        
        vector<Node*> topFour;

        while(topFour.size() < 4){
            if(!copy.empty()){
                if(copy.back().first != 0){
                    topFour.push_back(copy.back().second);
                }
                copy.pop_back();
            }
            else{
                break;
            }
        }

        return topFour;
    }
  
};


#endif // ACTORGRAPH_HPP
