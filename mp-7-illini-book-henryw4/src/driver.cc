#include <iostream>

#include "illini_book.hpp"

int main() {
  const std::string kPersonsCSV = "./example/persons.csv";
  const std::string kRelationsCSV = "./example/relations.csv";
  IlliniBook illini_book(kPersonsCSV, kRelationsCSV);

  std::vector<int> uins = illini_book.GetUINS();
  std::map<int, std::vector<std::pair<int, std::string>>> relationships =
      illini_book.GetRelationships();

  for (int i : uins) {
    std::cout << i << " ";
  }
  std::cout << std::endl;

  for (const auto& entry : relationships) {
    int uin_a = entry.first;
    const std::vector<std::pair<int, std::string>>& relations = entry.second;

    for (const auto& relation : relations) {
      int uin_b = relation.first;
      const std::string& relationship = relation.second;

      std::cout << "UIN " << uin_a << " is related to UIN " << uin_b << " by "
                << relationship << std::endl;
    }
  }

  const int kNine = 9;
  const int kSix = 6;

  std::cout << illini_book.AreRelated(1, 2) << " == true" << std::endl;
  std::cout << illini_book.AreRelated(3, 2) << " == true" << std::endl;
  std::cout << illini_book.AreRelated(1, kNine) << " == false" << std::endl;
  std::cout << illini_book.AreRelated(1, 2, "128") << " == true" << std::endl;
  std::cout << illini_book.AreRelated(1, 2, "124") << " == false" << std::endl;
  std::cout << illini_book.AreRelated(1, kSix, "128") << " == true"
            << std::endl;
  std::cout << illini_book.AreRelated(1, kSix, "124") << " == true"
            << std::endl;

  std::cout << illini_book.GetRelated(1, 2) << " == 1" << std::endl;
  std::cout << illini_book.GetRelated(3, 2) << " == 2" << std::endl;
  std::cout << illini_book.GetRelated(1, kNine) << " == -1" << std::endl;
  std::cout << illini_book.GetRelated(1, 2, "128") << " == 1" << std::endl;
  std::cout << illini_book.GetRelated(1, 2, "124") << " == -1" << std::endl;
  std::cout << illini_book.GetRelated(1, kSix, "128") << " == 2" << std::endl;
  std::cout << illini_book.GetRelated(1, kSix, "124") << " == 1" << std::endl;

  std::cout << "GetSteps(1, 1)"
            << "\t{ ";
  for (int uin : illini_book.GetSteps(1, 1)) {
    std::cout << uin << " ";
  }
  std::cout << "}" << std::endl;

  std::cout << "GetSteps(1, 2)"
            << "\t{ ";
  for (int uin : illini_book.GetSteps(1, 2)) {
    std::cout << uin << " ";
  }
  std::cout << "}" << std::endl;

  std::cout << "GetSteps(1, 3)"
            << "\t{ ";
  for (int uin : illini_book.GetSteps(1, 3)) {
    std::cout << uin << " ";
  }
  std::cout << "}" << std::endl;

  std::cout << "GetSteps(9, 1)"
            << "\t{ ";
  for (int uin : illini_book.GetSteps(9, 1)) {
    std::cout << uin << " ";
  }
  std::cout << "}" << std::endl;

  std::cout << illini_book.CountGroups() << " == 3" << std::endl;
  std::cout << illini_book.CountGroups("128") << " == 6" << std::endl;
  std::cout << illini_book.CountGroups("124") << " == 6" << std::endl;
  std::cout << illini_book.CountGroups("173") << " == 10" << std::endl;

  std::cout << illini_book.CountGroups(std::vector<std::string>{"128", "173"})
            << " == 6 " << std::endl;
  std::cout << illini_book.CountGroups(
                   std::vector<std::string>{"128", "124", "173"})
            << " == 3 " << std::endl;

  return 0;
}
