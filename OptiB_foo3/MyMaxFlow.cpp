#include "MyMaxFlow.h"


#include <boost/property_map/property_map.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <iostream>
#include <unordered_map>
#include <limits>
#include <algorithm>

using namespace boost;

unsigned int my_maxflow(DiGraph& g, const DiVertex& s, const DiVertex& t) {

    //save flow in variable flow
    unsigned int flow = 0;

    EdgeCapacityMap capacity{get(boost::edge_capacity, g) };
    EdgeResidualCapacityMap res_capacity{ get(boost::edge_residual_capacity, g) };
    EdgeReverseMap rev{ get(boost::edge_reverse, g) };
    //Flow is implicitly given as capacity - res_capacity

    //Edge weight map for dijkstra (initialized with 0)
    EdgeWeightMap weight{ get(boost::edge_weight, g) };
    //vector with predecessors (for dijkstra)
    std::vector<DiVertex> preds(num_vertices(g));
    //vector with distances (for dijkstra)
    std::vector<unsigned int> dists(num_vertices(g));

    //TODO implement Max Flow Algorithm

    // Lookup table for vertex endpoints to the edge object.... fed up trying to figure out how to do this properly with boost
    // C++ is so "high level", that one is at a level so high that the only way to see anything is to drop into the mess of low level
    unordered_map<std::pair<Vertex, Vertex>, detail::edge_desc_impl<directed_tag, size_t>> Endpts2Edge;    
    size_t max_cap = 0;
    size_t num_edges = 0;
    for (detail::edge_desc_impl<directed_tag, size_t> e : make_iterator_range(edges(g))) {
       // std::cout << e << " : " << get(weight, e);
        Endpts2Edge[std::pair < Vertex, Vertex>(source(e, g), target(e, g))] = e;


       // Also initialize values of weights and such
        std::cout << e << ": " << get(capacity, e) << ", " << get(res_capacity, e) << "\n";
            // get(res_capacity, e) = get(capacity, e);
        max_cap += get(capacity, e);
        num_edges++
    }		

    size_t max_num = 
    std::cout << "max_cap: " << max_cap << ", num_edges: " << num_edges << "... overflow ratio" <<  << "\n";


    std::cout << "source: " << s << ", target: " << t << std::endl;

//    dists[t] = 0;
//    while (dists[t] == 0) {
//        for (auto d : dists) {
//            std::cout << d << ", ";
//        }
//        std::cout << "\n";
//        
//        //update edge weights as "residual capacity"
//        for (auto e : make_iterator_range(edges(g))) {
//            get(weight, e) = get(res_capacity, e);
//            auto w = get(weight, e);
//
//        }
//        
//        // Find path with shortest path from s -> t
//        boost::dijkstra_shortest_paths(g, s, predecessor_map(boost::make_iterator_property_map(preds.begin(), get(boost::vertex_index, g))).distance_map(boost::make_iterator_property_map(dists.begin(), get(boost::vertex_index, g))));
//#
//        // Find minimum residual capacity in path from s -> t
//        Vertex node = t;
//        Vertex pred;
//        size_t min_res_cap = std::numeric_limits<int>::max();
//        while (node != s) {
//            pred = preds[node];
//            detail::edge_desc_impl<directed_tag, size_t> edge = Endpts2Edge[std::pair<Vertex, Vertex>(pred, node)];
//            std::cout << edge << "\n";
//            size_t res_cap = get(res_capacity, edge);
//
//            min_res_cap = std::min(min_res_cap, res_cap);
//            node = pred;
//        }
//      
//        // Adjust residual capacities along path s -> t
//        node = t;
//        while (node != s) {
//            pred = preds[node];
//            detail::edge_desc_impl<directed_tag, size_t> edge = Endpts2Edge[std::pair<Vertex, Vertex>(pred, node)];
//            auto rev_edge = get(rev, edge);
//            get(res_capacity, edge) -=  min_res_cap;
//            get(res_capacity, rev_edge) += min_res_cap;
//            node = pred;
//        }
//        flow += min_res_cap;
//    }
//
//  return flow;
//}





//// Getting the reverse edge for each
//for (auto e : make_iterator_range(edges(g))) {
//    std::cout << e << " - " << get(rev, e) << "\n";
//}


//// Check if each arc has a reverse arc in g
//bool masterflag = true;
//for (auto e : make_iterator_range(edges(g))) {
//    //std::cout << e << ", ";
//    get(weight, e) = 10;
//    Vertex v1 = source(e,g);
//    Vertex v2 = target(e, g);

//    // Loop through all outgoing edges from a node 
//    bool flag = false;
//    for (auto ee : make_iterator_range(out_edges(v2, g))) {
//        flag = flag || (target(ee,g) == v1);
//    }
//    masterflag = masterflag && flag;
//    //std::cout << e << ": " << flag <<"\n";
//}
//std::cout << "Does each arc have a reverse arc in g? " << masterflag << "\n";










//unsigned int my_maxflow(DiGraph &g, const DiVertex &s, const DiVertex &t) {
//
//  //save flow in variable flow
//  unsigned int flow = 0;
//
//  EdgeCapacityMap capacity{get(boost::edge_capacity, g)};
//  EdgeResidualCapacityMap res_capacity{get(boost::edge_residual_capacity, g)};
//  EdgeReverseMap rev{get(boost::edge_reverse, g)};
//  //Flow is implicitly given as capacity - res_capacity
//
//  //Edge weight map for dijkstra (initialized with 0)
//  EdgeWeightMap weight{get(boost::edge_weight, g)};
//  //vector with predecessors (for dijkstra)
//  std::vector<DiVertex> p(num_vertices(g));
//  //vector with distances (for dijkstra)
//  std::vector<unsigned int> d(num_vertices(g));
//
//  //TODO implement Max Flow Algorithm
//
//
//
//  //example call for dijkstra
//  boost::dijkstra_shortest_paths(g, s, predecessor_map(boost::make_iterator_property_map(p.begin(), get(boost::vertex_index, g))).distance_map(boost::make_iterator_property_map(d.begin(), get(boost::vertex_index, g))));
//
//  std::cout << "target id is " << t << " with distance " << d[t] << std::endl;
//  std::cout << "source is " << s << std::endl;
//  std::cout << "dijsktras distances are \n";
//  for (auto x : d) {
//      std::cout << x << ", ";
//  }
//  std::cout << "\n\n";
//
//  std::cout << "Path backwards is: ";
//  
//  DiVertex pred = p[t];
//  
//  int i = 0;
//  while ((pred != s) && (i <= 50)) {
//      i++;
//      pred = p[pred];
//      std::cout << pred << ", ";
//  }
//  std::cout << std::endl;
//
//
//  /*
//  //example call for dijkstra
//  boost::dijkstra_shortest_paths(g, 0,
//  predecessor_map(boost::make_iterator_property_map(
//                            p.begin(), get(boost::vertex_index, g)))
//            .distance_map(boost::make_iterator_property_map(
//                d.begin(), get(boost::vertex_index, g))));
//  */
//
//
//  return flow;
//}
