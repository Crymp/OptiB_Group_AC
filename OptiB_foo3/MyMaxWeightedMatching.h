#include "types.h"
using namespace boost;


std::vector<Vertex> my_max_weighted_matching(const Graph &g);

typedef adjacency_list_traits < vecS, vecS, directedS > Traits;

typedef adjacency_list < vecS, vecS, directedS, no_property,
	property < edge_capacity_t, long,
	property < edge_residual_capacity_t, long,
	property < edge_reverse_t, Traits::edge_descriptor,
	property <edge_weight_t, long>
	>
	>
	> > MyGraph;
typedef property_map < MyGraph, edge_capacity_t >::type MyCapacity;
typedef property_map < MyGraph, edge_residual_capacity_t >::type MyResidualCapacity;
typedef property_map < MyGraph, edge_weight_t >::type MyWeight;
typedef property_map < MyGraph, edge_reverse_t>::type MyReversed;
typedef boost::graph_traits<MyGraph>::vertices_size_type Mysize_type;
typedef Traits::vertex_descriptor vertex_descriptor;
