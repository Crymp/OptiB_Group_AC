#include "types.h"

// Check whether the given partition is a valid bipartition or an odd cycle
bool checkBiPartition(const Graph &g, const Partition_variant &res);

// Check whether the given matching is a valid maximum weighted matching
bool checkMaxWeightedMatching(const Graph &g,
                              const std::vector<Vertex> &matching);

// Check betting game bet
// bool checkBettingGame(const std::map<Team, unsigned int> &bet, const
// std::vector<Matchday> &season);
bool checkBettingGame(const std::map<Team, unsigned int> &bet,
                      const std::vector<Matchday> &season);

bool checkMaxFlow(DiGraph &g, DiGraph &gOrg, const DiVertex &s,
                  const DiVertex &t, unsigned int flow);
