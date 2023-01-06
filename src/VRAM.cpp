#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#include "VRAM.h"

short VRAM[128][128];
int COLORS[16] = {0xFFFFFFFF, 0xFFFF00FF, 0xFF00FFFF, 0xFF0000FF, 0xC0C0C0FF, 0x808080FF, 0x808000FF, 0x800080FF, 0x800000FF, 0x00FFFFFF, 0x00FF00FF, 0x008080FF, 0x008000FF, 0x0000FFFF, 0x000080FF, 0x000000FF};

void ClearVRAM() {
    for (short x = 0; x < 128; x++) {
        for (short y = 0; y < 128; y++) {
            VRAM[x][y] = 0;
        }
    }
}
void SaveVRAM() {
    sf::Image image;
    image.create(128, 128);
    for (short x = 0; x < 128; x++) {
        for (short y = 0; y < 128; y++) {
            image.setPixel(x, y, sf::Color(COLORS[VRAM[x][y]]));
        }
    }
    image.saveToFile("VRAM.png");
}
void SetPixel(short x, short y, short color) {
    if (x < 128 && y < 128 && x >= 0 && y >= 0) {
        if (color <= 16 && color >= 0) {
            VRAM[x][y] = color;
        } else {
            std::cout << "Error: Color out of bounds" << std::endl;
        }
    } else {
        std::cout << "Error: Pixel out of bounds" << std::endl;
    }
}
short GetPixel(short x, short y) {
    if (x < 128 && y < 128 && x >= 0 && y >= 0) {
        return VRAM[x][y];
    } else {
        std::cout << "Error: Pixel out of bounds" << std::endl;
    }
    return 0;
}
void SetPixels(short posX, short posY, short width, short height, short color) {
    for (short y = 0; y < width; y++) {
        for (short x = 0; x < height; x++) {
            if (posX + x < 128 && posY + y < 128 && x >= 0 && y >= 0) {
                if (color <= 16 && color >= 0) {
                    VRAM[posX + x][posY + y] = color;
                } else {
                    std::cout << "Error: Color out of bounds" << std::endl;
                }
            } else {
                std::cout << "Error: Pixel out of bounds" << std::endl;
            }
        }
    }
}
void SetSprite(short posX, short posY, short** sprite) {
    for (short y = 0; y < 16; y++) {
        for (short x = 0; x < 16; x++) {
            if (posX + x < 128 && posY + y < 128 && x >= 0 && y >= 0) {
                if (sprite[y][x] >= 16) {
                    VRAM[posY + y][posX + x] = sprite[y][x];
                } else {
                    std::cout << "Error: Color out of bounds" << std::endl;
                }
            } else {
                std::cout << "Error: Pixel out of bounds" << std::endl;
            }
        }
    }
}
void DrawColorPallete() {
    SetPixels(0, 0, 10, 10, 0);
    SetPixels(0, 10, 10, 10, 4);
    SetPixels(10, 0, 10, 10, 9);
    SetPixels(10, 10, 10, 10, 11);
    SetPixels(20, 0, 10, 10, 1);
    SetPixels(20, 10, 10, 10, 6);
    SetPixels(30, 0, 10, 10, 10);
    SetPixels(30, 10, 10, 10, 12);
    SetPixels(40, 0, 10, 10, 2);
    SetPixels(40, 10, 10, 10, 7);
    SetPixels(50, 0, 10, 10, 3);
    SetPixels(50, 10, 10, 10, 8);
    SetPixels(60, 0, 10, 10, 13);
    SetPixels(60, 10, 10, 10, 14);
    SetPixels(70, 0, 10, 10, 15);
    SetPixels(70, 10, 10, 10, 5);
}
void DrawLine(short x1, short y1, short x2, short y2, short color) {
    if (x1 < 0 || x1 > 127 || x2 < 0 || x2 > 127 || y1 < 0 || y1 > 127 || y2 < 0 || y2 > 127) {
        std::cout << "Error: Line out of bounds" << std::endl;
        return;
    }
    if (color < 0 || color > 15) {
        std::cout << "Error: Color out of bounds" << std::endl;
        return;
    }

    short dx = x2 - x1;
    short dy = y2 - y1;
    short steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float xinc = dx / (float)steps;
    float yinc = dy / (float)steps;
    float x = x1;
    float y = y1;
    for (short i = 0; i <= steps; i++) {
        SetPixel(round(x), round(y), color);
        x += xinc;
        y += yinc;
    }
}