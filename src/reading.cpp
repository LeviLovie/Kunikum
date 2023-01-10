#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <unistd.h>

#include "reading.h"
#include "VRAM.h"

std::string ErrorColor = "\033[0;31m";
std::string WarnColor = "\033[0;33m";
std::string ResetColor = "\033[0;37m";

int tabSize = 2;
int FPS;

std::string updateCommands[4096];
std::string initCommands[4096];

std::map<std::string, int>* intVars = new std::map<std::string, int>;
std::map<std::string, std::string>* strVars = new std::map<std::string, std::string>;

void CompileFile(std::string FileName) {
    std::ifstream infile(FileName);
    std::string line;
    std::string newline;

    std::string method = "";
    int step = 0;
    int lineNumber = 0;
    int startLinneNumber = 0;

    while (getline(infile, line)) {
        if (line.find("#") != std::string::npos || line == "") {
            continue;
        } else {
            if (method == "") {
                if (line.find("}") == 0) {
                    if (step > 1) {
                        step--;
                    } else if (step == 1) {
                        startLinneNumber = 0;
                        method = "";
                        step--;
                    } else {
                        std::cout << ErrorColor << lineNumber << ":1 - Doesn't statement to close" << ResetColor << std::endl;
                        std::exit(0);
                    }
                } else if (line.find("ENGINE") == 0) {
                    newline = line.substr(7);
                    if (newline == "INIT() {") {
                        method = "INIT";
                        startLinneNumber = lineNumber;
                        step++;
                    } else if (newline == "UPDATE() {") {
                        method = "UPDATE";
                        startLinneNumber = lineNumber;
                        step++;
                    } else {
                        std::cout << WarnColor << lineNumber << " - Unkow method name: \"" << line << "\"" << ResetColor << std::endl;
                    }
                } else {
                    std::cout << WarnColor << lineNumber << " - Unknow line syntax: \"" << line << "\"" << ResetColor << std::endl;
                }
            } else if (method == "INIT") {
                if (line.find("SETFPS") == 2) {
                    newline = line.substr(8);
                    if (newline.find("(") == 0) {
                        newline = newline.substr(1);
                        if (newline.find(");") == newline.length() - 2) {
                            newline = newline.substr(0, newline.length() - 2);
                            FPS = std::stoi(newline);
                        } else {
                            std::cout << WarnColor << lineNumber << " - function end missing: \"" << line << "\"" << ResetColor << std::endl;
                        }
                    } else {
                        std::cout << WarnColor << lineNumber << " - function start missing: \"" << line << "\"" << ResetColor << std::endl;
                    }
                } else if (line.find("SETUPDATETIMES") == 2) {
                    newline = line.substr(8);
                    if (newline.find("(") == 0) {
                        newline = newline.substr(1);
                        if (newline.find(");") == newline.length() - 2) {
                            newline = newline.substr(0, newline.length() - 2);
                            FPS = std::stoi(newline);
                        } else {
                            std::cout << WarnColor << lineNumber << " - function end missing: \"" << line << "\"" << ResetColor << std::endl;
                        }
                    } else {
                        std::cout << WarnColor << lineNumber << " - function start missing: \"" << line << "\"" << ResetColor << std::endl;
                    }
                } else if (line != "}") {
                    initCommands[startLinneNumber] = line.substr(2, line.length());
                    startLinneNumber++;
                } else {
                    step--;
                    startLinneNumber = 0;
                    method = "";
                }
            } else if (method == "UPDATE") {
                if (line != "}") {
                    updateCommands[startLinneNumber] = line.substr(2, line.length());
                    startLinneNumber++;
                } else {
                    step--;
                    startLinneNumber = 0;
                    method = "";
                }
            }
        }
        lineNumber++;
    }
    if (step > 0) {
        std::cout << ErrorColor << lineNumber << ":3 - Doesn't statement to close" << ResetColor << std::endl;
        std::exit(0);
    }

    Init();

    while (true)
    {
        Update();
        usleep(1000000 / FPS);
    }
}

void Init() {
    int step = 0;
    for (int i = 0; i < 4096; i++) {
        if (initCommands[i] != "") {
            step = WorkOnLine(initCommands[i], "INIT", i, step);
        }
    }
}

void Update() {
    int step = 0;
    for (int i = 0; i< 4096; i++) {
        if (updateCommands[i] != "") {
            step = WorkOnLine(updateCommands[i], "UPDATE", i, step);
        }
    }
    SaveVRAM();
}

