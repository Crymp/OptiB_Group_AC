#include "mySpanningTree.h"
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


using namespace boost;


int get_number_of_nodes(const Graph& g) {
    graph_traits<Graph>::vertex_iterator node_iter_begin, node_iter_end;
    tie(node_iter_begin, node_iter_end) = vertices(g);
    int n = node_iter_end - node_iter_begin;
    return n;
}


std::vector<Edge> my_spanning_tree(Graph& g) {
    std::vector<Edge> spanning_tree;

    //std::cout << "Building Qeue" << std::endl;
    // Construct Priorityque (minheap) with edge weights... O(1) access though with O(log(n)) construction
    property_map<Graph, edge_weight_t>::type weight_map = get(edge_weight, g);
    std::priority_queue<std::pair<int, Edge>, std::vector<std::pair<int, Edge>>, std::greater<std::pair<int, Edge>>> Edge_weight_queue;
    graph_traits<Graph>::edge_iterator edge_it_start, edges_it_end;
    tie(edge_it_start, edges_it_end) = edges(g);
    for (auto edge_it = edge_it_start; edge_it != edges_it_end; edge_it++) {
        int weight = get(weight_map, *edge_it);
        //int weight = get(edge_weight, g, *edge_it);
        Edge_weight_queue.push(std::make_pair(weight, *edge_it));
        //std::cout << "size " << sizeof(*edge_it)<<  " vs " << sizeof(edge_it) << std::endl;
    }
    // std::cout << "Done building queue, now adding edges" << std::endl;


    size_t num_of_nodes = get_number_of_nodes(g);

    // Now add edges in increasing order of weight, rejecting them if they close a loop
    // 
    // To keep if an edge closes a loop, each node is assigned a 'colour', all nodes that are connected (ie connected components) share the same colour
    // If an edge joins two connected components, the colour of one connected components is overriden
    //
    // To keep track of what colour of a node is, and which nodes share the same colour: using two hashmaps
    // Rather than hash the boost node object itself, just using the int index of each vertex


    std::unordered_map<int, int> colour_of_node; //node->colour    returns the colour of a node
    std::unordered_map<int, std::vector<int>> nodes_with_colour; //colour->node  returns list of node that have colour
    int max_colour = -1;

    while (spanning_tree.size() < num_of_nodes - 1) {
        //std::cout << "(" << source(*edge, g) << "," << target(*edge, g) << "): " << get(edge_weight, g, *edge) << " - ";

        std::pair<int, Edge> pair_weight_edge = Edge_weight_queue.top();
        Edge edge = pair_weight_edge.second;
        Edge_weight_queue.pop();
        int node1 = source(edge, g);
        int node2 = target(edge, g);
        int colour1 = (colour_of_node.find(node1) == colour_of_node.end()) ? -2 : colour_of_node[node1];
        int colour2 = (colour_of_node.find(node2) == colour_of_node.end()) ? -2 : colour_of_node[node2];
        // -2 signifies has never been found yet

        //std::cout << get(edge_weight, g, *edge) << " - (" << node1 << ": " << colour1 << ", " << node2 << ": " << colour2 << " ) ";
        if (colour1 == colour2) {
            if (colour1 == -2) {
                // Both nodes have not been added yet
                max_colour++;
                colour_of_node[node1] = max_colour;
                colour_of_node[node2] = max_colour;
                nodes_with_colour[max_colour] = std::vector<int>{ node1, node2 };
                //std::cout << "both unseen nodes " << node1 << " & " << node2 << ", now with colour " << max_colour << std::endl;
                spanning_tree.push_back(edge);
            }
        }
        else {
            if (colour1 == -2) {
                // Give node1 the same colour as node2
                colour_of_node[node1] = colour2;
                nodes_with_colour[colour2].push_back(node1);
            }
            else if (colour2 == -2) {
                // Give node2 the same colour as node1
                colour_of_node[node2] = colour1;
                nodes_with_colour[colour1].push_back(node2);
            }
            else {
                // Pick node with lower colour (higher colour liklier to contain less nodes)
                // Make all nodes that have the colour as node2 the same colour as node1
                std::vector<int> nodes_to_repaint = nodes_with_colour[colour2];
                for (auto node = nodes_to_repaint.begin(); node != nodes_to_repaint.end(); node++) {
                    colour_of_node[*node] = colour1;
                }
                std::vector<int> nodes_with_colour1 = nodes_with_colour[colour1];
                nodes_with_colour1.insert(nodes_with_colour1.end(), nodes_to_repaint.begin(), nodes_to_repaint.end());
                nodes_with_colour[colour1] = nodes_with_colour1;

                //Remove entries that have colour 2
                nodes_with_colour.erase(colour2);
            }
            spanning_tree.push_back(edge);
        }
    }
    return spanning_tree;
};