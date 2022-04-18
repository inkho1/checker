#include "Dictionary.hpp"

#include <iterator>
#include <sstream>


void Dictionary::loadFromStream(std::istream& in) {
  mDictionary.clear();

  std::string line;
  while (std::getline(in, line) && line != DELIMITER) {
    std::istringstream strm{line};
    mDictionary.insert(mDictionary.end(),
      std::istream_iterator<std::string>(strm),
      std::istream_iterator<std::string>());
  }
}

void Dictionary::printSuggestionsToStream(const std::string& value, std::ostream& out, size_t maxEdits) {
  std::map<size_t /*distance*/, std::deque<DictionaryStorage::iterator> /*suggestions by distance*/> suggestions;
  size_t maxDistance = MAX_DISTANCE;

  for (auto it = mDictionary.begin(); it != mDictionary.end(); ++it) {
    if (abs(static_cast<int>(value.size()) - static_cast<int>(it->length())) > maxEdits) {
      continue;
    }

    auto distance = levenshteinDistance(value.c_str(), value.length(), it->c_str(), it->length(), maxEdits);
    if (distance == 0) {
      out << value;
      return;
    } 
    
    if (distance != MAX_DISTANCE && distance <= maxDistance) {
      maxDistance = distance;
      suggestions[distance].push_back(it);
    }
  }

  if (!suggestions.empty()) {
    auto& entries = suggestions.begin()->second;
    if (entries.size() == 1) {
      out << *entries[0];
    } else {
      out << "{";
      for (int i = 0; i < entries.size(); ++i) {
        out << (i > 0 ? " " : "") << *entries[i];
      }
      out << "}";
    }
  } else {
    out << "{" << value << "?}";
  }
}

size_t Dictionary::levenshteinDistance(const char* str1, size_t i, const char* str2, size_t j, size_t maxEdits,
    OperationType previousOperation) noexcept {
  if (i == 0 && j == 0) {
    return 0;
  }

  auto distance = MAX_DISTANCE;

  // Replace, weight = 0 (if characters eaual) or 2 (delete + insert)
  bool charactersEqual = std::toupper(str1[i - 1]) == std::toupper(str2[j - 1]);
  if (i > 0 && j > 0 && (charactersEqual || maxEdits >= 2)) {
    auto weight = charactersEqual ? 0 : 2;
    auto replaceDistance = levenshteinDistance(str1, i - 1, str2, j - 1, maxEdits - weight);
    if (replaceDistance != MAX_DISTANCE) {
      distance = std::min(distance, replaceDistance + weight);
    }
  }

  // Insert, weight = 1
  if (j > 0 && maxEdits > 0 && previousOperation != OperationType::Insert) {
    auto insertDistance = levenshteinDistance(str1, i, str2, j - 1, maxEdits - 1, OperationType::Insert);
    if (insertDistance != MAX_DISTANCE) {
      distance = std::min(distance, insertDistance + 1);
    }
  }

  // Delete, weight = 1
  if (i > 0 && maxEdits > 0 && previousOperation != OperationType::Delete) {
    auto deleteDistance = levenshteinDistance(str1, i - 1, str2, j, maxEdits - 1, OperationType::Delete);
    if (deleteDistance != MAX_DISTANCE) {
      distance = std::min(distance, deleteDistance + 1);
    }
  }

  return distance > maxEdits ? MAX_DISTANCE : distance;
}
