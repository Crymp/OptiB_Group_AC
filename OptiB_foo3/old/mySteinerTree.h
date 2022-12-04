#include "types.h"

std::vector<Edge> my_steiner_tree(const Graph &g,
                                  const std::vector<Vertex> &terminals);

typedef std::tuple<int, Vertex, int> Nodeinfo;

std::tuple< std::vector<Vertex>, std::vector<Edge>> find_SP_to_nearest_terminal(const Graph& g, const std::unordered_map<Vertex, int> inside_Steiner, const std::unordered_map<Vertex, int> is_terminal);