#include "mySpanningTree.h"
#include <boost/graph/graph_traits.hpp>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>
#include <algorithm>    
#include <iterator>
#include <utility>                   // for std::pair
#include <unordered_map>
#include <functional>

using namespace boost;

void iterate_nodes(const Graph& g, std::unordered_map<int, int> colour_of_node) {
    // ...

    // get the property map for vertex indices
    typedef property_map<Graph, vertex_index_t>::type IndexMap;
    IndexMap index = get(vertex_index, g);

    // std::cout << "vertices(g) = ";
    typedef graph_traits<Graph>::vertex_iterator vertex_iter;
    std::pair<vertex_iter, vertex_iter> vp;
    for (vp = vertices(g); vp.first != vp.second; ++vp.first) {
        int node = index[*vp.first];
        int colour = (colour_of_node.find(node) == colour_of_node.end()) ? -2 : colour_of_node[node];
        std::cout << node << ": " << colour << std::endl;
    }
        //std::cout << index[*vp.first] << " " << endl;
    //std::cout << std::endl;

}



std::vector<Edge> my_spanning_tree(const Graph& g) {
    std::vector<Edge> spanning_tree;
    // compute a minimal spanning tree for g

        // ...

    // Get list of edges in a way such that it can be sorted by stl
    //graph_traits<Graph>::edge_iterator edges_it_start, edges_it_end;
    //tie(edges_it_start, edges_it_end) = edges(g);
    
    
    std::vector<Edge> sorted_edges;
    graph_traits<Graph>::edge_iterator edge_it_start, edges_it_end;
    tie(edge_it_start, edges_it_end) = edges(g);

    for (auto edge_it = edge_it_start; edge_it != edges_it_end; edge_it++) {
        sorted_edges.push_back(*edge_it);
    }
    
    
    // Sorted Edges
    std::sort(sorted_edges.begin(), sorted_edges.end(),
        [g](Edge e1, Edge e2) -> bool {return (get(edge_weight, g, e1)) < get(edge_weight, g, e2); }
    );

    
    // Loop over edges until complete
    std::unordered_map<int, int> colour_of_node; //node->colour    returns the colour of a node
    std::unordered_map<int, std::vector<int>> nodes_with_colour; //colour->node  returns list of node that have colour
    int max_colour = -1;

    graph_traits<Graph>::edge_iterator ei, ei_end;
    //for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
    for (auto edge = sorted_edges.begin(); edge != sorted_edges.end(); edge++) {
        // Prints edge (source, target) and edge case
        //std::cout << "(" << source(*edge, g) << "," << target(*edge, g) << "): " << get(edge_weight, g, *edge) << " - ";

        int node1 = source(*edge, g);
        int node2 = target(*edge, g);
        int colour1 = (colour_of_node.find(node1) == colour_of_node.end()) ? -2 : colour_of_node[node1];
        int colour2 = (colour_of_node.find(node2) == colour_of_node.end()) ? -2 : colour_of_node[node2];
        // -2 signifies has never been found yet

        std::cout << get(edge_weight, g, *edge) << " - (" << node1 << ": " << colour1 << ", " << node2 << ": " << colour2 << " ) ";
        if (colour1 == colour2) {
            if (colour1 == -2) {
                // Both nodes have not been added yet
                max_colour++;
                colour_of_node[node1] = max_colour;
                colour_of_node[node2] = max_colour;
                nodes_with_colour[max_colour] = std::vector<int>{ node1, node2 };
                std::cout << "both unseen nodes " << node1 << " & " << node2 << ", now with colour " << max_colour << std::endl;
                spanning_tree.push_back(*edge);

            }
            else {
                // Both nodes same colour... this would close a loop. Skip this.
                std::cout << "closed loop" << colour1 << std::endl;
            }

        }
        else {
            if (colour1 == -2) {
                // Give node1 the same colour as node2
                colour_of_node[node1] = colour2;
                nodes_with_colour[colour2].push_back(node1);
                std::cout << "node " << node1 << " unseen" << std::endl;

            }
            else if (colour2 == -2) {
                // Give node2 the same colour as node1
               colour_of_node[node2] = colour1;
               // std::vector<int> foo1 = nodes_with_colour[colour1];
               // foo1.emplace_back(node2);

               //nodes_with_colour[colour1] = foo1;
               nodes_with_colour[colour1].push_back(node2);
               std::cout << "node " << node2 <<" unseen" << std::endl;
            }
            else {
                // Pick node with lower colour (higher colour liklier to contain less nodes)
                // Make all nodes that have the colour as node2 the same colour as node1
                std::cout << " - repainting nodes with colour " << colour1 << ": ";
                std::vector<int> nodes_to_repaint = nodes_with_colour[colour2];
                for (auto node = nodes_to_repaint.begin(); node != nodes_to_repaint.end(); node++) {
                    std::cout << *node << ", ";
                    colour_of_node[*node] = colour1;
                }
                std::cout << std::endl;
                //Update entry of nodes that have colours 1
                std::vector<int> nodes_with_colour1 = nodes_with_colour[colour1];
                nodes_with_colour1.insert(nodes_with_colour1.end(), nodes_to_repaint.begin(), nodes_to_repaint.end());
                nodes_with_colour[colour1] = nodes_with_colour1;

                //Remove entries that have colour 2
                nodes_with_colour.erase(colour2);

            }
            spanning_tree.push_back(*edge);
       }
        // ...

        
    }
    //iterate_nodes(g, colour_of_node);
    return spanning_tree;
}