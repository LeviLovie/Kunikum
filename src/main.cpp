#include <iostream>

#include "VRAM.h"
#include "reading.h"

int main(int, char**) {
    ClearVRAM();
    DrawColorPallete();
    SaveVRAM();
}
