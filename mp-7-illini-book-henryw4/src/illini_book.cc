#include "illini_book.hpp"

#include <fstream>
#include <queue>

// Your code here!

std::vector<int> IlliniBook::GetUINS() const { return uins_; }
std::map<int, std::vector<std::pair<int, std::string>>>
IlliniBook::GetRelationships() const {
  return relationships_;
}

void IlliniBook::BFS2(int start_uin,
                      std::set<int>& visited,
                      const std::vector<std::string>& relationships) const {
  std::queue<int> q;
  std::set<std::string> rships(relationships.begin(), relationships.end());
  q.push(start_uin);
  visited.insert(start_uin);

  while (!q.empty()) {
    int current_uin = q.front();
    q.pop();
    auto rel_it = relationships_.find(current_uin);
    if (rel_it != relationships_.end()) {
      for (const auto& neighbor : relationships_.at(current_uin)) {
        int neighbor_uin = neighbor.first;
        std::string neighbor_relationship = neighbor.second;
        if (visited.find(neighbor_uin) == visited.end() &&
            rships.find(neighbor_relationship) != rships.end()) {
          q.push(neighbor_uin);
          visited.insert(neighbor_uin);
        }
      }
    }
  }
}

void IlliniBook::BFS1(int start_uin, std::set<int>& visited) const {
  std::queue<int> q;
  q.push(start_uin);
  visited.insert(start_uin);

  while (!q.empty()) {
    int current_uin = q.front();
    q.pop();
    for (const auto& neighbor : relationships_.at(current_uin)) {
      int neighbor_uin = neighbor.first;
      if (visited.find(neighbor_uin) == visited.end()) {
        q.push(neighbor_uin);
        visited.insert(neighbor_uin);
      }
    }
  }
}

void IlliniBook::BFS(int start_uin,
                     std::set<int>& visited,
                     const std::string& relationship) const {
  std::queue<int> q;
  q.push(start_uin);
  visited.insert(start_uin);

  while (!q.empty()) {
    int current_uin = q.front();
    q.pop();

    for (const auto& neighbor : relationships_.at(current_uin)) {
      int neighbor_uin = neighbor.first;
      const std::string& neighbor_relationship = neighbor.second;
      if (visited.find(neighbor_uin) == visited.end() &&
          neighbor_relationship == relationship) {
        q.push(neighbor_uin);
        visited.insert(neighbor_uin);
      }
    }
  }
}

void IlliniBook::ParseData(const std::string& filename, bool isRelationships) {
  std::ifstream ifs(filename);
  if (!ifs.is_open()) {
    return;
  }
  if (!isRelationships) {
    int uin = 0;
    while (ifs >> uin) {
      uins_.push_back(uin);
    }
  } else {
    std::string line;
    while (std::getline(ifs, line)) {
      std::string token;
      size_t pos = 0;
      std::vector<std::string> tokens;
      while ((pos = line.find(',')) != std::string::npos) {
        token = line.substr(0, pos);
        tokens.push_back(token);
        line.erase(0, pos + 1);
      }
      tokens.push_back(line);
      if (tokens.size() >= 3) {
        int uin_a = std::stoi(tokens[0]);
        int uin_b = std::stoi(tokens[1]);
        std::string relationship = tokens[2];
        relationships_[uin_a].push_back({uin_b, relationship});
        relationships_[uin_b].push_back({uin_a, relationship});
      }
    }
  }
  ifs.close();
}

IlliniBook::IlliniBook(const std::string& people_fpath,
                       const std::string& relations_fpath) {
  // first csv file is people (int UIN)
  // second csv file is relations (int int string)
  ParseData(people_fpath, false);
  ParseData(relations_fpath, true);
}

IlliniBook::~IlliniBook() = default;

