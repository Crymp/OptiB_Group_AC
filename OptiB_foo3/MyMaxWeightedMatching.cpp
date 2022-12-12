#include "MyMaxWeightedMatching.h"
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <iostream>
#include <boost/graph/find_flow_cost.hpp>
#include "min_cost_max_flow_utils.hpp"
#include "MyBipartition.h"
#include <algorithm>

#include <boost/graph/bipartite.hpp>
using namespace boost;



std::vector<Vertex> my_max_weighted_matching(const Graph& g) {
	std::cout << "Boost Bipartite?: " << is_bipartite(g) << "\n";
	std::vector<Vertex> mate(num_vertices(g));

	// TODO compute maximum weighted matching
	std::cout << "Hello World!\n";


	//EdgeWeightMap weight{ get(boost::edge_weight, g) };

	//std::cout << "Printing edges:\n";
	//for (auto e : make_iterator_range(edges(g))) {
	//    std::cout << source(e, g) << " -> " << target(e, g) << ": " << get(edge_weight,g, e) << "\n";
	//}
	//std::cout << "done\n";



	// Compute Bipartite node sets U and W
	auto res = my_bipartition(g);
	std::vector<Vertex> U(boost::get<std::pair<std::vector<Vertex>, std::vector<Vertex>>>(res).first);
	std::vector<Vertex> W(boost::get<std::pair<std::vector<Vertex>, std::vector<Vertex>>>(res).second);
	//std::cout << "Printing nodes in U\n";
	//for (auto u : U) {
	//	std::cout << u << "\n";
	//}
	//std::cout << "Printing nodes in W\n";
	//for (auto w : W) {
	//	std::cout << w << "\n";
	//}


	// Find maximum edge cost
	unsigned int max_cost = 0;
	for (auto e : make_iterator_range(edges(g))) {
		max_cost = std::max(max_cost, get(edge_weight, g, e));
	}

	MyGraph G;
	MyCapacity capacity = get(edge_capacity, G);
	MyReversed rev = get(edge_reverse, G);
	MyResidualCapacity residual_capacity = get(edge_residual_capacity, G);
	MyWeight Edge_weight = get(edge_weight, G);
	
	
	// Add Vertices
	for (auto v : make_iterator_range(vertices(G))) {
		add_vertex(G);
	}
	
	
	//max_cost++;
	// Defining my own Graph
	
	// Add Arcs (and virtual reverse Arcs)
	for (Vertex u : U) {
		for (auto e : make_iterator_range(out_edges(u, g))) {
			Vertex w = target(e, g);

			int weight = (max_cost - get(edge_weight, g, e));

			// Define new Arc (u,w) with weight of max_weigh
			auto a = add_edge(u, w, weight, G);
			capacity[a.first] = 1;
			residual_capacity[a.first] = 1;

			// Define Reverse Arc (w,u) with weight and capacity (0 since actually not on graph)
			auto rev_a = add_edge(w, u, -weight, G);
			capacity[rev_a.first] = 0;
			residual_capacity[rev_a.first] = 0;

			// Define arc reverse relations
			rev[a.first] = rev_a.first;
			rev[rev_a.first] = a.first;
		}
	}

//	// Add source vertex s, and edges (s,u) with cost 0 for all u in U:
//	Vertex s = add_vertex(G);
//
//	for (Vertex u : U) {
//
//		int weight = 0;
//
//		// Define new Arc (u,w) with weight of max_weigh
//		auto a = add_edge(s, u, weight, G);
//		capacity[a.first] = 1;
//		residual_capacity[a.first] = 1;
//
//		// Define Reverse Arc (w,u) with weight and capacity (0 since actually not on graph)
//		auto rev_a = add_edge(u,s, -weight, G);
//		capacity[rev_a.first] = 0;
//		residual_capacity[rev_a.first] = 0;
//
//		// Define arc reverse relations
//		rev[a.first] = rev_a.first;
//		rev[rev_a.first] = a.first;
//}
//
//	Vertex t = add_vertex(G);
//	for (Vertex w : W) {
//
//		int weight = 0;
//
//		// Define new Arc (u,w) with weight of max_weigh
//		auto a = add_edge(w, t, weight, G);
//		capacity[a.first] = 1;
//		residual_capacity[a.first] = 1;
//
//		// Define Reverse Arc (w,u) with weight and capacity (0 since actually not on graph)
//		auto rev_a = add_edge(t, w, -weight, G);
//		capacity[rev_a.first] = 0;
//		residual_capacity[rev_a.first] = 0;
//
//		// Define arc reverse relations
//		rev[a.first] = rev_a.first;
//		rev[rev_a.first] = a.first;
//	}
//
//	successive_shortest_path_nonnegative_weights(G, s, t);
//
//	std::vector<int> touched(num_vertices(g), 0);
//	for (auto e : make_iterator_range(edges(G))) {
//		Vertex u = source(e, G);
//		Vertex w = target(e, G);
//		int flow = get(capacity, e) - residual_capacity[e];
//		
//		if ((u != s) && (u != t) && (w != s) && (w != t) && (get(capacity, e) != 0) && (flow == 1)) {
//			mate[u] = w;
//			mate[w] = u;
//			touched[u] = 1;
//			touched[w] = 1;
//		}
//	}
//
//	for (Vertex v : make_iterator_range(vertices(g))) {
//		if (touched[v] == 0) {
//			mate[v] = boost::graph_traits<Graph>::null_vertex();
//		}
//	}

	std::cout << "Printing edges:\n";
  for (auto e : make_iterator_range(edges(G))) {
	  int cap = get(capacity, e);
	  if(true){//if (cap != 0) {
		  std::cout << "(" << source(e, G) << ", " << target(e, G) << ") w:" << get(edge_weight, G, e) << ", cap:" << get(capacity, e) << ", f:" << capacity[e] - residual_capacity[e] << "\n"; // 
	  }
	}


//	
//	
//	
//	
//	DiGraph G;
//	EdgeCapacityMap edge_capacity = get(boost::edge_capacity, G);
//	EdgeResidualCapacityMap ResidualCapacityEdgeMap = get(boost::edge_residual_capacity, G);
//	EdgeReverseMap edge_reverse = get(boost::edge_reverse, G);
//
//	// Add nodes
//	for (auto v : make_iterator_range(vertices(g))) {
//		auto v = add_vertex(G);
//		//std::cout << v << "\n";
//	}
//
//	// Add Arcs (and virtual reverse Arcs)
//	for (Vertex u : U) {
//		for (auto e : make_iterator_range(out_edges(u, g))) {
//			Vertex w = target(e, g);
//
//
//			// Define new Arc (u,w) with weight of max_weigh
//			auto a = add_edge(u, w, max_cost - get(edge_weight, g, e), G);
//			edge_capacity[a.first] = 1;
//			ResidualCapacityEdgeMap[a.first] = 1;
//
//			// Define Reverse Arc (w,u) with weight and capacity (0 since actually not on graph)
//			auto rev_a = add_edge(w, u, -(max_cost - get(edge_weight, g, e)), G);
//			edge_capacity[rev_a.first] = 0;
//			ResidualCapacityEdgeMap[rev_a.first] = 0;
//
//			// Define arc reverse relations
//			edge_reverse[a.first] = rev_a.first;
//			edge_reverse[rev_a.first] = a.first;
//		}
//	}
//
//
//	// Add extra arcs from source s to all nodes in U 
//	Vertex s = add_vertex(G);
//	for (Vertex u : U) {
//		// Define new Arc (u,w) with weight max_cost and capacity  1
//		auto a = add_edge(s, u, 0, G);
//		edge_capacity[a.first] = 1;
//		ResidualCapacityEdgeMap[a.first] = 1;
//
//		// Define Reverse Arc (w,u) with weight and capacity (0 since actually not on graph)
//		auto rev_a = add_edge(u, s, 0, G);
//		edge_capacity[rev_a.first] = 0;
//		ResidualCapacityEdgeMap[rev_a.first] = 0;
//
//		// Define arc reverse relations
//		edge_reverse[a.first] = rev_a.first;
//		edge_reverse[rev_a.first] = a.first;
//	}
//
//	// Add extra arcs from all nodes in W to target
//	Vertex t = add_vertex(G);
//	for (Vertex w : W) {
//		// Define new Arc (u,w) with weight max_cost and capacity  1
//		auto a = add_edge(w, t, 0, G);
//		edge_capacity[a.first] = 1;
//		ResidualCapacityEdgeMap[a.first] = 1;
//
//		// Define Reverse Arc (w,u) with weight and capacity (0 since actually not on graph)
//		auto rev_a = add_edge(t, w, 0, G);
//		edge_capacity[rev_a.first] = 0;
//		ResidualCapacityEdgeMap[rev_a.first] =  0;
//
//		// Define arc reverse relations
//		edge_reverse[a.first] = rev_a.first;
//		edge_reverse[rev_a.first] = a.first;
//	}
//
//	//std::cout << "Printing edges:\n";
//	//for (auto e : make_iterator_range(edges(G))) {
//	//	int cap = get(edge_capacity, e);
//	//	if (true) {//if (cap != 0) {
//	//		std::cout << "(" << source(e, G) << ", " << target(e, G) << ") w:" << get(edge_weight, G, e) << ", cap:" << get(edge_capacity, e) << ", f:" << edge_capacity[e] - ResidualCapacityEdgeMap[e] << "\n"; // 
//	//	}
//	//}
//
//	// Find min_cost maxflow on graph G
//	successive_shortest_path_nonnegative_weights(G, s, t);
//	//push_relabel_max_flow(G, s, t);
//	//edmonds_karp_max_flow(G, s, t);
//	//cycle_canceling(G);
//
//
//	//// Find edges of flow=1 (these are part of the Matching);
//	//std::vector<int> Wtouch(num_vertices(g), 0);
//	//for (Vertex u : U) {
//	//	int numtouchs = 0;
//	//	for (auto e : make_iterator_range(out_edges(u, G))) {
//	//		if (get(edge_capacity, e) != 0) {
//	//			int flow = get(edge_capacity, e) - ResidualCapacityEdgeMap[e];
//	//			if (flow == 1) {
//	//				Vertex w = target(e, G);
//	//				mate[u] = w;
//	//				mate[w] = u;
//	//				Wtouch[w] = 1;
//	//				numtouchs++;
//	//			}
//	//		}
//	//	}
//
//	//	if (numtouchs == 0) {
//	//		mate[u] = boost::graph_traits<Graph>::null_vertex();
//	//	}
//	//}
//
//	//for (Vertex w : W) {
//	//	if (Wtouch[w] == 0) {
//	//		mate[w]  = boost::graph_traits<Graph>::null_vertex();
//	//	}
//	//}
//
//	std::vector<int> touched(num_vertices(g), 0);
//	for (auto e : make_iterator_range(edges(G))) {
//		Vertex u = source(e, G);
//		Vertex w = target(e, G);
//		int flow = get(edge_capacity, e) - ResidualCapacityEdgeMap[e];
//		
//		if ((u != s) && (u != t) && (w != s) && (w != t) && (get(edge_capacity, e) != 0) && (flow == 1)) {
//			mate[u] = w;
//			mate[w] = u;
//			touched[u] = 1;
//			touched[w] = 1;
//		}
//	}
//
//	for (Vertex v : make_iterator_range(vertices(g))) {
//		if (touched[v] == 0) {
//			mate[v] = boost::graph_traits<Graph>::null_vertex();
//		}
//	}
//
////  std::cout << "Printing nodes:\n";
////  for (auto v : make_iterator_range(vertices(G))) {
////	  std::cout<< v << "\n";
////  }
//////
// // std::cout << "Printing edges:\n";
// // for (auto e : make_iterator_range(edges(G))) {
//	//  int cap = get(edge_capacity, e);
//	//  if(true){//if (cap != 0) {
//	//	  std::cout << "(" << source(e, G) << ", " << target(e, G) << ") w:" << get(edge_weight, G, e) << ", cap:" << get(edge_capacity, e) << ", f:" << edge_capacity[e] - ResidualCapacityEdgeMap[e] << "\n"; // 
//	//  }
//	//}


  return mate;
}


