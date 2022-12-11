#include "readLgf.h"

#include <fstream>

// Convert edgeline to EdgeWithWeight
EdgeWithWeight convertEdgeline(const std::string &input) {
  std::stringstream ss(input);
  int node1, node2, w;
  ss >> node1;
  ss >> node2;
  ss >> w;
  assert(node1 >= 1 && node2 >= 1);
  return std::make_pair(
      std::make_pair((unsigned int)(node1 - 1), (unsigned int)(node2 - 1)), w);
}

// Read Graph from file in lemongraph format (.lgf)
Graph readGraphFromFile(const std::string &filename) {
  std::ifstream ifs;
  ifs.open(filename, std::ifstream::in);
  std::string line;
  for (unsigned int i = 0; i < 4; ++i) {
    std::getline(ifs, line);
  }
  Graph g;
  std::getline(ifs, line);
  while (not line.empty()) {
    boost::add_vertex(g);
    std::getline(ifs, line);
  }

  for (unsigned int i = 0; i < 2; ++i) {
    std::getline(ifs, line);
  }

  std::getline(ifs, line);
  EdgeWithWeight edgew;
  while (ifs && (not line.empty())) {
    edgew = convertEdgeline(line);
    add_edge(edgew.first.first, edgew.first.second,
             EdgeWeightProperty(edgew.second), g);
    std::getline(ifs, line);
  }

  ifs.close();

  return g;
}

// Read DiGraph from file in lemongraph format (.lgf)
DiGraph readDiGraphFromFile(const std::string &filename) {
  std::ifstream ifs;
  ifs.open(filename, std::ifstream::in);
  std::string line;
  for (unsigned int i = 0; i < 4; ++i) {
    std::getline(ifs, line);
  }
  DiGraph g;
  EdgeCapacityMap capacity = get(boost::edge_capacity, g);
  EdgeResidualCapacityMap residual_capacity = get(boost::edge_residual_capacity, g);
  EdgeReverseMap rev = get(boost::edge_reverse, g);
  std::getline(ifs, line);
  while (not line.empty()) {
    boost::add_vertex(g);
    std::getline(ifs, line);
  }

  for (unsigned int i = 0; i < 2; ++i) {
    std::getline(ifs, line);
  }

  std::getline(ifs, line);
  EdgeWithWeight edgew;
  Arc e1, e2;
  while (ifs && (not line.empty())) {
    edgew = convertEdgeline(line);
    boost::add_edge(edgew.first.first, edgew.first.second, g);
    boost::add_edge(edgew.first.second, edgew.first.first, g);
    e1 = edge(edgew.first.first, edgew.first.second, g).first;
    e2 = edge(edgew.first.second, edgew.first.first, g).first;
    capacity[e1] = edgew.second;
    capacity[e2] = edgew.second;
    residual_capacity[e1] = 0;
    residual_capacity[e2] = 0;
    rev[e1] = e2;
    rev[e2] = e1;
    std::getline(ifs, line);
  }

  ifs.close();

  return g;
}

// Read Terminal Nodes for Steiner Tree from file
std::vector<Vertex> readTerminalsFromFile(const std::string &filename,
                                          const Graph &g) {
  std::ifstream ifs;
  ifs.open(filename, std::ifstream::in);
  std::string line;
  std::vector<Vertex> terminals;
  int nodeLabel;
  std::getline(ifs, line);
  std::pair<VertexIterator, VertexIterator> vertices = boost::vertices(g);
  while (not line.empty()) {
    nodeLabel = std::stoi(line) - 1;
    Vertex v = *(vertices.first + nodeLabel);
    terminals.push_back(v);
    std::getline(ifs, line);
  }
  return terminals;
}
