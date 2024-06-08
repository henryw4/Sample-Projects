#include "cord.hpp"

#include <stdexcept>

#include "cord_utilities.hpp"

// custom helper constructor
Cord::Cord(SharedPointer<Cord> left, SharedPointer<Cord> right):
    left_(left), right_(right), length_(left->Length() + right->Length()) {}

Cord::Cord(std::string_view sv) {
  if (sv.empty()) {
    throw std::exception();
  }
  data_ = new char[sv.size() + 1];

  for (size_t i = 0; i < sv.size(); ++i) {
    data_[i] = sv[i];
  }
  data_[sv.size()] = '\0';
  length_ = sv.size();
}

Cord::~Cord() { delete[] data_; }

bool Cord::IsLeaf(const Cord* node) const {
  return node->Left().Get() == nullptr && node->Right().Get() == nullptr;
}

SharedPointer<char> Cord::ToString() const {
  if (IsLeaf(this)) {
    std::string_view sv(this->data_);
    auto size = sv.size();
    char* buffer = new char[size + 1];
    std::copy(sv.begin(), sv.end(), buffer);
    buffer[size] = '\0';
    return {buffer, kSharedPointerType::kArray};
  }
  char* concatenated = new char[this->Length() + 1]();
  auto concatenate = [&](const Cord* node, auto& concatLambda) -> void {
    if (node->IsLeaf(node)) {
      std::string_view sv(node->data_);
      for (size_t i = 0; i < sv.size(); ++i) {
        *(concatenated++) = sv[i];
      }
    } else {
      if (node->Left().Get() != nullptr) {
        concatLambda(node->Left().Get(), concatLambda);
      }
      if (node->Right().Get() != nullptr) {
        concatLambda(node->Right().Get(), concatLambda);
      }
    }
  };
  concatenate(this, concatenate);
  return {concatenated - this->Length(), kSharedPointerType::kArray};
}