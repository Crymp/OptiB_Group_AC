// OptiB_foo3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
//

#include <iostream>
#include <string>

#include "FootballBettingGameParser.h"
#include "MyBipartition.h"
#include "MyFootballBettingGame.h"
#include "MyMaxFlow.h"
#include "MyMaxWeightedMatching.h"
#include "check.h"
#include "randomVertex.h"
#include "readLgf.h"
#include "types.h"

/**
 *
 *
 * @param args args[0] is the number of the exercise (decides the algorithm
 * which is to be executed) args[1] is the file to parse
 */
int main(int argc, char** argv) {

    std::cout << "Which algorithm would you like to test "
        "(MF, B, BM, F)?"
        << std::endl;

    std::string arg0;
    //std::cin >> arg0;
    arg0 = "BM";

    if (arg0 == "B") {
        std::cout << "Which instance would you like to solve (1,2,3,4)?"
            << std::endl;
        std::string arg1;
        std::cin >> arg1;

        std::cout << "Computing Bipartitioning of Graph " << arg1 << ":"
            << std::endl;

        Graph g{ readGraphFromFile("graph" + arg1 + ".lgf") };

        // compute bipartition
        Partition_variant res{ my_bipartition(g) };
        checkBiPartition(g, res);

    }
    else if (arg0 == "BM") {

        std::cout << "Which instance would you like to solve (1,2,3,4)?"
            << std::endl;
        std::string arg1;
        //std::cin >> arg1;
        arg1 = "1";
        std::cout << "Computing Bipartite Matching of Graph " << arg1 << ":"
            << std::endl;
        Graph g{ readGraphFromFile("graph" + arg1 + ".lgf") };

        // compute matching
        std::vector<Vertex> myMatching{ my_max_weighted_matching(g) };
        checkMaxWeightedMatching(g, myMatching);

    }
    else if (arg0 == "F") {
        std::cout << "Computing Optimal Betting:" << std::endl;

        std::string filename = "Season12.txt";
        std::vector<Matchday> season{ read_football_instance(filename) };
        std::map<Team, unsigned int> myBet{ my_football_betting(season) };

        checkBettingGame(myBet, season);

    }
    else if (arg0 == "MF") {

        std::cout << "Which instance would you like to solve (1,2,3,4)?"
            << std::endl;
        std::string arg1;
        std::cin >> arg1;
        std::cout << "Computing Maximum Flow of Graph " << arg1 << ":" << std::endl;
        std::cout << "Computing Maximum Flow of Graph " << arg1 << ":" << std::endl;
        DiGraph g{ readDiGraphFromFile("graph" + arg1 + ".lgf") };
        // TODO DiGraph has no deep copy constructor
        DiGraph gOrg{ readDiGraphFromFile("graph" + arg1 + ".lgf") };

        // generate random source and target vertex
        std::pair<DiVertex, DiVertex> stVertices{ get_tuple_of_distinct_vertices(g) };

        // compute maxflow
        unsigned int flow{ my_maxflow(g, stVertices.first, stVertices.second) };

        // check
        checkMaxFlow(g, gOrg, stVertices.first, stVertices.second, flow);
    }
    return 0;
}





//#include "check.h"
//#include "randomVertex.h"
//#include "readLgf.h"
//#include "types.h"
//
//#include "myShortestPath.h"
//#include "mySpanningTree.h"
//#include "mySteinerTree.h"
//
//#include <iostream>
//#include <string>
//
//int main() {
//    std::cout << "Which algorithm would you like to test "
//        "(MST,ShortestPath,SteinerTree)?"
//        << std::endl;
//    std::string algorithm;
//    std::cin >> algorithm;
//    //algorithm = "ShortestPath";
//    //algorithm = "SteinerTree";
//    std::cout << "Which instance would you like to solve (1,2,3,4,5,6)?"
//        << std::endl;
//    std::string graphNumber;
//    std::cin >> graphNumber;
//
//    Graph g{ readGraphFromFile("Graph" + graphNumber + ".lgf") };
//
//    if (algorithm == "MST") {
//        std::cout << "Computing " + algorithm + " of Graph " + graphNumber + ":"
//            << std::endl;
//        // compute a minimum spanning tree
//        std::vector<Edge> spanning_tree{ my_spanning_tree(g) };
//        checkSpanningTree(g, spanning_tree);
//    }
//
//    if (algorithm == "ShortestPath") {
//        std::cout << "Computing " + algorithm + " of Graph " + graphNumber + ":"
//            << std::endl;
//        // generate random start and end vertex
//        std::pair<Vertex, Vertex> stVertices{ get_tuple_of_distinct_vertices(g) };
//        // compute a shortest path
//        std::vector<Vertex> path{
//            my_shortest_path(g, stVertices.first, stVertices.second) };
//        // check if the computed path is a shortest path
//        checkPath(g, stVertices.first, stVertices.second, path);
//    }
//
//    if (algorithm == "SteinerTree") {
//        // read terminal nodes from file
//        std::vector<Vertex> terminals;
//        if (graphNumber == "4") {
//            std::cout
//                << "Which set of terminal nodes do you want to choose (1,2,3,4)?"
//                << std::endl;
//            std::string t;
//            std::cin >> t;
//            std::cout << "Computing " + algorithm + " of Graph " + graphNumber +
//                " with Terminals" + t + ":"
//                << std::endl;
//            terminals = readTerminalsFromFile(
//                "Graph" + graphNumber + "_Terminals" + t + ".txt", g);
//        }
//        else {
//            if (graphNumber == "5") {
//                std::cout
//                    << "Which set of terminal nodes do you want to choose (1,2,3)?"
//                    << std::endl;
//                std::string t;
//                std::cin >> t;
//                std::cout << "Computing " + algorithm + " of Graph " + graphNumber +
//                    " with Terminals" + t + ":"
//                    << std::endl;
//                terminals = readTerminalsFromFile(
//                    "Graph" + graphNumber + "_Terminals" + t + ".txt", g);
//            }
//            else {
//                std::cout << "Computing " + algorithm + " of Graph " + graphNumber + ":"
//                    << std::endl;
//                terminals =
//                    readTerminalsFromFile("Graph" + graphNumber + "_Terminals.txt", g);
//            }
//        }
//
//        // compute a steiner tree for g with terminals g
//        std::vector<Edge> steiner_tree{ my_steiner_tree(g, terminals) };
//        // check computed steiner tree
//        checkSteinerTree(g, steiner_tree, terminals);
//    }
//    g.clear();
//
//    return 0;
//}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

