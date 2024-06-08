#include "cord_utilities.hpp"

#include <stdexcept>
#include <string_view>

SharedPointer<Cord> ConcatCords(SharedPointer<Cord> left_cord,
                                SharedPointer<Cord> right_cord) {
  if (left_cord.Get() == nullptr || right_cord.Get() == nullptr) {
    throw std::invalid_argument("One of the cords is uninitialized.");
  }

  return {new Cord(left_cord, right_cord), kSharedPointerType::kObject};
}

// helper for substring
bool IsLeaf(const SharedPointer<Cord>& cord) {
  return cord->Left().Get() == nullptr && cord->Right().Get() == nullptr;
}
std::string_view CordToString(const SharedPointer<Cord>& cord) {
  if (!IsLeaf(cord)) {
    throw std::logic_error("CordToString must be called on a leaf Cord.");
  }
  return cord->Data();
}

SharedPointer<Cord> MakeCord(char* str) {
  if (str == nullptr) {
    throw std::invalid_argument("Null string passed to MakeCord.");
  }
  std::string_view sv(str);
  return {new Cord(sv)};
}

SharedPointer<Cord> SubString(SharedPointer<Cord> curr_cord,
                              unsigned int lower_idx,
                              unsigned int upper_idx) {
  if (curr_cord.Get() == nullptr) {
    throw std::invalid_argument("curr_cord cannot be null.");
  }

  if (lower_idx >= upper_idx) {
    throw std::invalid_argument("lower_idx must be less than upper_idx.");
  }

  if (upper_idx > curr_cord->Length()) {
    throw std::out_of_range("upper_idx is out of bounds.");
  }
  if (lower_idx == 0 && upper_idx == curr_cord->Length()) {
    return curr_cord;
  }
  if (IsLeaf(curr_cord)) {
    unsigned int length = upper_idx - lower_idx;
    char* sub_str = new char[length + 1];
    std::copy_n(curr_cord->Data().begin() + lower_idx, length, sub_str);
    sub_str[length] = '\0';
    return {new Cord(std::string_view(sub_str, length)),
            kSharedPointerType::kObject};
  }
  unsigned int midpoint = curr_cord->Left()->Length();
  if (upper_idx <= midpoint) {
    return SubString(curr_cord->Left(), lower_idx, upper_idx);
  }
  if (lower_idx >= midpoint) {
    return SubString(
        curr_cord->Right(), lower_idx - midpoint, upper_idx - midpoint);
  }
  SharedPointer<Cord> leftsub =
      SubString(curr_cord->Left(), lower_idx, midpoint);
  SharedPointer<Cord> rightsub =
      SubString(curr_cord->Right(), 0, upper_idx - midpoint);
  return ConcatCords(leftsub, rightsub);
}
