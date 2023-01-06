#include <iostream>

#include "VRAM.h"

int main(int, char**) {
    ClearVRAM();

    SetPixels(0, 0, 10, 10, 0);
    SetPixels(10, 0, 10, 10, 1);
    SetPixels(20, 0, 10, 10, 2);
    SetPixels(30, 0, 10, 10, 3);
    SetPixels(40, 0, 10, 10, 4);
    SetPixels(50, 0, 10, 10, 5);
    SetPixels(60, 0, 10, 10, 6);
    SetPixels(70, 0, 10, 10, 7);

    SetPixels(0, 10, 10, 20, 8);
    SetPixels(10, 10, 10, 20, 9);
    SetPixels(20, 10, 10, 20, 10);
    SetPixels(30, 10, 10, 20, 11);
    SetPixels(40, 10, 10, 20, 12);
    SetPixels(50, 10, 10, 20, 13);
    SetPixels(60, 10, 10, 20, 14);
    SetPixels(70, 10, 10, 20, 15);

    SaveVRAM();
}
