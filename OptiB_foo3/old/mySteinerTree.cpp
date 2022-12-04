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


std::vector<Edge> my_steiner_tree(const Graph& g, const std::vector<Vertex>& terminals) {
    // compute a steiner tree for g with terminals g
    // computes the steiner tree for which these are found

    std::vector<Edge> steiner_tree;
    // O(1) lookup tables for which nodes are terminal, and which nodes are in Steiner set
    std::unordered_map<Vertex, int> is_terminal;
    std::unordered_map<Vertex, int> inside_Steiner;
    for (Vertex node : make_iterator_range(terminals)) {
        is_terminal[node] = 1;
    }
    std::vector<Vertex> terminals_outside(terminals);

    // Add an arbitrary node to steiner set
    Vertex seed = terminals_outside.back();
    inside_Steiner[seed] = 1;
    terminals_outside.pop_back();

    // Find path to closest terminal node from Steinerset, add it, and repeat until all terminal nodes add
    int counter = 1; // number of terminal nodes in Steiner set
    while (terminals.size() != counter) {
        std::vector<Vertex> path2terminal_nodes;
        std::vector<Edge> path2terminal_edges;
        std::tie(path2terminal_nodes, path2terminal_edges) = find_SP_to_nearest_terminal(g, inside_Steiner, is_terminal);

        // Add new nodes to Steiner set
        for (Vertex node : make_iterator_range(path2terminal_nodes)) {
            inside_Steiner[node] = 1;
        }

        // Add new edges to Steiner set
        steiner_tree.insert(steiner_tree.end(), path2terminal_edges.begin(), path2terminal_edges.end());
        counter++;
    }
    return steiner_tree;
}

std::tuple< std::vector<Vertex>, std::vector<Edge>> find_SP_to_nearest_terminal(const Graph& g, const std::unordered_map<Vertex, int>inside_Steiner, const std::unordered_map<Vertex, int>is_terminal) {
	// Find path to nearest terminal node from set of nodes in Steiner set.
	// Uses a modified version of Dijsktra's algorithm, in which every node s in Steinerset is initialized as root, ie pred(s), dist(s).
	// The algorithm then proceeds exactly like Dijkstra's algorithm, until is scanned the terminal node not inside Steiner set.
	// For better details see the implentation in myShortestPathtree.cpp
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

    // Construct path from terminal node back to nearest Steiner node
	std::vector<Vertex> path_nodes;
    std::vector<Edge> path_edges;
	Vertex node = endVertex;
    Vertex pred = get<1>(node_info_map[node]);

    // Loop backwards until reaching steiner node
    while (inside_Steiner.find(node) == inside_Steiner.end()) {
        // Find edge that joins pred to node
        for (auto edge : make_iterator_range(out_edges(node, g))) {
            if (target(edge, g) == pred) {
                path_edges.push_back(edge);
                path_nodes.push_back(node);
                break;
            }
        }
        node = pred;
        pred = get<1>(node_info_map[node]);
    }

    return std::tuple< std::vector<Vertex>, std::vector<Edge>> {path_nodes, path_edges};
};


