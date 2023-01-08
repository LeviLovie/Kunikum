#include <iostream>

#include "VRAM.h"
#include "reading.h"

int main(int, char**) {
    ClearVRAM();
    CompileFile("app.kun");
    SaveVRAM();
}
