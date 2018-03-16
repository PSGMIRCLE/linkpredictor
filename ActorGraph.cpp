/*
 * ActorGraph.cpp
 * Author: <Austin Moon, Eric Gunn>
 * Date:   <3/6/18>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */
 
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include "ActorGraph.hpp"

using namespace std;

ActorGraph::ActorGraph(void) {}

bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges) {
    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;

    string previousActor = "";

    Node* currActor;
    Edge* currMovie;
  
    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
    
        // get the next line
        if (!getline( infile, s )) break;

        if (!have_header) {
            // skip the header
            have_header = true;
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
    
        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);
    
        // we have an actor/movie relationship, now what?

        int dist;
        //if we wanted weighted edges
        if(use_weighted_edges) {
            dist = (2018 - movie_year) + 1;
        }
        //if we don't want weighted edges
        else {
            dist = 1;
        }

        //if we are on the same actor
        if(previousActor != actor_name) {
            //create the new actor node
            currActor = new Node(actor_name);

            //store index in backing array
            currActor->index = actors.size();

            actors.push_back(currActor);
            actorByName[actor_name] = currActor;
        }

        //flag if movie was found
        bool found = false;

        //search in list of movies
        string title = movie_title + to_string(movie_year);
        //if movie exists
        if(movies.find(title) != movies.end()) {
            //go to that movie
            currMovie = movies[title];
            found = true;
        }

        //if it is a new movie
        if(!found) {
            //make the new movie edge
            currMovie = new Edge(movie_title, movie_year, dist);
            movies[title] = currMovie;
        }

        //add currMovie to currActor's list of movies
        currActor->edges.push_back(currMovie);

        //add currActor to currMovie's list of actors
        currMovie->actors.push_back(currActor);

        //keep track of previous actor
        previousActor = actor_name;

    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();

    //create the NxN matrix
    matrix = createMatrix();

    return true;
}
