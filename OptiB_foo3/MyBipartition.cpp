#include "MyBipartition.h"


Partition_variant my_bipartition(const Graph &g) {
  Partition_variant res;
  std::pair<std::vector<Vertex>, std::vector<Vertex>> myPartition;
  std::vector<Vertex> odd_cycle;
  bool bipartite{true};

  // TODO compute bipartition




  //return
  if (bipartite) {
    res = myPartition;
  } else {
    res = odd_cycle;
  }
  //------------------------

  return res;
}
