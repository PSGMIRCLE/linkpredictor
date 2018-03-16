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
#include <ctime>

// Maybe include some data structures here

using namespace std;

class Compare {
public:
    bool operator()(const Node* x, const Node* y) const {
        return x->dist > y->dist;
    }
};

class Compare2 {
public:
    bool operator()(const Node* x, const Node* y) const {
        return x->degree > y->degree;
    }
};

class ActorGraph {
protected:

    // Maybe add class data structure(s) here

public:
    vector<Node*> actors;
    unordered_map<string, Node*> actorByName;
    unordered_map<string, Edge*> movies;

    vector<vector<int>> matrix;

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

    //create the full adjacency matrix
    vector<vector<int>> createMatrix() {

        //initialize with actors.size() as dimensions with 0's
        vector<vector<int>> am(actors.size(), vector<int>(actors.size(), 0));

        //for each actor in list of actors
        for(Node* actor : actors) {
            //for each of their movies
            for(Edge* edge : actor->edges) {
                //for each of the actors in those movies
                for(Node* n : edge->actors) {
                    //if we are not comparing the actor with him/herself
                    if(actor->name != n->name) {
                        //set matrix at that position to 1
                        am[actor->index][n->index] = 1;
                    }
                }
            }
        }

        return am;
    }


    //create a 2D array to represent an actor's row in the adjacency matrix
    //will be indices.size() x N dimensions
    vector<vector<int>> createMatrix(vector<int> indices) {

        //initialize with actors.size() as dimensions
        vector<vector<int>> am(indices.size(), vector<int>(actors.size()));

        //for each index
        for(int i = 0; i < indices.size(); i++) {
            //copy that row from total matrix
            am[i] = matrix[indices[i]];
        }
        return am;
    }

