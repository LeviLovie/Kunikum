#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

#include "reading.h"
#include "VRAM.h"

std::string ErrorColor = "\033[0;31m";
std::string WarnColor = "\033[0;33m";
std::string ResetColor = "\033[0;37m";

int tabSize = 2;
int FPS = 1;

std::string updateCommands[4096];

std::map<std::string, int>* intVars = new std::map<std::string, int>;
std::map<std::string, std::string>* strVars = new std::map<std::string, std::string>;

void CompileFile(std::string FileName) {
    std::ifstream infile(FileName);
    std::string line;
    std::string newline;

    std::string method = "";
    int step = 0;
    int lineNumber = 0;

    int updateLineNumber = 0;

    while (getline(infile, line)) {
        lineNumber++;
        if (line.find("TABSIZE") == 0) {
            newline = line.substr(8);
            if (newline.find("=") == 0) {
                newline = newline.substr(2);
                if (newline.find(";") == newline.length() - 1) {
                    newline = newline.substr(0, newline.length() - 1);
                    tabSize = std::stoi(newline);
                    std::cout << tabSize << std::endl;
                } else {
                    std::cout << WarnColor << lineNumber << " - Missing semicolon: \"" << line << "\"" << ResetColor << std::endl;
                }
            } else {
                std::cout << WarnColor << lineNumber << " - Missing equal sign: \"" << line << "\"" << ResetColor << std::endl;
            }
        }
        if (line.find("FPS") == 0) {
            newline = line.substr(4);
            if (newline.find("=") == 0) {
                newline = newline.substr(2);
                if (newline.find(";") == newline.length() - 1) {
                    newline = newline.substr(0, newline.length() - 1);
                    FPS = std::stoi(newline);
                    std::cout << FPS << std::endl;
                } else {
                    std::cout << WarnColor << lineNumber << " - Missing semicolon: \"" << line << "\"" << ResetColor << std::endl;
                }
            } else {
                std::cout << WarnColor << lineNumber << " - Missing equal sign: \"" << line << "\"" << ResetColor << std::endl;
            }
        }

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
                    std::cout << newline << std::endl;
                    if (newline.find("INIT" == 0)) {
                        newline = newline.substr(4);
                        if (newline.find("() {") == 0) {
                            if (newline != "") {
                                method = "INIT";
                                step++;
                            } else {
                                std::cout << WarnColor << lineNumber << " - can't compille code after \"() {\": \"" << line << "\"" << ResetColor << std::endl;
                            }
                        } else {
                            std::cout << WarnColor << lineNumber << " - function start missing: \"" << line << "\"" << ResetColor << std::endl;
                        }
                    } else if (newline.find("UPDATE" == 0)) {
                        newline = newline.substr(6);
                        std::cout << newline << std::endl;
                        if (newline.find("() {") == 0) {
                            if (newline != "") {
                                method = "UPDATE";
                                step++;
                            } else {
                                std::cout << WarnColor << lineNumber << " - can't compille code after \"() {\": \"" << line << "\"" << ResetColor << std::endl;
                            }
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
                if (line.find("LOG") == tabSize) {
                    newline = line.substr(tabSize + 3);
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
                } else if (line.find("VRAM.SET") == tabSize) {
                    newline = line.substr(tabSize + 8);
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
                } else if (line.find("VRAM.RECTSET") == tabSize) {
                    newline = line.substr(tabSize + 12);
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
                        method = "";
                        step--;
                    } else {
                        std::cout << ErrorColor << lineNumber << " - Doesn't statement to close" << ResetColor << std::endl;
                    }
                } else {
                    std::cout << WarnColor << lineNumber << " - Unknow line syntax: \"" << line << "\"" << ResetColor << std::endl;
                } 
            } else if (method == "UPDATE") {
                std::cout << "Update" << std::endl;
                if (line.find("}") == 0) {
                    if (step > 0) {
                        method = "";
                        step--;
                    } else {
                        std::cout << ErrorColor << lineNumber << " - Doesn't statement to close" << ResetColor << std::endl;
                    }
                } else {
                    updateCommands[updateLineNumber] = line.substr(0, line.length());
                }
                updateLineNumber++;
            }
        }
    }
    if (step > 0) {
        std::cout << ErrorColor << lineNumber << " - Doesn't statement to close" << ResetColor << std::endl;
    }

    Update();
}

void Update() {
    for (int i = 0; i < 4096; i++) {
        if (updateCommands[i] != "") {
            std::cout << updateCommands[i] << std::endl;
        }
    }
}