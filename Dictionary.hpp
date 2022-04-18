#include <iostream>
#include <limits>
#include <algorithm>
#include <string>
#include <map>
#include <deque>
#include <sstream>


// Allows to load dictionary and print suggestions within maximum edits count allowed.
// * Dictionary entries are not indexed by length and therefore the complexity is linear for each entry.
// * No other optimizations presented as well - for test code simplicity.
// * Dictionary items are not checked for being duplicated - for the same reason.
// * The public interface for spell checking was not generalized and chosen to fit the task only.
class Dictionary final
{
  const size_t MAX_DISTANCE = std::numeric_limits<size_t>::max();

  enum class OperationType {
    None,
    Insert,
    Delete
  };

public:
  inline static const std::string DELIMITER = "===";

  Dictionary() = default;

  void loadFromStream(std::istream& in);
  void printSuggestionsToStream(const std::string& value, std::ostream& out, size_t maxEdits = 2);

private:
  size_t levenshteinDistance(const char* str1, size_t i, const char* str2, size_t j, size_t maxEdits = 2,
      OperationType previousOperation = OperationType::None) noexcept;

private:
  using DictionaryStorage = std::deque<std::string>;
  DictionaryStorage mDictionary;
};