// Vertex v0 = add_vertex(G);
//  Vertex v1 = add_vertex(G);
//  Vertex v2 = add_vertex(G);
//  Vertex v3 = add_vertex(G);
// 
//  auto e0 = add_edge(v0, v1, 2, G);
//  auto e1 = add_edge(v0, v2, 1, G);
//  auto e2 = add_edge(v1, v3, 1, G);
//  auto e3 = add_edge(v2, v3, 3, G);
//  auto e4 = add_edge(v1, v2, 3, G);
//
//  auto r0 = add_edge(v1, v0, 2, G);
//  auto r1 = add_edge(v2, v0, 1, G);
//  auto r2 = add_edge(v3, v1, 1, G);
//  auto r3 = add_edge(v3, v2, 3, G);
//
//  edge_reverse[e0.first] = r0.first;
//  edge_reverse[r0.first] = e0.first;
//
//  edge_reverse[e1.first] = r1.first;
//  edge_reverse[r1.first] = e1.first;
//
//  edge_reverse[e2.first] = r2.first;
//  edge_reverse[r2.first] = e2.first;
//
//  edge_reverse[e3.first] = r3.first;
//  edge_reverse[r3.first] = e3.first;
//
//
//  edge_capacity[e0.first] = 5;
//  edge_capacity[e1.first] = 3;
//  edge_capacity[e2.first] = 2;
//  edge_capacity[e3.first] = 4;
//
//  edge_capacity[r0.first] = 0;
//  edge_capacity[r1.first] = 0;
//  edge_capacity[r2.first] = 0;
//  edge_capacity[r3.first] = 0;
//
//
//
//// Reverse Capacity
//  ResidualCapacityEdgeMap[e0.first] = 5;
//  ResidualCapacityEdgeMap[e1.first] = 3;
//  ResidualCapacityEdgeMap[e2.first] = 2;
//  ResidualCapacityEdgeMap[e3.first] = 4;
//
//  ResidualCapacityEdgeMap[r0.first] = 0;
//  ResidualCapacityEdgeMap[r1.first] = 0;
//  ResidualCapacityEdgeMap[r2.first] = 0;
//  ResidualCapacityEdgeMap[r3.first] = 0;
