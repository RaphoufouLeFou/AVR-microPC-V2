from PIL import Image

image = Image.open("src/font/Chars.png")
pixels = image.load()

CharWidth = 5
CharHeight = 7
imageWidth = 155
imageHeight = 31
charSpacing = 1

"""Output format:

const uint8_t font[] = {
{0x00, 0x00, 0x00, 0x00, 0x00}, // space
{0x00, 0x00, 0x5F, 0x00, 0x00}, // !
...
};
in the ascii order, with space for empty characters
"""

"""
Char order in the image:
ABCDEFGHIJKLMNOPQRSTUVWXYZ
1234567890!"#@%&'()~+-/*=.
,{}?$ <>:\\[] &|^;
abcdefghijklmnopqrstuvwxyz
"""
AsciiToChar = [
    # ascii, Char number x (0-25),  Char number y (0-4)
    [32, 17, 2, " "], # space
    [33, 10, 1, "!"], # !
    [34, 11, 1, "\""], # "
    [35, 12, 1, "#"], # #
    [36, 5, 2, "€"], # €
    [37, 14, 1, "%"], # %
    [38, 15, 1, "&"], # &
    [39, 16, 1, "'"], # '
    [40, 17, 1, "("], # (
    [41, 18, 1, ")"], # )
    [42, 23, 1, "*"], # *
    [43, 20, 1, "+"], # +
    [44, 0, 2, ","], # ,
    [45, 21, 1, "-"], # -
    [46, 25, 1, "."], # .
    [47, 22, 1, "/"], # /
    [48, 9, 1, "0"], # 0
    [49, 0, 1, "1"], # 1
    [50, 1, 1, "2"], # 2
    [51, 2, 1, "3"], # 3
    [52, 3, 1, "4"], # 4
    [53, 4, 1, "5"], # 5
    [54, 5, 1, "6"], # 6
    [55, 6, 1, "7"], # 7
    [56, 7, 1, "8"], # 8
    [57, 8, 1, "9"], # 9
    [58, 8, 2, ":"], # :
    [59, 16, 2, ";"], # ;
    [60, 6, 1, "<"], # <
    [61, 24, 1, "="], # =
    [62, 7, 1, ">"], # >
    [63, 3, 2, "?"], # ?
    [64, 13, 1, "@"], # @
    [65, 0, 0, "A"], # A
    [66, 1, 0, "B"], # B
    [67, 2, 0, "C"], # C
    [68, 3, 0, "D"], # D
    [69, 4, 0, "E"], # E
    [70, 5, 0, "F"], # F
    [71, 6, 0, "G"], # G
    [72, 7, 0, "H"], # H
    [73, 8, 0, "I"], # I
    [74, 9, 0, "J"], # J
    [75, 10, 0, "K"], # K
    [76, 11, 0, "L"], # L
    [77, 12, 0, "M"], # M
    [78, 13, 0, "N"], # N
    [79, 14, 0, "O"], # O
    [80, 15, 0, "P"], # P
    [81, 16, 0, "Q"], # Q
    [82, 17, 0, "R"], # R
    [83, 18, 0, "S"], # S
    [84, 19, 0, "T"], # T
    [85, 20, 0, "U"], # U
    [86, 21, 0, "V"], # V
    [87, 22, 0, "W"], # W
    [88, 23, 0, "X"], # X
    [89, 24, 0, "Y"], # Y
    [90, 25, 0, "Z"], # Z
    [91, 10, 2, "["], # [
    [92, 9, 2, "\\"], # \
    [93, 11, 2, "]"], # ]
    [94, 15, 2, "^"], # ^
    [95, 12, 2, "_"], # _
    [96, 12, 2, "`"], # `
    [97, 0, 3, "a"], # a
    [98, 1, 3, "b"], # b
    [99, 2, 3, "c"], # c
    [100, 3, 3, "d"], # d
    [101, 4, 3, "e"], # e
    [102, 5, 3, "f"], # f
    [103, 6, 3, "g"], # g
    [104, 7, 3, "h"], # h
    [105, 8, 3, "i"], # i
    [106, 9, 3, "j"], # j
    [107, 10, 3, "k"], # k
    [108, 11, 3, "l"], # l
    [109, 12, 3, "m"], # m
    [110, 13, 3, "n"], # n
    [111, 14, 3, "o"], # o
    [112, 15, 3, "p"], # p
    [113, 16, 3, "q"], # q
    [114, 17, 3, "r"], # r
    [115, 18, 3, "s"], # s
    [116, 19, 3, "t"], # t
    [117, 20, 3, "u"], # u
    [118, 21, 3, "v"], # v
    [119, 22, 3, "w"], # w
    [120, 23, 3, "x"], # x
    [121, 24, 3, "y"], # y
    [122, 25, 3, "z"], # z
    [123, 1, 2, "{"], # {
    [124, 14, 2, "|"], # |
    [125, 2, 2, "}"], # }
    [126, 19, 1, "~"], # ~
]

def GetCharImg(x, y):
    char = []
    for xi in range(CharWidth):
        byte = 0
        for yj in range(CharHeight):
            byte = byte << 1
            if pixels[x + xi, y + yj] == (0, 0, 0, 255):
                byte = byte | 1
        byte = byte << 1
        char.append(byte)
    return char

def CharImgToHex(char, letter):
    hexStr = "{"
    for byte in char:
        hexStr += hex(byte) + ", "
    hexStr += "}, // " + letter 
    return hexStr

def GetCharImgFromAsciiTable(ascii):
    for char in AsciiToChar:
        if char[0] == ascii:
            return (GetCharImg(char[1] * (CharWidth + charSpacing), char[2] * (CharHeight + charSpacing)), char[3])
    return GetCharImg(12, 2), "---"

for i in range(0, 127):
    char, letter = GetCharImgFromAsciiTable(i)
    print(CharImgToHex(char, letter))

