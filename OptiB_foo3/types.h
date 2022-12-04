#include "boost/variant.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <map>
#include <string>

typedef boost::property<boost::edge_weight_t, unsigned int> EdgeWeightProperty;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property, EdgeWeightProperty>
    Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor Edge;
typedef boost::graph_traits<Graph>::vertex_iterator VertexIterator;
typedef boost::graph_traits<Graph>::edge_iterator EdgeIterator;
typedef std::pair<std::pair<unsigned int, unsigned int>, unsigned int>
    EdgeWithWeight;
typedef boost::property_map<Graph, boost::edge_weight_t>::type
    EdgeWeightPropertyMap;
typedef boost::property_map<Graph, boost::edge_weight_t>::const_type
    constEdgeWeightPropertyMap;
typedef boost::variant<std::pair<std::vector<Vertex>, std::vector<Vertex>>,
                       std::vector<Vertex>>
    Partition_variant;

typedef std::pair<int, int> Score;
typedef std::string Team;
typedef std::pair<std::pair<Team, Team>, Score> Match;
typedef std::vector<Match> Matchday;

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>
    Traits;

typedef boost::property<
    boost::vertex_name_t, std::string,
    boost::property<
        boost::vertex_index_t, unsigned int,
        boost::property<
            boost::vertex_color_t, boost::default_color_type,
            boost::property<boost::vertex_distance_t, unsigned int,
                            boost::property<boost::vertex_predecessor_t,
                                            Traits::edge_descriptor>>>>>
    VertexProperties;
typedef boost::property<
    boost::edge_weight_t, unsigned int, boost::property<
    boost::edge_capacity_t, unsigned int,
    boost::property<
        boost::edge_residual_capacity_t, unsigned int,
        boost::property<boost::edge_reverse_t, Traits::edge_descriptor>>>>
    ArcProperties;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                              VertexProperties, ArcProperties>
    DiGraph;
typedef boost::graph_traits<DiGraph>::vertex_descriptor DiVertex;
typedef boost::graph_traits<DiGraph>::edge_descriptor Arc;
typedef boost::graph_traits<DiGraph>::vertex_iterator DiVertexIterator;
typedef boost::graph_traits<DiGraph>::edge_iterator ArcIterator;

typedef boost::property_map<DiGraph, boost::edge_weight_t>::type
    EdgeWeightMap;
typedef boost::property_map<DiGraph, boost::edge_capacity_t>::type
    EdgeCapacityMap;
typedef boost::property_map<DiGraph, boost::edge_residual_capacity_t>::type
    EdgeResidualCapacityMap;
typedef boost::property_map<DiGraph, boost::edge_reverse_t>::type
    EdgeReverseMap;
