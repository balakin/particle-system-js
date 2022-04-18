#include "Resources.hpp"

#include <fstream>

std::string Resources::LoadText(const std::string &filename) {
  std::string result;
  std::ifstream file(filename);
  if (file.is_open()) {
    std::string line;
    while (std::getline(file, line)) {
      result += line + "\n";
    }

    file.close();
    return result;
  } else {
    throw std::runtime_error("Cannot open file");
  }
}
