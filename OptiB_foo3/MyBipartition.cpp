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

          if (colours[node1] == -1) {
              std::cout << "oops\n";
          }

          // Loop through all neighbouring nodes
          for (auto edge : make_iterator_range(out_edges(node1, g))) {
              Vertex node2 = target(edge, g);
              //std::cout << "(" << node1 << ", " << colours[node1] << ") -> (" << node2 << ", " << colours[node2] << ") ";
              if (colours[node2] == colours[node1]) {
                  // Two neighbouring nodes have the same colour..
                  std::cout << "ALARM!!! Graph is not bipartite\n";
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
              //std::cout << colours[node2] << "\n";

          }
          node_queue.pop_front();
      }
      i++;
  }




  std::cout << "\n";
  std::cout << "Reached end of loop.... is bipartite: " << bipartite << "\n";
// If not bipartite, use this code section to find an odd loop
find_odd_loop:

  //std::cout << "Debug Info:\n";
  //for (auto v : make_iterator_range(vertices(g))) {
  //    std::cout << v << ": " << colours[v] << ", " << debug[v] << "\n";
  //}
  if (bipartite == false) {
      std::cout << "Searching for odd loop\n";

      //unordered_map<Vertex, int> marked;
      //marked[odd_loop_start] = 1;
      //std::unordered_map<Vertex, std::vector<Vertex>> node_indexes;
      //node_indexes[odd_loop_start] = std::vector<Vertex>{ odd_loop_start };
      std::deque<Vertex> queue{odd_loop_start};
      std::vector<std::unordered_map<Vertex, int>> tree_paths(colours.size());
      tree_paths[odd_loop_start][odd_loop_start] = 0;
     

      bool flag = true;
      while (flag) {
          //Vertex node1 = (*node_indexes.begin()).first;
          //std::cout << "QUEUEU is "; for (auto v : queue) { std::cout << v << ", "; }; std::cout << "\n";
          Vertex node1 = queue.front();
         // std::cout << "list is: "; for (auto v : node_indexes[node1]) { std::cout << "(" << v << ", " << debug[v] << ")" << ", "; }; std::cout << "\n";
          
          for (auto e : make_iterator_range(out_edges(node1, g))) {
              Vertex node2 = target(e, g);
              //std::cout << "    " << node2 << ": ";
          
              if (tree_paths[node2].empty()) {
                  // node2 unseen in BFS
                  tree_paths[node2] = tree_paths[node1];
                  tree_paths[node2][node2] = tree_paths[node1].size();
                  queue.push_back(node2);
                  //std::cout << debug[node2] << ", unseen\n";
              }
              else {
                  // node2 in BFS, check if they form an odd cycle
                  if ((tree_paths[node1].size() + tree_paths[node2].size() - 1) % 2 == 1) {



                      bool repeated_nodes = false;
                      
                      // Check cycle is elementary, ie no repeated nodes (other than root)
                      for (auto x : tree_paths[node2]) {
                          if ((x.first != odd_loop_start) && (tree_paths[node1].find((x.first)) != tree_paths[node1].end())) {
                              repeated_nodes = true;
                              break;
                          }
                      }

                      // Finally found odd elementary cycle (now combine them)
                      if (repeated_nodes == false) {

                          int len = tree_paths[node1].size() + tree_paths[node2].size(); // length of cycle
                          std::vector<Vertex> cycle (len);

                          // Add nodes from 1st path from front
                          for (auto x : tree_paths[node1]) {
                              int idx = x.second;
                              int v = x.first;
                              cycle[x.second] = x.first;
                          }

                          // Add nodes from 2nd path from behind (skip root node)
                          for (auto x : tree_paths[node2]) {
                              cycle[len - 1 - x.second] = x.first;
                          }
                          return cycle;
                      }
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