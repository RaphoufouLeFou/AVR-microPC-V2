
#include "baseLib.h"

const uint8_t font[][5] = {
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0xf0, 0xa, 0xa, 0xa, 0x10, }, // ---
{0x0, 0x0, 0x0, 0x0, 0x0, }, //
{0x0, 0xf6, 0xf6, 0x0, 0x0, }, // !
{0x0, 0xc0, 0x0, 0xc0, 0x0, }, // "
{0x28, 0xfe, 0x28, 0xfe, 0x28, }, // #
{0x1c, 0x2a, 0x2a, 0x2a, 0x18, }, // $
{0xc6, 0xc8, 0x10, 0x26, 0xc6, }, // %
{0x5c, 0xa2, 0xa2, 0x5c, 0x12, }, // &
{0x0, 0x0, 0xc0, 0x0, 0x0, }, // '
{0x0, 0x38, 0x44, 0x82, 0x0, }, // (
{0x0, 0x82, 0x44, 0x38, 0x0, }, // )
{0x0, 0xa0, 0x40, 0xa0, 0x0, }, // *
{0x10, 0x10, 0x7c, 0x10, 0x10, }, // +
{0xe, 0xc, 0x0, 0x0, 0x0, }, // ,
{0x10, 0x10, 0x10, 0x10, 0x10, }, // -
{0x6, 0x6, 0x0, 0x0, 0x0, }, // .
{0x6, 0x8, 0x10, 0x20, 0xc0, }, // /
{0x7c, 0x8a, 0x92, 0xa2, 0x7c, }, // 0
{0x0, 0x42, 0xfe, 0x2, 0x0, }, // 1
{0x42, 0x86, 0x8a, 0x92, 0x62, }, // 2
{0x44, 0x82, 0x92, 0x92, 0x6c, }, // 3
{0x30, 0x50, 0x90, 0xfe, 0x10, }, // 4
{0xe2, 0xa2, 0xa2, 0xa2, 0x9c, }, // 5
{0x3c, 0x52, 0x92, 0x92, 0xc, }, // 6
{0x80, 0x80, 0x9e, 0xa0, 0xc0, }, // 7
{0x6c, 0x92, 0x92, 0x92, 0x6c, }, // 8
{0x60, 0x92, 0x92, 0x94, 0x78, }, // 9
{0x0, 0x6c, 0x6c, 0x0, 0x0, }, // :
{0x0, 0x6e, 0x6c, 0x0, 0x0, }, // ;
{0x80, 0x80, 0x9e, 0xa0, 0xc0, }, // <
{0x28, 0x28, 0x28, 0x28, 0x28, }, // =
{0x6c, 0x92, 0x92, 0x92, 0x6c, }, // >
{0x40, 0x80, 0x8a, 0x90, 0x60, }, // ?
{0x7c, 0x82, 0xba, 0x8a, 0x78, }, // @
{0x7e, 0x88, 0x88, 0x88, 0x7e, }, // A
{0xfe, 0x92, 0x92, 0x92, 0x6c, }, // B
{0x7c, 0x82, 0x82, 0x82, 0x44, }, // C
{0xfe, 0x82, 0x82, 0x82, 0x7c, }, // D
{0xfe, 0x92, 0x92, 0x92, 0x82, }, // E
{0xfe, 0x90, 0x90, 0x90, 0x80, }, // F
{0x7c, 0x82, 0x92, 0x92, 0x5c, }, // G
{0xfe, 0x10, 0x10, 0x10, 0xfe, }, // H
{0x82, 0x82, 0xfe, 0x82, 0x82, }, // I
{0xc, 0x2, 0x2, 0x2, 0xfc, }, // J
{0xfe, 0x10, 0x28, 0x44, 0x82, }, // K
{0xfe, 0x2, 0x2, 0x2, 0x2, }, // L
{0xfe, 0x40, 0x20, 0x40, 0xfe, }, // M
{0xfe, 0x40, 0x20, 0x10, 0xfe, }, // N
{0x7c, 0x82, 0x82, 0x82, 0x7c, }, // O
{0xfe, 0x90, 0x90, 0x90, 0x60, }, // P
{0x7c, 0x82, 0x8a, 0x84, 0x7a, }, // Q
{0xfe, 0x90, 0x98, 0x94, 0x62, }, // R
{0x64, 0x92, 0x92, 0x92, 0x4c, }, // S
{0x80, 0x80, 0xfe, 0x80, 0x80, }, // T
{0xfc, 0x2, 0x2, 0x2, 0xfc, }, // U
{0xf8, 0x4, 0x2, 0x4, 0xf8, }, // V
{0xfc, 0x2, 0xc, 0x2, 0xfc, }, // W
{0xc6, 0x28, 0x10, 0x28, 0xc6, }, // X
{0xe4, 0x12, 0x12, 0x12, 0xfc, }, // Y
{0x86, 0x8a, 0x92, 0xa2, 0xc2, }, // Z
{0xfe, 0x82, 0x82, 0x82, 0x0, }, // [
{0xc0, 0x20, 0x10, 0x8, 0x6, }, // "\"
{0x0, 0x82, 0x82, 0x82, 0xfe, }, // ]
{0x20, 0x40, 0x80, 0x40, 0x20, }, // ^
{0xfe, 0x82, 0x82, 0x82, 0xfe, }, // _
{0xfe, 0x82, 0x82, 0x82, 0xfe, }, // `
{0x4, 0x2a, 0x2a, 0x2a, 0x1e, }, // a
{0xfe, 0x14, 0x22, 0x22, 0x1c, }, // b
{0x1c, 0x22, 0x22, 0x22, 0x14, }, // c
{0x1c, 0x22, 0x22, 0x14, 0xfe, }, // d
{0x1c, 0x2a, 0x2a, 0x2a, 0x18, }, // e
{0x20, 0x7e, 0xa0, 0x80, 0x40, }, // f
{0x10, 0x2a, 0x2a, 0x2a, 0x3c, }, // g
{0xfe, 0x20, 0x20, 0x20, 0x1e, }, // h
{0x0, 0x22, 0xbe, 0x2, 0x0, }, // i
{0x4, 0x2, 0x2, 0xbc, 0x0, }, // j
{0xfe, 0x8, 0x8, 0x14, 0x22, }, // k
{0x0, 0x82, 0xfe, 0x2, 0x0, }, // l
{0x3e, 0x20, 0x3e, 0x20, 0x1e, }, // m
{0x3e, 0x10, 0x20, 0x20, 0x1e, }, // n
{0x1c, 0x22, 0x22, 0x22, 0x1c, }, // o
{0x3e, 0x24, 0x24, 0x24, 0x18, }, // p
{0x18, 0x24, 0x24, 0x24, 0x3e, }, // q
{0x3e, 0x10, 0x20, 0x20, 0x10, }, // r
{0x12, 0x2a, 0x2a, 0x2a, 0x4, }, // s
{0x20, 0x7c, 0x22, 0x2, 0x4, }, // t
{0x3c, 0x2, 0x2, 0x2, 0x3c, }, // u
{0x38, 0x4, 0x2, 0x4, 0x38, }, // v
{0x3c, 0x2, 0xc, 0x2, 0x3c, }, // w
{0x22, 0x14, 0x8, 0x14, 0x22, }, // x
{0x20, 0x12, 0xc, 0x10, 0x20, }, // y
{0x22, 0x26, 0x2a, 0x32, 0x22, }, // z
{0x0, 0x10, 0x6c, 0x82, 0x0, }, // {
{0x0, 0x0, 0xfe, 0x0, 0x0, }, // |
{0x0, 0x82, 0x6c, 0x10, 0x0, }, // }
{0x30, 0x40, 0x20, 0x10, 0x60, }, // ~
};
