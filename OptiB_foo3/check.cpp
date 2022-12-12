#include "check.h"

#include <iostream>

#include <boost/graph/bipartite.hpp>
#include <boost/graph/boykov_kolmogorov_max_flow.hpp>
#include <boost/graph/connected_components.hpp>
//#include "maximum_weighted_matching.hpp"
#include <boost/graph/maximum_weighted_matching.hpp>

bool checkIndependence(const std::vector<Vertex> &p, const Graph &g) {
  for (std::vector<Vertex>::const_iterator vi = p.begin(); vi != p.end();
       ++vi) {
    for (std::vector<Vertex>::const_iterator wi = vi; wi != p.end(); ++wi) {
      if (boost::edge(*vi, *wi, g).second) {
        std::cout
            << "There exists at least one edge within a bipartition class."
            << std::endl;
        return false;
      }
    }
  }
  return true;
}

bool checkBiPartition(const Graph &g, const Partition_variant &res) {

  bool bipartite = is_bipartite(g);
  std::cout << "Boost bipartite: " << bipartite << std::endl;
  bool isBiPartition = res.which() == 0;
  if (!(bipartite == isBiPartition)) {
    return false;
  }
  if (isBiPartition) {
    std::vector<Vertex> myPartitionA(boost::get<std::pair<std::vector<Vertex>, std::vector<Vertex>>>(res).first);
    std::vector<Vertex> myPartitionB(boost::get<std::pair<std::vector<Vertex>, std::vector<Vertex>>>(res).second);
    if (myPartitionA.size() + myPartitionB.size() != boost::num_vertices(g)) {
      std::cout << "The partitioning is invalid." << std::endl;
      return false;
    }
    checkIndependence(myPartitionA, g);
    checkIndependence(myPartitionB, g);
    std::cout << "The computed bipartitioning is valid." << std::endl;
  } else {
    std::vector<Vertex> myOddCycle(boost::get<std::vector<Vertex>>(res));
    if (myOddCycle.front() != myOddCycle.back()) {
      std::cout << "The computed cycle should be simple and start and end with "
                   "the same vertex."
                << std::endl;
      return false;
    }
    if (myOddCycle.size() % 2 != 0) {
      std::cout << "The computed cycle is not of odd length." << std::endl;
      return false;
    }
    for (std::vector<Vertex>::const_iterator vi = myOddCycle.begin();
         vi != myOddCycle.end() - 1; ++vi) {
      if (!boost::edge(*vi, *(vi + 1), g).second) {
        std::cout << "The cycle uses edges that do not exist." << std::endl;
        return false;
      }
    }
    std::cout << "The computed odd cycle is valid." << std::endl;
  }

  return true;
}

bool checkMaxWeightedMatching(const Graph &g, const std::vector<Vertex> &mate) {

    std::cout << "Boost Bipartite?: "<< is_bipartite(g) << "\n";


  VertexIterator vi, vi_end;

  // check matching
  for (boost::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
    if (mate[*vi] != boost::graph_traits<Graph>::null_vertex()) {
      // check if all chosen edges exist in g
      if (!edge(*vi, mate[*vi], g).second) {
        std::cout << "The computed matching is invalid: Does not exist in g: (" << *vi << ", " << mate[*vi] << ")" << std::endl;
        return false;
      }
      // check that no chosen edges are incident
      if (mate[mate[*vi]] != *vi) {
        std::cout << "The computed matching is invalid: incident edges" << std::endl;
        return false;
      }
    }
  }

  // check optimality
  std::vector<Vertex> optMate(num_vertices(g));
  maximum_weighted_matching(g, &optMate[0]);
  int optWeight = matching_weight_sum(g, &optMate[0]);
  int weight = matching_weight_sum(g, &mate[0]);
  if (weight < optWeight) {
    std::cout << "The computed matching has not maximum weight:" <<  weight << " < " << optWeight << std::endl;
  }
  //assert(weight == optWeight);
  else {
      std::cout << "The computed matching has maximum weight " << weight << "."
          << std::endl;
      return true;
  }
  return false;
}