bool IlliniBook::AreRelated(int uin_1, int uin_2) const {
  if (relationships_.find(uin_1) == relationships_.end() ||
      relationships_.find(uin_2) == relationships_.end()) {
    return false;
  }

  std::queue<int> q;
  std::set<int> visited;
  q.push(uin_1);
  visited.insert(uin_1);

  while (!q.empty()) {
    int current_uin = q.front();
    q.pop();
    if (current_uin == uin_2) {
      return true;
    }
    if (relationships_.find(current_uin) == relationships_.end()) {
      continue;
    }
    for (const auto& neighbor : relationships_.at(current_uin)) {
      int neighbor_uin = neighbor.first;
      if (visited.find(neighbor_uin) == visited.end()) {
        q.push(neighbor_uin);
        visited.insert(neighbor_uin);
      }
    }
  }
  return false;
}
bool IlliniBook::AreRelated(int uin_1,
                            int uin_2,
                            const std::string& relationship) const {
  if (relationships_.find(uin_1) == relationships_.end() ||
      relationships_.find(uin_2) == relationships_.end()) {
    return false;
  }
  std::queue<int> q;
  std::set<int> visited;

  q.push(uin_1);
  visited.insert(uin_1);

  while (!q.empty()) {
    int current_uin = q.front();
    q.pop();
    if (current_uin == uin_2) {
      return true;
    }
    if (relationships_.find(current_uin) == relationships_.end()) {
      continue;
    }
    for (const auto& neighbor : relationships_.at(current_uin)) {
      int neighbor_uin = neighbor.first;
      const std::string& neighbor_relationship = neighbor.second;
      if (visited.find(neighbor_uin) == visited.end() &&
          neighbor_relationship == relationship) {
        q.push(neighbor_uin);
        visited.insert(neighbor_uin);
      }
    }
  }
  return false;
}
int IlliniBook::GetRelated(int uin_1, int uin_2) const {
  if (relationships_.find(uin_1) == relationships_.end() ||
      relationships_.find(uin_2) == relationships_.end()) {
    return -1;
  }

  std::queue<int> q;
  std::set<int> visited;
  std::map<int, int> distance;

  q.push(uin_1);
  visited.insert(uin_1);
  distance[uin_1] = 0;

  while (!q.empty()) {
    int current_uin = q.front();
    q.pop();
    if (current_uin == uin_2) {
      return distance[current_uin];
    }
    for (const auto& neighbor : relationships_.at(current_uin)) {
      int neighbor_uin = neighbor.first;
      if (visited.find(neighbor_uin) == visited.end()) {
        q.push(neighbor_uin);
        visited.insert(neighbor_uin);
        distance[neighbor_uin] = distance[current_uin] + 1;
      }
    }
  }
  return -1;
}
int IlliniBook::GetRelated(int uin_1,
                           int uin_2,
                           const std::string& relationship) const {
  if (relationships_.find(uin_1) == relationships_.end() ||
      relationships_.find(uin_2) == relationships_.end()) {
    return -1;
  }
  std::queue<int> q;
  std::set<int> visited;
  std::map<int, int> distance;

  q.push(uin_1);
  visited.insert(uin_1);
  distance[uin_1] = 0;

  while (!q.empty()) {
    int current_uin = q.front();
    q.pop();
    if (current_uin == uin_2) {
      return distance[current_uin];
    }
    for (const auto& neighbor : relationships_.at(current_uin)) {
      int neighbor_uin = neighbor.first;
      const std::string& neighbor_relationship = neighbor.second;
      if (visited.find(neighbor_uin) == visited.end() &&
          neighbor_relationship == relationship) {
        q.push(neighbor_uin);
        visited.insert(neighbor_uin);
        distance[neighbor_uin] = distance[current_uin] + 1;
      }
    }
  }
  return -1;
}
std::vector<int> IlliniBook::GetSteps(int uin, int n) const {
  if (relationships_.find(uin) == relationships_.end()) {
    return {};
  }

  std::vector<int> steps;
  std::queue<int> q;
  std::map<int, int> distance;

  q.push(uin);
  distance[uin] = 0;

  while (!q.empty()) {
    int current_uin = q.front();
    q.pop();

    if (distance[current_uin] == n) {
      steps.push_back(current_uin);
    } else if (distance[current_uin] > n) {
      break;
    }

    for (const auto& neighbor : relationships_.at(current_uin)) {
      int neighbor_uin = neighbor.first;
      if (distance.find(neighbor_uin) == distance.end()) {
        q.push(neighbor_uin);
        distance[neighbor_uin] = distance[current_uin] + 1;
      }
    }
  }

  return steps;
}
size_t IlliniBook::CountGroups() const {
  std::set<int> visited;
  size_t num_groups = 0;

  for (const auto& entry : relationships_) {
    int uin = entry.first;
    if (visited.find(uin) == visited.end()) {
      BFS1(uin, visited);
      num_groups++;
    }
  }
  // consider solo groups
  for (const auto& uin : uins_) {
    if (visited.find(uin) == visited.end()) {
      num_groups++;
    }
  }
  return num_groups;
}
size_t IlliniBook::CountGroups(const std::string& relationship) const {
  std::set<int> visited;
  size_t num_groups = 0;

  for (const auto& entry : relationships_) {
    int uin = entry.first;
    if (visited.find(uin) == visited.end()) {
      BFS(uin, visited, relationship);
      num_groups++;
    }
  }
  // consider solo groups
  for (const auto& uin : uins_) {
    if (visited.find(uin) == visited.end()) {
      num_groups++;
    }
  }
  return num_groups;
}
size_t IlliniBook::CountGroups(
    const std::vector<std::string>& relationships) const {
  std::set<int> visited;
  size_t num_groups = 0;

  for (const auto& uin : uins_) {
    if (visited.find(uin) == visited.end()) {
      BFS2(uin, visited, relationships);
      num_groups++;
    }
  }

  return num_groups;
}