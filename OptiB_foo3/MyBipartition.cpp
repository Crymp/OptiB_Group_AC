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

  std::vector<int> debug(unmarked_nodes.size(), -2);

  Vertex odd_loop_start;

  int i = 0;
  // Loop until all nodes marked (or until it discovered that not bipartite)
  while (unmarked_nodes.size() != 0) {
     // std::cout << "# unmarked: " << unmarked_nodes.size() << "\n";

      // Select arbitrary unmarked node, and mark it
      Vertex node0 = (*unmarked_nodes.begin()).first;
      unmarked_nodes.erase(node0);

      colours[node0] = 0;
      myPartition.first.push_back(node0);
      debug[node0] = i;

      // Iteratively find and mark neighbours, until no unmarked node can be found
      std::deque<Vertex> node_queue{ node0 };
      while (node_queue.size() != 0) {
          Vertex node1 = node_queue.front();

          // Loop through all neighbouring nodes
          for (auto edge : make_iterator_range(out_edges(node1, g))) {
              Vertex node2 = target(edge, g);
              if (colours[node2] == colours[node1]) {
                  // Two neighbouring nodes have the same colour..
                  // Break assignment and go find that odd loop
                  odd_loop_start = node2;
                  bipartite = false;
                  goto find_odd_loop;
              }
              if (colours[node2] == -1) {
                  // node2 unmarked: mark it opposite colour
                  unmarked_nodes.erase(node2);
                  node_queue.push_back(node2);
                  debug[node2] = i;
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
      i++;
  }



find_odd_loop:
  if (bipartite == false) {
      // Find odd cycle with BFS and root odd_loop_start
      //
      // In BFS pattern, iterate out from the root s until leafs of BFS "tree" meet at neighbouring nodes v1 and v2.
      // (Note: not actually a tree since it contains repeated nodes).
      // The paths from s->v1 and v2->s joined will be the odd elementary cycle, if |s->v1| + |s->v2| is odd
      // and when {s->v1 U v2->s} contains no repeated nodes (other than root s).
      // To check this; for every node v in search path we keep lookup table (unordered hashmap; O(1) access) for all
      // nodes encountered path from s to v. Ie a node u will only be in lookup table of v, if u is in path from s to v,
      // and the lookup table table will map u to the index position of u in path from s to v.
      
      // Lookup table to indicate
      std::vector<std::unordered_map<Vertex, int>> tree_paths(colours.size());
      tree_paths[odd_loop_start][odd_loop_start] = 0;
     
      std::deque<Vertex> queue{ odd_loop_start };
      while (true) {
          Vertex node1 = queue.front();          
          for (auto e : make_iterator_range(out_edges(node1, g))) {
              Vertex node2 = target(e, g);

              if (tree_paths[node2].empty()) {
                  // node2 unseen in BFS
                  tree_paths[node2] = tree_paths[node1];
                  tree_paths[node2][node2] = tree_paths[node1].size();
                  queue.push_back(node2);
              }
              
              // node2 in bfs, check if they form an odd cycle
              else if ((tree_paths[node1].size() + tree_paths[node2].size() - 1) % 2 == 1) {                   
                      
                   // check cycle is elementary, ie no repeated nodes (other than root)
                   bool repeated_nodes = false;
                   for (auto node_idx : tree_paths[node2]) {
                       Vertex v = node_idx.first;
                       if ((v!= odd_loop_start) && (tree_paths[node1].find((v)) != tree_paths[node1].end())) {
                           // node v in both paths from root to node1 and node2
                           repeated_nodes = true;
                           break;
                       }
                   }

                   if (repeated_nodes == false) {
                       // build odd cycle by joining bfs paths from root to node1 and node2

                       int len = tree_paths[node1].size() + tree_paths[node2].size(); // length of cycle
                       std::vector<Vertex> cycle (len);

                       // path from root to node1
                       for (auto node_idx : tree_paths[node1]) {
                           cycle[node_idx.first] = node_idx.second;
                       }

                       // path from node2 to root (ie add in reverse
                       for (auto node_idx : tree_paths[node2]) {
                           cycle[len - 1 - node_idx.second] = node_idx.first;
                       }
                       return cycle;
                   }
              }
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




//std::cout << "Debug Info:\n";
//for (auto v : make_iterator_range(vertices(g))) {
//    std::cout << v << ": " << colours[v] << ", " << debug[v] << "\n";
//}
//if (bipartite == false) {
//    std::cout << "Searching for odd loop\n";

//    //unordered_map<Vertex, int> marked;
//    //marked[odd_loop_start] = 1;
//    std::unordered_map<Vertex, std::vector<Vertex>> node_indexes;
//    node_indexes[odd_loop_start] = std::vector<Vertex>{ odd_loop_start };
//    std::deque<Vertex> queue{odd_loop_start};

//    bool flag = true;
//    while (flag) {
//        //Vertex node1 = (*node_indexes.begin()).first;
//        std::cout << "QUEUEU is "; for (auto v : queue) { std::cout << v << ", "; }; std::cout << "\n";
//        Vertex node1 = queue.front();
//        std::cout << "list is: "; for (auto v : node_indexes[node1]) { std::cout << "(" << v << ", " << debug[v] << ")" << ", "; }; std::cout << "\n";
//        
//        for (auto e : make_iterator_range(out_edges(node1, g))) {

//            Vertex node2 = target(e, g);
//            std::cout << "    " << node2 << ": ";
//            if ((node2 == odd_loop_start) && (node_indexes[node2].size()>1)) {
//                std::cout << "FOUND IT!\n";
//               // flag = false;
//                std::cout << node_indexes[node2].size() << "\n";
//                return node_indexes[node2];
//   
//            }
//            if (node_indexes.find(node2) == node_indexes.end()) {
//                node_indexes[node2] = node_indexes[node1];
//                node_indexes[node2].push_back(node2);
//                queue.push_back(node2);
//                std::cout << debug[node2] << ", unseen\n";
//            }
//            else {
//                std::cout << debug[node2] << ", seen\n";
//            }
//        }
//        queue.pop_front();
//    }
//}