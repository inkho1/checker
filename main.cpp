#include "Dictionary.hpp"

#include <iostream>
#include <string>
#include <sstream>


int main() {
  Dictionary dictionary;
  dictionary.loadFromStream(std::cin);

  std::string line;
  while (std::getline(std::cin, line) && line != Dictionary::DELIMITER) {
    std::istringstream strm{line};
    std::string value;
    while (std::getline(strm, value, ' ')) {
      if (!value.empty()) {
        dictionary.printSuggestionsToStream(value, std::cout);
        std::cout << " ";
      }
    }
    std::cout << std::endl;
  }

  return 0;
}