    //prints out a given 2D matrix (for debugging)
    void printMatrix(vector<vector<int>> matrix) {
        for(int i = 0; i < matrix.size(); i++) {
            for(int j = 0; j < matrix[0].size(); j++) {
                cout << matrix[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    } 


    //predicts top 4 actors using existing relationships
    vector<vector<Node*>> predictExisting(vector<Node*> queryActors) {

        //convert nodes to index int values
        vector<int> indices;
        for(Node* n : queryActors) {
            indices.push_back(n->index);
        }

        //create the inputsize x N matrix
        vector<vector<int>> adjMatrix = createMatrix(indices);
        vector<vector<int>> resMatrix; 

        //perform matrix multiplication
        MatrixOperations<int> m(adjMatrix, matrix);
        resMatrix = m.matrixMultiply();

        //create 2D vector to hold pairs of #of common neighbors with
        //corresponding actor
        vector<vector<pair<int, Node*>>> copy(indices.size());

        //for each index
        for(int i = 0; i < indices.size(); i++) {
            for(int j = 0; j < actors.size(); j++) {
                //not on diagonal line i = j
                if(indices[i] != j) {
                    //if actor[i] and actor[j] are a direct link (1 away)
                    if(matrix[indices[i]][j] == 1) {
                        //push to the copy the actors we care about
                        copy[i].push_back(make_pair(resMatrix[i][j], actors[j]));
                    }
                }
            }
        }

        //sort every vector for each query actor
        for(int i = 0; i < indices.size(); i++) {
            //define sort function and call it on each vector
            sort(copy[i].begin(), copy[i].end(), []
                (const pair<int, Node*> &p1, const pair<int, Node*> &p2) {
                //if number of common neighbors is same, then compare alphabetically
                if(p1.first == p2.first) {
                    return p1.second->name > p2.second->name;
                }
                //else just compare number of common neighbors
                return p1.first < p2.first;
            });
        }
 
        //new vectors to truncate til top 4
        vector<vector<Node*>> topFour(indices.size());

        //for each vector
        for(int i = 0; i < indices.size(); i++) {
            //while size is less than 4
            while(topFour[i].size() < 4){
                //when copy is not empty
                if(!copy[i].empty()){
                    if(copy[i].back().first != 0){
                        //push to top four vector
                        topFour[i].push_back(copy[i].back().second);
                    }
                    copy[i].pop_back();
                }
                else{
                    break;
                }
            }
        }

        return topFour;
    }


    vector<vector<Node*>> predictNew(vector<Node*> queryActors) {

        //change nodes to index int values
        vector<int> indices;
        for(Node* n : queryActors) {
            indices.push_back(n->index);
        }

        //create reduced matrix
        vector<vector<int>> adjMatrix = createMatrix(indices);
        vector<vector<int>> resMatrix; 

        //perform matrix multiplication
        MatrixOperations<int> m(adjMatrix, matrix);
        resMatrix = m.matrixMultiply();

        //copy vectors
        vector<vector<pair<int, Node*>>> copy(indices.size());

        //push to copies the actors we care about
        for(int i = 0; i < indices.size(); i++) {
            for(int j = 0; j < actors.size(); j++) {
                if(indices[i] != j) {
                    //if actor[i] and actor[j] are not connected by a
                    //direct link
                    if(matrix[indices[i]][j] == 0) {
                        copy[i].push_back(make_pair(resMatrix[i][j],
                            actors[j]));
                    }
                }
            }
        }

        for(int i = 0; i < indices.size(); i++) {
            //define sort function and call it on each vector
            sort(copy[i].begin(), copy[i].end(), []
                (const pair<int, Node*> &p1, const pair<int, Node*> &p2) {
                //if number of common neighbors is same, then compare alphabetically
                if(p1.first == p2.first) {
                    return p1.second->name > p2.second->name;
                }
                //else just compare number of common neighbors
                return p1.first < p2.first;
            });
        }

        //vector for top 4 actors
        vector<vector<Node*>> topFour(indices.size());

        //for each vector
        for(int i = 0; i < indices.size(); i++) {
            //push back at most 4 of the actors with highest common
            //neighbors
            while(topFour[i].size() < 4){
                if(!copy[i].empty()){
                    if(copy[i].back().first != 0){
                        topFour[i].push_back(copy[i].back().second);
                    }
                    copy[i].pop_back();
                }
                else{
                    break;
                }
            }
        }

        return topFour;
    }

    //set degrees to each actor
    void getDegrees() {
        //iterate through each actor
        for(Node* actor : actors) {
            unordered_map<string, Node*> known;

            //search for connections
            for(Edge* e : actor->edges) {
                for(Node* n : e->actors) {
                    if(known.find(n->name) == known.end() &&
                        n->name != actor->name) {
                        known[n->name] = n;        
                    }
                }
            }

            actor->degree = known.size();
        }
    }


    //determine invitees using k-core of graph
    vector<Node*> calculate(int k) {

        vector<Node*> res;
        
        //get degrees for each actor
        getDegrees();

        //push every actor into pq
        priority_queue<Node*, vector<Node*>, Compare2> pq;
        for(Node* actor : actors) {
            pq.push(actor);
        }

        //while smallest degree is less than k
        while(pq.top()->degree < k && !pq.empty()) {
            Node* curr = pq.top();
            //get row for that node
            for(int i = 0; i < matrix.size(); i++) {
                //for each related node, subtract their degree by 1
                if(matrix[curr->index][i] == 1 && actors[i]->degree > 0) {
                    actors[i]->degree--;
                }
            }
            //pop that smallest degree node
            pq.pop();
        }

        //while pq is non-empty
        while(!pq.empty()) {
            //push nodes with degree > k into res vector
            if(pq.top()->degree > k) {
                res.push_back(pq.top());
            }
            pq.pop();
        }

        //sort names alphabetically
        sort(res.begin(), res.end(), []
            (const Node* p1, const Node* p2) {
            //if number of common neighbors is same, then compare alphabetically
            return p1->name < p2->name;
        });

        return res;
    } 
  
};


#endif // ACTORGRAPH_HPP
