#ifndef ILLINI_BOOK_HPP
#define ILLINI_BOOK_HPP

#include <map>
#include <set>
#include <string>
#include <vector>

class IlliniBook {
public:
  IlliniBook(const std::string& people_fpath,
             const std::string& relations_fpath);
  IlliniBook(const IlliniBook& rhs) = delete;
  IlliniBook& operator=(const IlliniBook& rhs) = delete;
  ~IlliniBook();
  bool AreRelated(int uin_1, int uin_2) const;
  bool AreRelated(int uin_1, int uin_2, const std::string& relationship) const;
  int GetRelated(int uin_1, int uin_2) const;
  int GetRelated(int uin_1, int uin_2, const std::string& relationship) const;
  std::vector<int> GetSteps(int uin, int n) const;
  size_t CountGroups() const;
  size_t CountGroups(const std::string& relationship) const;
  size_t CountGroups(const std::vector<std::string>& relationships) const;

  // helper functions for debug
  std::vector<int> GetUINS() const;
  std::map<int, std::vector<std::pair<int, std::string>>> GetRelationships()
      const;

private:
  std::vector<int> uins_;
  std::map<int, std::vector<std::pair<int, std::string>>> relationships_;

  void ParseData(const std::string& filename, bool isRelationships);
  void BFS1(int start_uin, std::set<int>& visited) const;
  void BFS(int start_uin,
           std::set<int>& visited,
           const std::string& relationship) const;
  void BFS2(int start_uin,
            std::set<int>& visited,
            const std::vector<std::string>& relationships) const;
};

#endif
