#include "MyBipartition.h"
#include <vector>
#include <unordered_map>
#include <deque>
#include <iostream>
using namespace boost;





Partition_variant my_bipartition(const Graph &g) {
  Partition_variant res;
  std::pair<std::vector<Vertex>, std::vector<Vertex>> myPartition;
  std::vector<Vertex> odd_cycle;
  bool bipartite{true};

  // TODO compute bipartition


  // Add all nodes to list of unmarked nodes)
  //std::vector<Vertex> unmarked_nodes;
  //for (auto v : make_iterator_range(vertices(g))) {unmarked_nodes.push_back(v);}
  std::unordered_map<Vertex, int> unmarked_nodes;
  for (auto v : make_iterator_range(vertices(g))) {unmarked_nodes[v] = -1;}

  // Mark all nodes as unmarked ie colour of -1 (colour 0 or 1 would indicate member of a bipartite set)
  std::vector<int> colours(unmarked_nodes.size());
  for (auto v : unmarked_nodes) {colours[v.first] = -1; }


  Vertex odd_loop_start;

  // Loop until all nodes marked (or until it discovered that not bipartite)
  while (unmarked_nodes.size() != 0) {

      // Select arbitrary unmarked node, and mark it
      Vertex node0 = (*unmarked_nodes.begin()).first;
      unmarked_nodes.erase(node0);
      colours[node0] = 0;
      myPartition.first.push_back(node0);

      // Iteratively find and mark neighbours, until no unmarked node can be found
      std::deque<Vertex> node_queue{ node0 };
      while (node_queue.size() != 0) {
          Vertex node1 = node_queue.front();

          // Loop through all neighbouring nodes
          for (auto edge : make_iterator_range(out_edges(node1, g))) {
              Vertex node2 = target(edge, g);
              if (colours[node2] == colours[node1]) {
                  // Two neighbouring nodes have the same colour..
                  std::cout << "ALARM!!! Graph is not bipartite\n";
                  // Break assignment and go find that odd loop
                  odd_loop_start = node1;
                  goto find_odd_loop;
                  bipartite = false;
              }
              else if (colours[node2] == -1) {
                  // node2 unmarked: mark it opposite colour
                  unmarked_nodes.erase(node2);s
                  if (colours[node1] == 0) {
                      colours[node2] = 1;
                      myPartition.second.push_back(node2);
                  }
                  else {
                      colours[node2] = 0;
                      myPartition.first.push_back(node2);
                  }
              }
          }
          node_queue.pop_front();
      }
  }

// If not bipartite, use this code section to find an odd loop
find_odd_loop:
  if (bipartite == false) {
      //unordered_map<Vertex, int> marked;
      //marked[odd_loop_start] = 1;
      std::unordered_map<Vertex, std::vector<int>> node_indexes;
      node_indexes[odd_loop_start] = std::vector<int>(0);
      std::deque<Vertex> queue{odd_loop_start};

      bool flag = true;
      while (flag) {
          //Vertex node1 = (*node_indexes.begin()).first;
          Vertex node1 = queue.front();
          int i = 0;
          for (auto e : make_iterator_range(out_edges(node1, g))) {
              Vertex node2 = target(e, g);
              if (node2 == odd_loop_start) {
                  std::cout << "FOUND IT!\n";
                  flag = false;
                  break;
              }
              if (node_indexes.find(node2) == node_indexes.end()) {
                  node_indexes[node2] = node_indexes[node1];
                  node_indexes[node2].push_back(i);
              }
              i++;
          }
          queue.pop_front();
      }
  }

      
 


  //return
  if (bipartite) {
    res = myPartition;
  } else {
    res = odd_cycle;
  }
  //------------------------

  return res;
}
