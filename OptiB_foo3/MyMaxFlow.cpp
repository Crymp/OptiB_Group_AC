#include "MyMaxFlow.h"


#include <boost/property_map/property_map.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <iostream>
//#include <unordered_map>
#include <limits>
#include <algorithm>

//#include <iomanip>
//#include <string>
//#include <cmath>
//#include <cstdlib>


using namespace boost;




unsigned int my_maxflow(DiGraph& g, const DiVertex& s, const DiVertex& t) {
    // Note: As suggested by the code skeleton, we use Dijktra's algorithm to find the (shortest) paths of non-zero residual capacity from s to t;
    // As such, in each iteration of pushing flow through a new path, the weights are set to the residual capacity for each edge;
    // But to ensure that Dijktra's algorithm doesn't find paths involving an edge of zero residual capacity:
    // if an edge's residual capacity is zero, its weight will not be initialized as zero, but to a very large value.
    // This value should be larger than the total capacity of any path that is valid.
    // The upper bound to this is sum of capacities over all edges in g, which we call "max_val"
    // We know when there are no paths left where every edge has a non-zero capacity, when the total distance from s to t is larger than max_val.
    // (since it means at least one edge has a residual capacity of 0, ie a weight of max_val).
    // 
    // This is a bit of a dirty trick, since it can result in overflow errors for unsigned int. But luckily it works for all graphs given here.
    // For a more general implementation beyond, one should either use a specific algorithm to find paths of non-zero capacities (e.g. modified dijkstras).
    // or modify the graph itself by e.g. removing edges once saturated.

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


    // Initialize residual capacities and compute max_val (see above)
    unsigned int max_val = 1;
    for (auto e : make_iterator_range(edges(g))) {
        get(res_capacity, e) = get(capacity, e);
        max_val += get(capacity, e);
    }


    // Loop until can no longer push any extra flow (break conditions built inside)
    while (true) {

        // Initialize weights as residual capacities* (see comment above about max_val)
        for (auto e : make_iterator_range(edges(g))) {
            unsigned int res_cap = get(res_capacity, e);
            // max_val indicates that an edge is satured with flow.
            get(weight, e) = (res_cap == 0) ? max_val : res_cap;
        }

        // Find shorted path of residual_capacities from s to t
        boost::dijkstra_shortest_paths(g, s,
            predecessor_map(boost::make_iterator_property_map(preds.begin(), get(boost::vertex_index, g))).distance_map(boost::make_iterator_property_map(dists.begin(),
            get(boost::vertex_index, g))));
      
        // Terminate if path involves a saturated edge (if true for shortest, this will be true for all,... see beginning comment)
        if (dists[t] >= max_val) {
            break;
        }

        // Find minimum capacity along path from s->t (backwards)
        unsigned int  min_res_cap = std::numeric_limits<unsigned int>::max();
        Vertex node = t;
        Vertex pred;
        while (node != s) {
            pred = preds[node];

            // Find edge object that matches pred->node
            for (auto e : make_iterator_range(out_edges(pred, g))) {
                if (target(e, g) == node) {
                    unsigned int res_cap = get(res_capacity, e);
                    min_res_cap = std::min(min_res_cap, res_cap);
                }
            }
            node = pred;
        }

        ////std::cout << "min_res_cap: " << min_res_cap << "\n";
        //if (min_res_cap == 0) {
        //    break;
        //}

        // Update flow / residual capacities along path from s->t (backwards)
        node = t;
        while (node != s) {
            pred = preds[node];
            for (auto e : make_iterator_range(out_edges(pred, g))) {
                
                // Find edge that matches pred->node
                if (target(e, g) == node) {
                    get(res_capacity, e) -= min_res_cap;
                    
                    // Update flow in reverse direction
                    auto rev_e = get(rev, e);
                    get(res_capacity, rev_e) += min_res_cap;
                }
            }
            node = pred;
        }
        flow += min_res_cap;
    }
    return flow;
}


    // Lookup table for vertex endpoints to the edge object.... fed up trying to figure out how to do this properly with boost
    // C++ is so "high level", that one is at a level so high that the only way to see anything is to drop into the mess of low level
    //unordered_map<std::pair<Vertex, Vertex>, detail::edge_desc_impl<directed_tag, size_t>> Endpts2Edge;    
    //size_t tot_cap = 0;
    //size_t num_edges = 0;
    //for (detail::edge_desc_impl<directed_tag, size_t> e : make_iterator_range(edges(g))) {
    //   // std::cout << e << " : " << get(weight, e);
    //    Endpts2Edge[std::pair < Vertex, Vertex>(source(e, g), target(e, g))] = e;


    //   // Also initialize values of weights and such
    //    std::cout << e << ": " << get(capacity, e) << ", " << get(res_capacity, e) << "  --  ";// << "\n";
    //    auto re = get(rev, e);
    //    //std::cout << re << ": " << get(res_capacity, re) << "\n";
    //    if (source(e, g) == target(e, g)) {
    //        std::cout << "alarm! " << e << "\n";
    //    }
    //    get(res_capacity, e) = get(capacity, e);
    //    tot_cap += get(capacity, e);
    //    num_edges++;
    //}		

    //std::cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa\n";
    //for (auto e : make_iterator_range(edges(g))) {
    //    //auto re = get(rev, e);
    //    auto re = Endpts2Edge[std::pair<Vertex, Vertex>(target(e, g), source(e, g))];
    //    auto foo = get(res_capacity, e);
    //    auto bar = get(res_capacity, re);
    //    std::cout << e << ": " << foo << ", " << bar << std::endl;
    //}

    // foo
    //size_t max_num = std::numeric_limits<unsigned int>::max();
    //std::cout << max_num << std::endl;
    //std::cout << tot_cap * num_edges << std::endl;
    //std::cout << "max_cap: " << tot_cap << ", num_edges: " << num_edges << ", max:" << tot_cap*num_edges << "... overflow? " << (tot_cap * num_edges >= max_num) << "\n";
    //std::cout << "source: " << s << ", target: " << t << std::endl;
    //unsigned int max_cap = tot_cap;

    //dists[t] = 0;
    //size_t i = 0;
    //while (dists[t] <= max_cap) {
    //    for (auto d : dists) {
    //        std::cout << d << ", ";
      //  }
    //    
    //    //update edge weights as "residual capacity"
    //    for (auto e : make_iterator_range(edges(g))) {
    //        unsigned int w = get(res_capacity, e);
    //        w = (w == 0) ? max_cap : w;
    //        get(weight, e) = w;
    //    }
    //    
    //    // Find path with shortest path from s -> t
    //    boost::dijkstra_shortest_paths(g, s, predecessor_map(boost::make_iterator_property_map(preds.begin(), get(boost::vertex_index, g))).distance_map(boost::make_iterator_property_map(dists.begin(), get(boost::vertex_index, g))));
    //    // Find minimum residual capacity in path from s -> t
    //    Vertex node = t;
    //    Vertex pred;
    //    unsigned int min_res_cap = std::numeric_limits<int>::max();
    //    while (node != s) {
    //        pred = preds[node];
    //        detail::edge_desc_impl<directed_tag, size_t> edge = Endpts2Edge[std::pair<Vertex, Vertex>(pred, node)];
    //        //std::cout << edge << "\n";
    //        unsigned int res_cap = get(res_capacity, edge);
    //        min_res_cap = std::min(min_res_cap, res_cap);
    //        node = pred;
    //    }
    //  
    //    // Adjust residual capacities along path s -> t
    //    node = t;
    //    while (node != s) {
    //        pred = preds[node];
    //        detail::edge_desc_impl<directed_tag, size_t> edge = Endpts2Edge[std::pair<Vertex, Vertex>(pred, node)];
    //        auto rev_edge = get(rev, edge);
    //        /*std::cout << edge << " : " << rev_edge << "\n";*/
    //        //std::cout << get(res_capacity, edge) << " : " << get(res_capacity, rev_edge) << "\n";
    //        get(res_capacity, edge) -=  min_res_cap;
    //        get(res_capacity, rev_edge) += min_res_cap;
    //        node = pred;
    //    }
    //    flow += min_res_cap;
    //    i++;
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