// bool checkBettingGame(const std::map<Team, unsigned int> &bet, const
// std::vector<Matchday> &season){
bool checkBettingGame(const std::map<Team, unsigned int> &bet,
                      const std::vector<Matchday> &season) {
  if (bet.size() != 17) {
    std::cout << "The betting does not choose 17 teams." << std::endl;
    return false;
  }
  unsigned int points{0};
  for (std::map<Team, unsigned int>::const_iterator ki = bet.begin();
       ki != bet.end(); ++ki) {
    Team team{(*ki).first};
    unsigned int day{(*ki).second};
    if (bet.count(team) != 1) {
      std::cout << "The betting does not choose 17 different teams."
                << std::endl;
      return false;
    }
    Matchday matchday{season.at(day)};
    for (std::vector<Match>::const_iterator mi = matchday.begin();
         mi != matchday.end(); ++mi) {
      Score score = (*mi).second;
      if (team == (*mi).first.first) {
        if (score.first > score.second) {
          points += 3;
          break;
        } else if (score.first == score.second) {
          points += 1;
          break;
        }
      } else if (team == (*mi).first.second) {
        if (score.first < score.second) {
          points += 3;
          break;
        } else if (score.first == score.second) {
          points += 1;
          break;
        }
      }
    }
  }
  if (points != 51) {
    std::cout << "The betting is valid, but not optimal." << std::endl;
    return false;
  }
  std::cout << "The betting is valid with an optimal value of 51." << std::endl;
  return true;
}

bool equalGraph(DiGraph &g, DiGraph &gOrg) {
  EdgeCapacityMap capacity = get(boost::edge_capacity, g);
  EdgeCapacityMap capacityOrg{get(boost::edge_capacity, gOrg)};
  if (num_vertices(g) != num_vertices(gOrg))
    return false;
  if (num_edges(g) != num_edges(gOrg))
    return false;
  ArcIterator e, e_end;
  for (boost::tie(e, e_end) = edges(g); e != e_end; ++e) {
    if (not edge(source(*e, g), target(*e, g), gOrg).second)
      return false;
    if (capacity[*e] !=
        capacityOrg[edge(source(*e, g), target(*e, g), gOrg).first])
      return false;
  }
  return true;
}

bool checkMaxFlow(DiGraph &g, DiGraph &gOrg, const DiVertex &s,
                  const DiVertex &t, unsigned int flow) {
  if (not equalGraph(g, gOrg)) {
    std::cout << "The two graphs don't match." << std::endl;
    return false;
  }
  unsigned int boostFlow{boost::boykov_kolmogorov_max_flow(gOrg, s, t)};
  std::cout << "Boost flow = " << boostFlow <<std::endl;
  if (flow != boostFlow) {
    std::cout << "Your calculated flow of " << flow << " is not correct." << std::endl;
    return false;
  }
  EdgeCapacityMap capacity{get(boost::edge_capacity, g)};
  EdgeResidualCapacityMap res_capacity{get(boost::edge_residual_capacity, g)};
  EdgeReverseMap rev{get(boost::edge_reverse, g)};
  // check flows at vertices
  DiVertexIterator v, v_end;
  boost::graph_traits<DiGraph>::out_edge_iterator eo, eo_end;
  Arc r;
  unsigned int flowPos, flowNeg;
  for (boost::tie(v, v_end) = vertices(g); v != v_end; ++v) {
    flowPos = 0;
    flowNeg = 0;
    for (boost::tie(eo, eo_end) = boost::out_edges(*v, g); eo != eo_end; ++eo) {
      r = rev[*eo];
      if (capacity[*eo] >= res_capacity[*eo]) {
        flowNeg += capacity[*eo];
        flowPos += res_capacity[*eo];
      }
      if (capacity[r] >= res_capacity[r]) {
        flowPos += capacity[r];
        flowNeg += res_capacity[r];
      }
    }
    if (*v == s) {
      if (flowNeg != flowPos + flow) {
        std::cout << "Flow in s is not correct." << std::endl;
        return false;
      }
      continue;
    }
    if (*v == t) {
      if (flowNeg + flow != flowPos) {
        std::cout << "Flow in t is not correct." << std::endl;
        return false;
      }
      continue;
    }
    if (flowNeg != flowPos) {
      std::cout << "Flow conservation is not met." << std::endl;
      return false;
    }
  }
  std::cout << "The flow is a maximum flow of value " << flow << std::endl;
  return true;
}
