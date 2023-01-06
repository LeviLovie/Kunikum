#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "reading.h"
#include "VRAM.h"

std::string ErrorColor = "\033[0;31m";
std::string WarnColor = "\033[0;33m";
std::string ResetColor = "\033[0;37m";

void CompileFile(std::string FileName) {
    std::ifstream infile(FileName);
    std::string line;
    std::string newline;

    std::string method = "";
    int step = 0;
    int lineNumber = 0;

    while (getline(infile, line)) {
        lineNumber++;

        if (line.find("#") != std::string::npos || line == "") {
            continue;
        } else {
            if (method == "") {
                if (line.find("}") == 0) {
                    if (step > 0) {
                        step--;
                    } else {
                        std::cout << ErrorColor << lineNumber << " - Doesn't statement to close" << ResetColor << std::endl;
                    }
                } else if (line.find("ENGINE") == 0) {
                    newline = line.substr(7);
                    if (line.find("INIT" == 0)) {
                        newline = newline.substr(4);
                        if (newline == "() {") {
                            step++;
                            method = "INIT";
                        } else {
                            std::cout << WarnColor << lineNumber << " - function start missing: \"" << line << "\"" << ResetColor << std::endl;
                        }
                    } else {
                        std::cout << WarnColor << lineNumber << " - Unkow method name: \"" << line << "\"" << ResetColor << std::endl;
                    }
                } else {
                    std::cout << WarnColor << lineNumber << " - Unknow line syntax: \"" << line << "\"" << ResetColor << std::endl;
                }
            } else if (method == "INIT") {
                if (line.find("LOG") == 4) {
                    newline = line.substr(4 + 3);
                    if (newline.find("(") == 0) {
                        newline = newline.substr(1);
                        if (newline.find(");") == newline.length() - 2) {
                            newline = newline.substr(1, newline.length() - 4);
                            std::cout << newline << std::endl;
                        } else {
                            std::cout << WarnColor << lineNumber << " - function end missing: \"" << line << "\"" << ResetColor << std::endl;
                        }
                    } else {
                        std::cout << WarnColor << lineNumber << " - function start missing: \"" << line << "\"" << ResetColor << std::endl;
                    }
                } else if (line.find("VRAM.SET") == 4) {
                    newline = line.substr(4 + 8);
                    if (newline.find("(") == 0) {
                        newline = newline.substr(1);
                        if (newline.find(");") == newline.length() - 2) {
                            newline = newline.substr(0, newline.length() - 2);
                            short args[3];
                            short arg = 0;
                            std::stringstream ss(newline);
                            std::string item;
                            while (getline(ss, item, ',')) {
                                std::stringstream token_stream(item);
                                token_stream >> item;
                                args[arg] = std::stoi(item);
                                arg++;
                            }
                            SetPixel(args[0], args[1], args[2]);
                        } else {
                            std::cout << WarnColor << lineNumber << " - function end missing: \"" << line << "\"" << ResetColor << std::endl;
                        }
                    } else {
                        std::cout << WarnColor << lineNumber << " - function start missing: \"" << line << "\"" << ResetColor << std::endl;
                    }
                } else if (line.find("VRAM.RECTSET") == 4) {
                    newline = line.substr(4 + 12);
                    if (newline.find("(") == 0) {
                        newline = newline.substr(1);
                        if (newline.find(");") == newline.length() - 2) {
                            newline = newline.substr(0, newline.length() - 2);
                            short args[5];
                            short arg = 0;
                            std::stringstream ss(newline);
                            std::string item;
                            while (getline(ss, item, ',')) {
                                std::stringstream token_stream(item);
                                token_stream >> item;
                                args[arg] = std::stoi(item);
                                arg++;
                            }
                            SetPixels(args[0], args[1], args[2], args[3], args[4]);
                        } else {
                            std::cout << WarnColor << lineNumber << " - function end missing: \"" << line << "\"" << ResetColor << std::endl;
                        }
                    } else {
                        std::cout << WarnColor << lineNumber << " - function start missing: \"" << line << "\"" << ResetColor << std::endl;
                    }
                } else if (line.find("}") == 0) {
                    if (step > 0) {
                        step--;
                    } else {
                        std::cout << ErrorColor << lineNumber << " - Doesn't statement to close" << ResetColor << std::endl;
                    }
                } else {
                    std::cout << WarnColor << lineNumber << " - Unknow line syntax: \"" << line << "\"" << ResetColor << std::endl;
                } 
            }
        }
    }
    if (step > 0) {
        std::cout << ErrorColor << lineNumber << " - Doesn't statement to close" << ResetColor << std::endl;
    }
}