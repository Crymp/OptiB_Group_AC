#include "types.h"

std::vector<Vertex> my_shortest_path(const Graph &g, const Vertex &startVertex,
                                     const Vertex &endVertex);

typedef std::tuple<int, Vertex, int> Nodeinfo;