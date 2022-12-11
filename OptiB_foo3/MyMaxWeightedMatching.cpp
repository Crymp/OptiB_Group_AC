#include "MyMaxWeightedMatching.h"
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <iostream>
#include <boost/graph/find_flow_cost.hpp>
#include "min_cost_max_flow_utils.hpp"

using namespace boost;

std::vector<Vertex> my_max_weighted_matching(const Graph &g) {

  std::vector<Vertex> mate(num_vertices(g));

  // TODO compute maximum weighted matching
  std::cout << "Hello World!\n";


  //EdgeWeightMap weight{ get(boost::edge_weight, g) };

  //std::cout << "Printing edges:\n";
  //for (auto e : make_iterator_range(edges(g))) {
	 // std::cout << source(e, g) << " -> " << target(e, g) << ": " << get(edge_weight,g, e) << "\n";
  //}

  

  // Defining my own Graph

 // SampleGraph::vertex_descriptor s, t;
 // boost::SampleGraph::Graph GG;
 // boost::SampleGraph::getSampleGraph(GG, s, t);

 // boost::successive_shortest_path_nonnegative_weights(GG, s, t);

 // int cost = boost::find_flow_cost(GG);
 // assert(cost == 29);


 // //std::cout << "Printing edges:\n";
	//for (auto e : make_iterator_range(edges(GG))) {
	//	std::cout << source(e, GG) << "," << target(e, GG) << "," << get(edge_capacity, GG, e) << "," << get(edge_weight, GG, e) << "\n"; // 
	//}



  //adjacency_list<vecS, vecS, directedS, boost::no_property, EdgeWeightProperty > G;
  DiGraph G;

  EdgeCapacityMap edge_capacity = get(boost::edge_capacity, G);
  EdgeResidualCapacityMap ResidualCapacityEdgeMap = get(boost::edge_residual_capacity, G);
  EdgeReverseMap edge_reverse = get(boost::edge_reverse, G);


  Vertex v0 = add_vertex(G);
  Vertex v1 = add_vertex(G);
  Vertex v2 = add_vertex(G);
  Vertex v3 = add_vertex(G);
 
  auto e0 = add_edge(v0, v1, 2, G);
  auto e1 = add_edge(v0, v2, 1, G);
  auto e2 = add_edge(v1, v3, 1, G);
  auto e3 = add_edge(v2, v3, 3, G);
  auto e4 = add_edge(v1, v2, 3, G);

  auto r0 = add_edge(v1, v0, 2, G);
  auto r1 = add_edge(v2, v0, 1, G);
  auto r2 = add_edge(v3, v1, 1, G);
  auto r3 = add_edge(v3, v2, 3, G);

  edge_reverse[e0.first] = r0.first;
  edge_reverse[r0.first] = e0.first;

  edge_reverse[e1.first] = r1.first;
  edge_reverse[r1.first] = e1.first;

  edge_reverse[e2.first] = r2.first;
  edge_reverse[r2.first] = e2.first;

  edge_reverse[e3.first] = r3.first;
  edge_reverse[r3.first] = e3.first;


  edge_capacity[e0.first] = 5;
  edge_capacity[e1.first] = 3;
  edge_capacity[e2.first] = 2;
  edge_capacity[e3.first] = 4;

  edge_capacity[r0.first] = 0;
  edge_capacity[r1.first] = 0;
  edge_capacity[r2.first] = 0;
  edge_capacity[r3.first] = 0;



// Reverse Capacity
  ResidualCapacityEdgeMap[e0.first] = 5;
  ResidualCapacityEdgeMap[e1.first] = 3;
  ResidualCapacityEdgeMap[e2.first] = 2;
  ResidualCapacityEdgeMap[e3.first] = 4;

  ResidualCapacityEdgeMap[r0.first] = 0;
  ResidualCapacityEdgeMap[r1.first] = 0;
  ResidualCapacityEdgeMap[r2.first] = 0;
  ResidualCapacityEdgeMap[r3.first] = 0;




  successive_shortest_path_nonnegative_weights(G, v0, v3);


  std::cout << "Printing nodes:\n";
  for (auto v : make_iterator_range(vertices(G))) {
	  std::cout<< v << "\n";
  }

  std::cout << "Printing edges:\n";
  for (auto e : make_iterator_range(edges(G))) {
	  std::cout << source(e, G) << " -> " << target(e, G) << ": " << get(edge_capacity, e) << " / " << get(edge_weight, G, e) <<  ": " << edge_capacity[e] - ResidualCapacityEdgeMap[e] << "\n"; // 
  }


  return mate;
}
