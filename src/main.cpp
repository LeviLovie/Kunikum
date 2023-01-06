#include <iostream>

#include "VRAM.h"
#include "reading.h"

int main(int, char**) {
    ClearVRAM();
    DrawColorPallete();

    CompileFile("app.kun");

    SetPixels(50, 40, 10, 10, 1); SetPixels(60, 40, 10, 10, 1); SetPixels(50, 30, 10, 10, 6); SetPixels(60, 30, 10, 10, 6);
    SetPixels(53, 43, 4, 4, 12); SetPixels(63, 43, 4, 4, 12);
    SetPixels(50, 50, 10, 10, 2); SetPixels(60, 50, 10, 10, 2); SetPixels(50, 60, 10, 10, 2); SetPixels(60, 60, 10, 10, 2); SetPixels(50, 70, 10, 10, 2); SetPixels(60, 70, 10, 10, 2);
    SetPixels(40, 50, 10, 10, 8); SetPixels(70, 50, 10, 10, 8); SetPixels(40, 60, 10, 10, 8); SetPixels(70, 60, 10, 10, 8);
    SetPixels(50, 80, 10, 10, 11); SetPixels(60, 80, 10, 10, 11); SetPixels(50, 90, 10, 10, 11); SetPixels(60, 90, 10, 10, 11);

    SaveVRAM();
}
