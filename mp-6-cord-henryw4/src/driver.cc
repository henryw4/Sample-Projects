#include <iostream>

#include "cord.hpp"
#include "cord_utilities.hpp"

int main() {
  SharedPointer<Cord> first = new Cord("Goodbye, ");
  SharedPointer<Cord> second = new Cord("Cruel World!");
  SharedPointer<Cord> sentence = ConcatCords(first, second);
  SharedPointer<Cord> third = new Cord(" angle .");
  SharedPointer<Cord> sentence2 = ConcatCords(sentence, third);
  auto part = SubString(sentence, 2, 20);

  auto part_str = part->ToString();
  std::cout << part_str.Get() << std::endl;

  auto partB = SubString(sentence, 3, 17);

  auto partB_str = partB->ToString();
  std::cout << partB_str.Get() << std::endl;

  auto part2 = SubString(sentence2, 5, 24);
  auto part2_str = part2->ToString();
  std::cout << part2_str.Get() << std::endl;

  SharedPointer<Cord> sentence3 = ConcatCords(first, third);
  auto part3 = SubString(sentence3, 4, 10);
  auto part3_str = part3->ToString();
  std::cout << part3_str.Get() << std::endl;
}