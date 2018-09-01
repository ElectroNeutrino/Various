#include <string.h>

GLubyte letters[][10] = { // Bottom to top
    {0x00, 0x00, 0x20, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x00}, // !
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, //  
    {0x90, 0x90, 0x48, 0xFC, 0x48, 0x48, 0xFC, 0x48, 0x24, 0x24}, // #
    {0x20, 0x20, 0xE0, 0x90, 0x10, 0x60, 0x80, 0x90, 0x70, 0x20}, // $
    {0x00, 0x00, 0x10, 0x28, 0x10, 0xE0, 0x18, 0x40, 0xA0, 0x40}, // %
    {0x00, 0x00, 0x78, 0x90, 0xA8, 0x60, 0x40, 0x40, 0x38, 0x00}, // &
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x20, 0x30, 0x30}, // '
    {0x00, 0x10, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, 0x10}, // (
    {0x00, 0x20, 0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x20, 0x20}, // )
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x28, 0x28, 0x10, 0x7C, 0x10}, // *
    {0x00, 0x00, 0x10, 0x10, 0x10, 0xFE, 0x10, 0x10, 0x10, 0x00}, // +
    {0x00, 0x40, 0x60, 0x20, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00}, // ,
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00}, // -
    {0x00, 0x30, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // .
    {0x80, 0x80, 0x40, 0x40, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08}, // /
    {0x00, 0x00, 0x78, 0x84, 0x84, 0x84, 0x84, 0x84, 0x84, 0x78}, // 0
    {0x00, 0x00, 0xF8, 0x20, 0x20, 0x20, 0x20, 0x20, 0xE0, 0x20}, // 1
    {0x00, 0x00, 0xF8, 0x88, 0x40, 0x20, 0x10, 0x08, 0x88, 0x70}, // 2
    {0x00, 0x00, 0x70, 0x88, 0x08, 0x08, 0x30, 0x08, 0x88, 0x70}, // 3
    {0x00, 0x00, 0x1C, 0x08, 0x08, 0xFC, 0x48, 0x48, 0x28, 0x18}, // 4
    {0x00, 0x00, 0x78, 0x84, 0x04, 0x04, 0x78, 0x40, 0x40, 0x7C}, // 5
    {0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0xF0, 0x80, 0x40, 0x38}, // 6
    {0x00, 0x00, 0x10, 0x10, 0x10, 0x08, 0x08, 0x04, 0x84, 0xFC}, // 7
    {0x00, 0x00, 0x78, 0x84, 0x84, 0x84, 0x78, 0x84, 0x84, 0x78}, // 8
    {0x00, 0x00, 0xF0, 0x08, 0x04, 0x7C, 0x84, 0x84, 0x84, 0x78}, // 9
    {0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00}, // :
    {0x00, 0x40, 0x60, 0x30, 0x00, 0x00, 0x30, 0x30, 0x00, 0x00}, // ;
    {0x00, 0x00, 0x04, 0x18, 0x20, 0xC0, 0x20, 0x18, 0x04, 0x00}, // <
    {0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0xFC, 0x00, 0x00}, // =
    {0x00, 0x00, 0x80, 0x60, 0x20, 0x18, 0x20, 0x60, 0x80, 0x00}, // >
    {0x00, 0x00, 0x60, 0x00, 0x20, 0x10, 0x08, 0x08, 0x88, 0x70}, // ?
    {0x70, 0x88, 0x80, 0x98, 0xA8, 0xA8, 0x98, 0x88, 0x88, 0x70}, // @
    {0x00, 0x00, 0xEE, 0x44, 0x38, 0x28, 0x28, 0x28, 0x10, 0x30}, // A
    {0x00, 0x00, 0xF8, 0x44, 0x44, 0x44, 0x78, 0x44, 0x44, 0xF8}, // B
    {0x00, 0x00, 0x38, 0x44, 0x80, 0x80, 0x80, 0x80, 0x44, 0x3C}, // C
    {0x00, 0x00, 0xF0, 0x48, 0x44, 0x44, 0x44, 0x44, 0x48, 0xF0}, // D
    {0x00, 0x00, 0xFC, 0x44, 0x40, 0x50, 0x70, 0x50, 0x44, 0xFC}, // E
    {0x00, 0x00, 0xE0, 0x40, 0x40, 0x50, 0x70, 0x50, 0x44, 0xFC}, // F
    {0x00, 0x00, 0x38, 0x44, 0x84, 0x8E, 0x80, 0x80, 0x44, 0x3C}, // G
    {0x00, 0x00, 0xEE, 0x44, 0x44, 0x44, 0x7C, 0x44, 0x44, 0xEE}, // H
    {0x00, 0x00, 0xF8, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xF8}, // I
    {0x00, 0x00, 0x70, 0x88, 0x88, 0x88, 0x08, 0x08, 0x08, 0x3C}, // J
    {0x00, 0x00, 0xE6, 0x44, 0x48, 0x70, 0x50, 0x48, 0x44, 0xEE}, // K
    {0x00, 0x00, 0xFC, 0x44, 0x44, 0x40, 0x40, 0x40, 0x40, 0xE0}, // L
    {0x00, 0x00, 0xEE, 0x44, 0x44, 0x54, 0x54, 0x6C, 0x6C, 0xEE}, // M
    {0x00, 0x00, 0xE6, 0x46, 0x4A, 0x4A, 0x52, 0x52, 0x65, 0xE7}, // N
    {0x00, 0x00, 0x38, 0x44, 0x82, 0x82, 0x82, 0x82, 0x44, 0x38}, // O
    {0x00, 0x00, 0xE0, 0x40, 0x40, 0x78, 0x44, 0x44, 0x44, 0xF8}, // P
    {0x00, 0x3E, 0x38, 0x44, 0x82, 0x82, 0x82, 0x82, 0x44, 0x38}, // Q
    {0x00, 0x00, 0xE2, 0x44, 0x48, 0x78, 0x44, 0x44, 0x44, 0xF8}, // R
    {0x00, 0x00, 0xB8, 0xC4, 0x04, 0x04, 0x78, 0x80, 0x8C, 0x74}, // S
    {0x00, 0x00, 0x38, 0x10, 0x10, 0x10, 0x10, 0x10, 0x92, 0xFE}, // T
    {0x00, 0x00, 0x38, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0xEE}, // U
    {0x00, 0x00, 0x18, 0x18, 0x24, 0x24, 0x24, 0x42, 0x42, 0xE7}, // V
    {0x00, 0x00, 0x28, 0x54, 0x54, 0x54, 0x54, 0x44, 0x44, 0xEE}, // W
    {0x00, 0x00, 0xEE, 0x44, 0x28, 0x10, 0x10, 0x28, 0x44, 0xEE}, // X
    {0x00, 0x00, 0x38, 0x10, 0x10, 0x10, 0x28, 0x28, 0x44, 0xEE}, // Y
    {0x00, 0x00, 0xF8, 0x88, 0x40, 0x20, 0x20, 0x10, 0x88, 0xF8}, // Z
    {0x70, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x70}, // [
    {0x08, 0x08, 0x10, 0x10, 0x20, 0x20, 0x40, 0x40, 0x80, 0x80}, // \
    {0x70, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x70}, // ]
    {0x70, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x70}, // ]
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x28, 0x10, 0x10}, // ^
    {0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}, // _
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x20, 0x00}, // `
    {0x00, 0x00, 0x76, 0x8C, 0x84, 0x7C, 0x84, 0x78, 0x00, 0x00}, // a
    {0x00, 0x00, 0xDC, 0x62, 0x42, 0x42, 0x62, 0x5C, 0x40, 0xC0}, // b
    {0x00, 0x00, 0x78, 0x84, 0x80, 0x80, 0x8C, 0x74, 0x00, 0x00}, // c
    {0x00, 0x00, 0x76, 0x8C, 0x84, 0x84, 0x8C, 0x74, 0x04, 0x0C}, // d
    {0x00, 0x00, 0x7C, 0x80, 0x80, 0xFC, 0x84, 0x78, 0x00, 0x00}, // e
    {0x00, 0x00, 0xFC, 0x20, 0x20, 0x20, 0x20, 0xFC, 0x20, 0x1C}, // f
    {0x78, 0x04, 0x74, 0x8C, 0x84, 0x84, 0x8C, 0x76, 0x00, 0x00}, // g
    {0x00, 0x00, 0xEE, 0x44, 0x44, 0x44, 0x64, 0x58, 0x40, 0xC0}, // h
    {0x00, 0x00, 0xF8, 0x20, 0x20, 0x20, 0x20, 0xE0, 0x00, 0x20}, // i
    {0xF0, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x78, 0x00, 0x10}, // j
    {0x00, 0x00, 0xCE, 0x48, 0x50, 0x70, 0x48, 0x5E, 0x40, 0xC0}, // k
    {0x00, 0x00, 0xF8, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x60}, // l
    {0x00, 0x00, 0xED, 0x49, 0x49, 0x49, 0x6D, 0xD2, 0x00, 0x00}, // m
    {0x00, 0x00, 0xEE, 0x44, 0x44, 0x44, 0x64, 0xD8, 0x00, 0x00}, // n
    {0x00, 0x00, 0x78, 0x84, 0x84, 0x84, 0x84, 0x78, 0x00, 0x00}, // o
    {0xE0, 0x40, 0x78, 0x44, 0x44, 0x44, 0x64, 0xD8, 0x00, 0x00}, // p
    {0x0E, 0x04, 0x74, 0x8C, 0x84, 0x84, 0x8C, 0x76, 0x00, 0x00}, // q
    {0x00, 0x00, 0xF8, 0x40, 0x40, 0x40, 0x60, 0xDC, 0x00, 0x00}, // r
    {0x00, 0x00, 0xF8, 0x84, 0x04, 0x78, 0x84, 0x7C, 0x00, 0x00}, // s
    {0x00, 0x00, 0x38, 0x44, 0x40, 0x40, 0x40, 0xF8, 0x40, 0x00}, // t
    {0x00, 0x00, 0x36, 0x4C, 0x44, 0x44, 0x44, 0xCC, 0x00, 0x00}, // u
    {0x00, 0x00, 0x18, 0x18, 0x24, 0x24, 0x42, 0xE7, 0x00, 0x00}, // v
    {0x00, 0x00, 0x28, 0x54, 0x54, 0x54, 0x44, 0xEE, 0x00, 0x00}, // w
    {0x00, 0x00, 0xCC, 0x48, 0x30, 0x30, 0x48, 0xCC, 0x00, 0x00}, // x
    {0x70, 0x10, 0x10, 0x28, 0x28, 0x44, 0x44, 0xEE, 0x00, 0x00}, // y
    {0x00, 0x00, 0xF8, 0x88, 0x40, 0x20, 0x90, 0xF8, 0x00, 0x00}, // z
    {0x00, 0x20, 0x40, 0x40, 0x40, 0x80, 0x40, 0x40, 0x40, 0x20}, // {
    {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10}, // |
    {0x00, 0x08, 0x04, 0x04, 0x04, 0x02, 0x04, 0x04, 0x04, 0x08}, // }
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x98, 0x32, 0x00, 0x00, 0x00}, // ~
};

void printString(const char *s, float x, float y)
{
    glRasterPos2f(x, y);
    glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
    for (int x = 0; x < strlen(s); x++)
    {
        glBitmap(8, 10, 0.0, 0.0, 10.0, 0.0, letters[s[x] - '!']);
    }
}