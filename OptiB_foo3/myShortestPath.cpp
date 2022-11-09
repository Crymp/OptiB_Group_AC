#include "myShortestPath.h"
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
#include <tuple>
#include <sstream>

using namespace boost;
#include <limits>

std::stringstream printNI(Nodeinfo nodeinfo) {


    
    int dist1; Vertex node1; int counter1; //node is "v" in Dijsktras algorithm
    std::tie(dist1, node1, counter1) = nodeinfo;
    std::stringstream stream;
    stream << "( " << dist1 << ", " << node1 << ", " << counter1 << ")";
    return stream;
}


std::vector<Vertex> my_shortest_path(const Graph &g, const Vertex &startVertex,
                                     const Vertex &endVertex) {
  std::vector<Vertex> path;
  // compute a shortest path between startVertex and endVertex and save it in
  // the vector >>path<<
  
  //





  /*

  // Custom typdef to keep track of node information // typedef std::tuple<int, Vertex, int> Nodeinfo;

  // first is dist(v) in Alg, second is the vertex, and first is a counter keepin track of how often appeared in priority_queueu
  std::priority_queue<Nodeinfo, std::vector<Nodeinfo>, std::greater<Nodeinfo>> nodes_queue; // Keeps track of nodes and their distances {dist, node, counter}
  std::unordered_map<Vertex, Nodeinfo> node_info_map; //node -> {dist, pred, counter}

  // Generate queue and lookup
  for (auto node : make_iterator_range(vertices(g))) {
      //std::cout << node << std::endl;
      if (node != startVertex) {
          int dist = std::numeric_limits<int>::max(); //Set distance to "infinity"
          nodes_queue.push(Nodeinfo{ dist, node, 0 });
          node_info_map[node] = Nodeinfo{ dist, NULL, 0 };
      }
      else {
          nodes_queue.push(Nodeinfo{ 0, node, 0 });
          node_info_map[node] = Nodeinfo{ 0, node, 0 };
      }
  }

  std::cout << "start vertex " << startVertex << ", end vertex " << endVertex << std::endl;


  // Build (full) shortest path trree
  while (nodes_queue.size() != 0) {
      int dist1; Vertex node1; int counter1; //node is "v" in Dijsktras algorithm
      std::tie(dist1, node1, counter1) = nodes_queue.top();
      nodes_queue.pop();
      Nodeinfo foo = node_info_map[node1];
      int counterfoo = std::get<2>(foo);
      std::cout << "( " << node1 << ": " << dist1 << ", " << counter1 << ")" << " vs " << "( " << std::get<1>(foo) << ": " << std::get<0>(foo) << ", " << std::get<2>(foo) << ")" << std::endl;
      while (counter1 != counterfoo) {
          //Clear any outdated entries from queue
          foo = node_info_map[node1];
          counterfoo = std::get<2>(foo);
          std::tie(dist1, node1, counter1) = nodes_queue.top();
          std::cout << "( " << node1 << ": " << dist1 << ", " << counter1 << ")" << " vs " << "( " << "__" << ": " << std::get<0>(foo) << ", " << std::get<2>(foo) << ")" << std::endl;

          nodes_queue.pop();
      }
      std::cout << "    Done \n";
      // Iterate through all outgoing_edges/neighbour nodes
      auto incident_edges = out_edges(node1, g);
      for (auto const edge : make_iterator_range(incident_edges)) {
          Vertex node2 = target(edge, g);
          int edge_cost = get(edge_weight, g, edge);

          int dist2; Vertex node2_pred; int counter2; //node2 is "w" in Dijsktras algorithm
          std::tie(dist2, node2_pred, counter2) = node_info_map[node2];
          if (dist1 + edge_cost < dist2){
              // Update dist, pred and counter if less
              Nodeinfo node2_info_updated = Nodeinfo{ dist1 + edge_cost, node1, counter2 + 1 };
              node_info_map[node2] = node2_info_updated;
              nodes_queue.push(node2_info_updated);
          }
      }
     // std::cout << node1 << ", " << dist1 << ", " << counter1 << std::endl;
  }

  
// Construct path from min_distance tree

  Vertex pred = std::get<1>(node_info_map[endVertex]);
  path.push_back(pred);
  while (pred != startVertex) {
      pred = std::get<1>(node_info_map[endVertex]);
      path.push_back(pred);
  }
 
  std::reverse(path.begin(), path.end());
  */

  return path;



}


/*
///////////////////
// Neighbours of a node
graph_traits<Graph>::edge_iterator edge_it_start, edge_it_end;
tie(edge_it_start, edge_it_end) = edges(g);
Gives neighbours of a node
auto node1 = source(*edge_it_start, g);
auto neighbours = boost::adjacent_vertices(node1, g);
for (auto node2 : make_iterator_range(neighbours)) {
    std::cout << node2 << std::endl;
}

////


//////// Get incident outedges to a node
  graph_traits<Graph>::edge_iterator edge_it_start, edge_it_end;
  tie(edge_it_start, edge_it_end) = edges(g);
  auto node1 = source(*edge_it_start,g);
  auto incident_edges = out_edges(node1, g);
  for (auto edge : make_iterator_range(incident_edges)) {
      std::cout << source(edge, g) << " -> " << target(edge,g) << std::endl;
  }
/////



// Check for loops


*/