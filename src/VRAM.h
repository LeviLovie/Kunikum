extern short VRAM[128][128];
extern int COLORS[16];

void ClearVRAM();
void SaveVRAM();
void SetPixel(short x, short y, short color);
short GetPixel(short x, short y);
void SetPixels(short posX, short posY, short width, short height, short color);
void SetSprite(short posX, short posY, short** sprite);
void DrawColorPallete();
void DrawLine(short x1, short y1, short x2, short y2, short color);