int WorkOnLine(std::string line, std::string method, int lineNumber, int step) {
    std::string newline;
    if (line.find("LOG") == 0) {
        newline = line.substr(3);
        if (newline.find("(") == 0) {
            newline = newline.substr(1);
            if (newline.find(");") == newline.length() - 2) {
                newline = newline.substr(1, newline.length() - 4);
                std::cout << lineNumber << ":" << method << " - \"" << newline << "\"" << std::endl;
            } else {
                std::cout << WarnColor << lineNumber << ":(INIT) - function end missing: \"" << line << "\"" << ResetColor << std::endl;
            }
        } else {
            std::cout << WarnColor << lineNumber << ":(INIT) - function start missing: \"" << line << "\"" << ResetColor << std::endl;
        }
    } else if (line.find("VRAM.SET") == 0) {
        newline = line.substr(8);
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
                std::cout << WarnColor << lineNumber << ":(INIT) - function end missing: \"" << line << "\"" << ResetColor << std::endl;
            }
        } else {
            std::cout << WarnColor << lineNumber << ":(INIT) - function start missing: \"" << line << "\"" << ResetColor << std::endl;
        }
    } else if (line.find("VRAM.RECTSET") == 0) {
        newline = line.substr(12);
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
                std::cout << WarnColor << lineNumber << ":(INIT) - function end missing: \"" << line << "\"" << ResetColor << std::endl;
            }
        } else {
            std::cout << WarnColor << lineNumber << ":(INIT) - function start missing: \"" << line << "\"" << ResetColor << std::endl;
        }
    } else if (line.find("VRAM.PRINT") == 0) {
        newline = line.substr(10);
        if (newline.find("(") == 0) {
            newline = newline.substr(1);
            if (newline.find(");") == newline.length() - 2) {
                std::string text;
                short positions[2];
                int scale;

                newline = newline.substr(0, newline.length() - 2);
                text = newline.substr(1);
                text = text.substr(0, text.find("\""));
                newline = newline.substr(text.length() + 2 + 1);
                if (newline.find(" ") == 0) {
                    newline = newline.substr(1);
                }

                std::stringstream ss(newline);
                std::string item;
                short arg = 0;
                while (getline(ss, item, ',')) {
                    std::stringstream token_stream(item);
                    token_stream >> item;
                    if (arg == 0) {
                        positions[1] = std::stoi(item);
                    } else if (arg == 1) {
                        positions[0] = std::stoi(item);
                    } else if (arg == 2) {
                        scale = std::stoi(item);
                    }
                    arg++;
                }
                
                WriteText(text, positions[0], positions[1], scale, 0);
            } else {
                std::cout << WarnColor << lineNumber << ":(INIT) - function end missing: \"" << line << "\"" << ResetColor << std::endl;
            }
        } else {
            std::cout << WarnColor << lineNumber << ":(INIT) - function start missing: \"" << line << "\"" << ResetColor << std::endl;
        }
    } else if (line.find("string") == 0) {
        newline = line.substr(6);
        if (newline.find("=") == 1) {
            newline = newline.substr(1);
            if (newline.find(" ") == 0) {
                newline = newline.substr(1);
            }
            if (newline.find("\"") == 0) {
                newline = newline.substr(1);
                if (newline.find("\"") == newline.length() - 1) {
                    newline = newline.substr(0, newline.length() - 1);
                    std::cout << lineNumber << ":" << method << " - \"" << newline << "\"" << std::endl;
                } else {
                    std::cout << WarnColor << lineNumber << ":(INIT) - string end missing: \"" << line << "\"" << ResetColor << std::endl;
                }
            } else {
                std::cout << WarnColor << lineNumber << ":(INIT) - string start missing: \"" << line << "\"" << ResetColor << std::endl;
            }
        } else {
            std::cout << WarnColor << lineNumber << ":(INIT) - string assignment missing: \"" << line << "\"" << ResetColor << std::endl;
        }
    } else if (line.find("stirng") == 0) {
        newline = line.substr(6);
        std::cout << "string" << std::endl;
        // if (newline.find("=") == 1) {
        //     newline = newline.substr(1);
        //     if (newline.find(" ") == 0) {
        //         newline = newline.substr(1);
        //     }
        //     if (newline.find("0") == 0) {
        //         newline = newline.substr(1);
        //         if (newline.find(";") == newline.length() - 1) {
        //             newline = newline.substr(0, newline.length() - 1);
        //             std::cout << lineNumber << ":" << method << " - \"" << newline << "\"" << std::endl;
        //         } else {
        //             std::cout << WarnColor << lineNumber << ":(INIT) - int end missing: \"" << line << "\"" << ResetColor << std::endl;
        //         }
        //     } else {
        //         std::cout << WarnColor << lineNumber << ":(INIT) - int start missing: \"" << line << "\"" << ResetColor << std::endl;
        //     }
        // } else {
        //     std::cout << WarnColor << lineNumber << ":(INIT) - int assignment missing: \"" << line << "\"" << ResetColor << std::endl;
        // }
    } else if (line.find("VRAM.COLORS") == 0) {
        newline = line.substr(11);
         if (newline.find("(") == 0) {
            newline = newline.substr(1);
            if (newline.find(");") == newline.length() - 2) {
                newline = newline.substr(0, newline.length() - 2);
                DrawColorPallete();
            } else {
                std::cout << WarnColor << lineNumber << ":(INIT) - function end missing: \"" << line << "\"" << ResetColor << std::endl;
            }
        } else {
            std::cout << WarnColor << lineNumber << ":(INIT) - function start missing: \"" << line << "\"" << ResetColor << std::endl;
        }
    } else if (line.find("}") == 0) {
        if (step > 0) {
            step--;
        } else {
            std::cout << ErrorColor << lineNumber << ":(INIT) - Doesn't statement to close" << ResetColor << std::endl;
            std::exit(0);
        }
    } else {
        std::cout << WarnColor << lineNumber << ":(INIT) - Unknow line syntax: \"" << line << "\"" << ResetColor << std::endl;
    }
    return step;
}