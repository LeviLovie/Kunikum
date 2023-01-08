#include <iostream>
#include <fstream>
#include <string>

void CompileFile(std::string FileName);
void Update();
void Init();
int WorkOnLine(std::string line, std::string method, int lineNumber, int step);