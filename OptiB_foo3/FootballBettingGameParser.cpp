#include "FootballBettingGameParser.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

std::vector<Matchday> read_football_instance(const std::string &path) {
  std::vector<Matchday> season;

  std::ifstream stream(path);

  int number = 0;
  Matchday currentMatchday;

  if (stream.is_open()) {
    std::string line;
    while (std::getline(stream, line)) {

      if (!line.empty()) {
        if (line.at(0) == '#') {
          if (number > 0) {
            season.push_back(currentMatchday);
            currentMatchday.clear();
          }
          number++;
        } else {
          std::vector<std::string> items;
          // split line at ','
          size_t pos = 0;
          std::string token;
          while ((pos = line.find(',')) != std::string::npos) {
            token = line.substr(0, pos);
            items.push_back(token);
            line.erase(0, pos + 1);
          }
          token = line.substr(0, pos);
          items.push_back(token);
          assert(items.size() == 4);
          Team t1 = items.at(0);
          Team t2 = items.at(1);
          std::pair<Team, Team> teams = std::make_pair<Team &, Team &>(t1, t2);
          int s1 = std::stoi(items.at(2));
          int s2 = std::stoi(items.at(3));
          Score s = std::make_pair<int &, int &>(s1, s2);
          Match currentMatch = std::make_pair(teams, s);
          currentMatchday.push_back(currentMatch);
        }
      }
    }
    season.push_back(currentMatchday);
  } else {
    std::cout << "Unable to open file!";
  }

  return season;
}
