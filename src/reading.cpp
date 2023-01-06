#include <iostream>
#include <fstream>
#include <string>

#include "reading.h"

void CompileFile(std::string FileName) {
    std::ifstream infile(FileName);
    std::string line;
    while (std::getline(infile, line)) {
        std::cout << line << std::endl;
    }
}