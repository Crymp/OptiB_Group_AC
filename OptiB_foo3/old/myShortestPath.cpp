#include "myShortestPath.h"
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
          nodes_queue.push(Nodeinfo{ dist, node, counter});
          node_info_map[node] = Nodeinfo{ dist, pred, counter };
      }
      else {
          // Root vertex s has special values dist(s)=0, pred(s)=s (but it is not yet scanned)
          nodes_queue.push(Nodeinfo{ 0, node, 0 });
          node_info_map[node] = Nodeinfo{ 0, node, 0 };
      }
  }


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
          if( (counter2 != -1) && (dist1 + edge_cost < dist2)){
              int dist2 = dist1 + edge_cost;
              counter2++;
              // Update tuple in hash_map, and push new updated tuple into queue
              node_info_map[node2] = Nodeinfo{ dist2, node1, counter2};
              nodes_queue.push(Nodeinfo{ dist2, node2, counter2});
            }
      }
      // Mark node1 as seen (ie remove to "remove v from V'  " in alg)
      get<2>(node_info_map[node1]) = -1;
     
      // If endVertex is marked as seen, then endVertex is permantly marked in shortestpathtree, ie we have found the shorted path from so to v/
      if (node1 == endVertex) {
          break;
      }

  }
  
// Construct path from min_distance tree by going backward
  Vertex pred = endVertex;
  path.push_back(pred);
  while (pred != startVertex) {
      pred = std::get<1>(node_info_map[pred]);
      path.push_back(pred);   
  }
 // Reverse path
  std::reverse(path.begin(), path.end());  

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