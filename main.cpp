#include "bf_interpreter.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>

#define USAGE_MSG \
  "Usage %s [-f <code file> | -c <code>] (-i <input file>) (-o <output file>)\n" \
  "If no input/output file is provided standart input/output is used instead.\n"


// TODO: implement proper CLI parsing
// TODO: https://github.com/CLIUtils/CLI11
int main(int argc, char **argv) {
  if (argc < 3 || argc % 2 == 0) {
    std::cout << USAGE_MSG;
  }

  std::string source;
  std::istream *is = &std::cin;
  std::ostream *os = &std::cout;
  bool file = false, code = false;
  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      if (argv[i][2] != '\0') {
        std::cout << USAGE_MSG;
        return 1;
      }
      
      switch (argv[i][1]) {
      case 'f':
        if (code) {
          std::cout << USAGE_MSG;
          return 1;
        }
        source = argv[++i];
        file = true;
        break;
      case 'c':
        if (file) {
          std::cout << USAGE_MSG;
          return 1;
        }
        code = true;
        source = argv[++i];
        break;
      case 'i':
        is =  new std::ifstream {argv[++i]};
        break;
      case 'o':
        os = new std::ofstream {argv[++i]};        
        break;
      default:
        std::cout << USAGE_MSG;
        return 1;
      }
      
    } else {
      std::cout << USAGE_MSG;
      return 1;
    }
  }

  if (file) {
    std::ifstream file {source.c_str()};
    source.assign((std::istreambuf_iterator<char>(file)),
                  std::istreambuf_iterator<char>());
  }
  
  BFInterpreter bfi{source.c_str(), is, os};
  bfi.compile();

  return 0;
}
