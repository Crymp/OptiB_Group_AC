#include "mySteinerTree.h"
#include <unordered_map>
#include <boost/graph/graph_traits.hpp>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <algorithm>    
#include <iterator>
#include <utility>          
#include <queue>
#include <functional>
#include <vector>
#include <tuple>
#include <sstream>

using namespace boost;
#include <limits>


std::vector<Edge> my_steiner_tree(const Graph& g,
    const std::vector<Vertex>& terminals) {
    std::cout << "boop\n";

    // compute a steiner tree for g with terminals g

    // Generate a hash based O(1) lookup table to indicate wether a node v in inside Steiner set
    std::vector<Edge> steiner_tree;
    std::unordered_map<Vertex, int> inside_Steiner;
    std::unordered_map<Vertex, int> is_terminal;
    for (Vertex node : make_iterator_range(terminals)) {
        is_terminal[node] = 1;
    }

    // List of terminal not yet in Steiner
    std::vector<Vertex> terminals_outside(terminals);

    // Add an arbitrary node to steiner set
    Vertex seed = terminals_outside.back();
    inside_Steiner[seed] = 1;
    terminals_outside.pop_back();

    int counter = 1;
    while (terminals.size() != counter) {
        std::vector<Vertex> path2terminal = find_SP_to_nearest_terminal(g, inside_Steiner, is_terminal);
        for (Vertex node : make_iterator_range(path2terminal)) {
            inside_Steiner[node] = 1;
        }
        // Newest terminal is the first terminal in path;
        Vertex finalvertex = path2terminal[0];
       // std::cout << finalvertex << " is terminal?: " << (is_terminal.find(finalvertex) != is_terminal.end()) << "\n";
        counter++;
        std::cout << counter << "\n";
    }

    // Now construct edge from list of terminals;


    for (auto it : inside_Steiner) {
        //        Vertex node1 = it.first;
        Vertex node1 = it.first;
        auto incident_edges = out_edges(node1, g);
        for (auto edge : make_iterator_range(incident_edges)) {
            Vertex node2 = target(edge, g);
            
            // If node2 is inside Steiner set, but has not yet been marked
            if ((inside_Steiner.find(node2) != inside_Steiner.end()) && (inside_Steiner[node2] != 2)) {
                steiner_tree.push_back(edge);
            }
        }
        inside_Steiner[node1] = 2;
    }

//    std::unordered_map<std::tuple<int, int>, int> steiner_edges;
//    for (auto it : inside_Steiner) {
//        Vertex node1 = it.first;
//
//        auto incident_edges = out_edges(node1, g);
//        for (auto edge : make_iterator_range(incident_edges)) {
//            int idx1 = source(edge, g);
//            int idx2 = target(edge, g);
//            if (idx2 < idx1) {
//                std::swap(idx1, idx2);
//            }
//            std::pair<int, int> edge_pair {idx1, idx2};
//           // if (steiner_edges.find(edge_pair) == steiner_edges.end()) {
//            //    steiner_edges[edge_pair] = 1;
////steiner_tree.push_back(edge.copy());
//     //       }
//
//        }
//    }


   // std::cout << inside_Steiner.size() << " vs " << terminals.size() << std::endl;
        return steiner_tree;
    
}



    std::vector<Vertex> find_SP_to_nearest_terminal(const Graph & g, const std::unordered_map<Vertex, int>inside_Steiner, const std::unordered_map<Vertex, int>is_terminal) {
        // compute a shortest path between startVertex and endVertex and save it in
        // the vector >>path<<

        // Based on Dijsktra's algorithm 

        //


        //std::cout << "*******************************************\n";
        //std::cout << "is_terminal: ";  for (auto i : is_terminal) { std::cout << i.first << ", "; }; std::cout << std::endl;
        //std::cout << "inside_Steiner: ";  for (auto i : inside_Steiner) { std::cout << i.first << ", "; }; std::cout << std::endl;

        std::unordered_map<Vertex, Nodeinfo> node_info_map; //v -> tuple {dist(v), pred(v), counter(v)}

        std::priority_queue<Nodeinfo, std::vector<Nodeinfo>, std::greater<Nodeinfo>> nodes_queue;
        // Note we used tuples, instead of structs, because tuples already have build in comparison ==,<=,<, operators by the first element

        // Build queue, and hash map (ie set distance values, and add all nodes to V')
        for (auto node : make_iterator_range(vertices(g))) {
            if (inside_Steiner.find(node) == inside_Steiner.end()) {
                // Nodes not in steiner
                int dist = std::numeric_limits<int>::max(); //Set distance to "infinity"
                int counter = 0;   // Mark node v in V', ie "yet to be scanned"
                Vertex pred = NULL;
                nodes_queue.push(Nodeinfo{ dist, node, counter });
                node_info_map[node] = Nodeinfo{ dist, pred, counter };
            }
            else {
                // Nodes that are Steiner set (initialize them as if they were root).
                nodes_queue.push(Nodeinfo{ 0, node, 0 });
                node_info_map[node] = Nodeinfo{ 0, node, 0 };
            }
        }
        Vertex endVertex;


        bool testflag = false;
        // Build (full) shortest path trree
        while (nodes_queue.size() != 0) {

            // Find the node with the lowest dist() in V'.
            int dist1; // ie node v in alg
            Vertex node1;
            int counter1;
            std::tie(dist1, node1, counter1) = nodes_queue.top();
            nodes_queue.pop();
            //  Keep popping until first tuple in queue is no longer outdated
            while ((nodes_queue.size() != 0) && (counter1 != std::get<2>(node_info_map[node1]))) {
                std::tie(dist1, node1, counter1) = nodes_queue.top();
                nodes_queue.pop();
            }

            // Iterate through all incident edges e = (v, w)
            auto incident_edges = out_edges(node1, g);
            for (auto const edge : make_iterator_range(incident_edges)) {

                int edge_cost = get(edge_weight, g, edge);

                // Get information about neighbouring node w 
                Vertex node2 = target(edge, g);
                int dist2;
                Vertex node2_pred;
                int counter2;
                std::tie(dist2, node2_pred, counter2) = node_info_map[node2];

                // If w in V' (ie unscanned), update distance value if neccessary
                if ((counter2 != -1) && (dist1 + edge_cost < dist2)) {
                    int dist2 = dist1 + edge_cost;
                    counter2++;
                    // Update tuple in hash_map, and push new updated tuple into queue
                    node_info_map[node2] = Nodeinfo{ dist2, node1, counter2 };
                    nodes_queue.push(Nodeinfo{ dist2, node2, counter2 });
                }
            }
            // Mark node1 as seen (ie remove to "remove v from V'  " in alg)
            get<2>(node_info_map[node1]) = -1;

            // If node1 is a terminal node, that is not in steiner path already: then stop
            if ((is_terminal.find(node1) != is_terminal.end()) && (inside_Steiner.find(node1) == inside_Steiner.end())) {
                endVertex = node1;
               // std::cout << "found node: " << node1 << "\n";
                testflag = true;
                break;
            }

        }

        //if (testflag == false) { std::cout << "Did not find any node\n"; }
        // Construct path from endVertex (ie the closest node in Steiner set) to the root
        // Do not include endVertex itself!

        std::vector<Vertex> path;
        Vertex pred = endVertex;
        path.push_back(pred);
        // Loop until pred is inside Steiner
        while (inside_Steiner.find(pred) == inside_Steiner.end()) {
            pred = std::get<1>(node_info_map[pred]);
            path.push_back(pred);
        }
        // Return tuple of distance to nearest steiner, and the path towards it.

        return path;
        //return std::tuple<int, std::vector<Vertex>>{get<0>(node_info_map[endVertex]), path
    };




    /*
    std::tuple<int, std::vector<Vertex>> find_SP_to_a_Steiner(const Graph& g, const Vertex& startVertex, const std::unordered_map<Vertex, int>& inside_Steiner) {
        // compute a shortest path between startVertex and endVertex and save it in
        // the vector >>path<<

        // Based on Dijsktra's algorithm 

        //


        // Some preamable on how did this implemenation.
        // Using Dijsktra's algorithm to build the shortest spanning tree with root startVertex, until endVertex is found in the tree.
        // 
        // This implementation follows the algorimth as given in the notes.
        // To keep track of "dist(v)" and "pred(v)" of each node v (and wether v is in V', the set of "not yet scanned vertices") we use a unordered (has) map that has constant O(1) acccess time.
        std::unordered_map<Vertex, Nodeinfo> node_info_map; //v -> tuple {dist(v), pred(v), counter(v)}
        // Nodeinfo is a custom tuple typedef tuple<int, Vertex, int>. 
        // Counter(v)=-1 indicates that v no longer in V' (ie it has been "scanned"), counter(v)>=0 indicates v in V'.

        // The algorithm also requires to find the node v that has the shortest dist(v) in V'.
        // Naturally a priority_queue (min heap) comes in mind since it has O(1) access time (at the expense of O(nlog(n)) to build it).
        // However it is required that some elements dist(v) values are modified. The solution is to allow duplicate entries, ie modifying an element means pushing a duplicate with the new value into the queue.
        // When old/outdated elements are encountered, they are simply ignored, and popped from the queue. 
        // The queue holds Nodeinfo type tuples {dist(v), v, counter(v)} for every v in V'.
        // We keep track of wether a tuple in the queue is the "newest" through the counter variable. Evertime the tuple of node v is updated, the counter(v) is increased by +1 in the node_info_map (until it is permantly set to -1, once v is "scanned").
        // Therefore if a tuple with node v but a counter value not equal to the value in the unordered map, it means that that tuple is outdated (or v has been removed from V).

        std::priority_queue<Nodeinfo, std::vector<Nodeinfo>, std::greater<Nodeinfo>> nodes_queue;
        // Note we used tuples, instead of structs, because tuples already have build in comparison ==,<=,<, operators by the first element


        // Build queue, and hash map (ie set distance values, and add all nodes to V')
        for (auto node : make_iterator_range(vertices(g))) {
            if (node != startVertex) {
                int dist = std::numeric_limits<int>::max(); //Set distance to "infinity"
                int counter = 0;   // Mark node v in V', ie "yet to be scanned"
                Vertex pred = NULL;
                nodes_queue.push(Nodeinfo{ dist, node, counter });
                node_info_map[node] = Nodeinfo{ dist, pred, counter };
            }
            else {
                // Root vertex s has special values dist(s)=0, pred(s)=s (but it is not yet scanned)
                nodes_queue.push(Nodeinfo{ 0, node, 0 });
                node_info_map[node] = Nodeinfo{ 0, node, 0 };
            }
        }
        Vertex endVertex;

        // Build (full) shortest path trree
        while (nodes_queue.size() != 0) {

            // Find the node with the lowest dist() in V'.
            int dist1; // ie node v in alg
            Vertex node1;
            int counter1;
            std::tie(dist1, node1, counter1) = nodes_queue.top();
            nodes_queue.pop();
            //  Keep popping until first tuple in queue is no longer outdated
            while ((nodes_queue.size() != 0) && (counter1 != std::get<2>(node_info_map[node1]))) {
                std::tie(dist1, node1, counter1) = nodes_queue.top();
                nodes_queue.pop();
            }

            // Iterate through all incident edges e = (v, w)
            auto incident_edges = out_edges(node1, g);
            for (auto const edge : make_iterator_range(incident_edges)) {

                int edge_cost = get(edge_weight, g, edge);

                // Get information about neighbouring node w 
                Vertex node2 = target(edge, g);
                int dist2;
                Vertex node2_pred;
                int counter2;
                std::tie(dist2, node2_pred, counter2) = node_info_map[node2];

                // If w in V' (ie unscanned), update distance value if neccessary
                if ((counter2 != -1) && (dist1 + edge_cost < dist2)) {
                    int dist2 = dist1 + edge_cost;
                    counter2++;
                    // Update tuple in hash_map, and push new updated tuple into queue
                    node_info_map[node2] = Nodeinfo{ dist2, node1, counter2 };
                    nodes_queue.push(Nodeinfo{ dist2, node2, counter2 });
                }
            }
            // Mark node1 as seen (ie remove to "remove v from V'  " in alg)
            get<2>(node_info_map[node1]) = -1;

            // If endVertex is marked as seen, then endVertex is permantly marked in shortestpathtree, ie we have found the shorted path from so to v/
            bool node1_in_Steiner = (inside_Steiner.find(node1) == inside_Steiner.end()) ? false : true;
            if (node1_in_Steiner == true) {
                endVertex = node1;
                break;
            }

        }

        // Construct path from endVertex (ie the closest node in Steiner set) to the root
        // Do not include endVertex itself!

        std::vector<Vertex> path;
        Vertex pred = endVertex;
        while (pred != startVertex) {
            pred = std::get<1>(node_info_map[pred]);
            path.push_back(pred);
        }
        // Return tuple of distance to nearest steiner, and the path towards it.
        return std::tuple<int, std::vector<Vertex>>{get<0>(node_info_map[endVertex]), path
        };
    }


    */


    //while (terminals_outside.size() != 0) {
//    std::tuple<int, std::vector<Vertex>> min_SP_terminal2Steiner{std::numeric_limits<int>::max(), NULL};
//    
//    // Find the terminal node closest to any vertex of Steiner, and it and its path to Steiner
//    //for (Vertex node : make_iterator_range(terminals_outside)) {
//    for (auto nodeit = terminals_outside.)
//    // Use Dijkstras to find the path from terminal to nearest node in Steiner
//        auto SP_terminal2Steiner = find_SP_to_a_Steiner(g, node, inside_Steiner);
//        min_SP_terminal2Steiner = std::min(SP_terminal2Steiner, min_SP_terminal2Steiner);
//    }
//    std::vector<Vertex> path = get<1>(min_SP_terminal2Steiner);
//    terminals.
//    for (auto node : make_iterator_range(path)) {
//        inside_Steiner[node] = 1;
//        
//    }


//}

