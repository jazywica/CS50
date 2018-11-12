// BMP-related data types based on Microsoft's own

#include <stdint.h>

// aliases for C/C++ primitive data types
// https://msdn.microsoft.com/en-us/library/cc230309.aspx
typedef uint8_t  BYTE; // unsigned char (8 bits)
typedef uint32_t DWORD; // unsigned int (32 bits)
typedef int32_t  LONG; // int (32 bits)
typedef uint16_t WORD; // unsigned short (16 bits)

// information about the type, size, and layout of a file
// https://msdn.microsoft.com/en-us/library/dd183374(v=vs.85).aspx
typedef struct
{
    WORD bfType; // 2 bytes
    DWORD bfSize; // 4 bytes
    WORD bfReserved1; // 2 bytes
    WORD bfReserved2; // 2 bytes
    DWORD bfOffBits; // 4 bytes
} __attribute__((__packed__))
BITMAPFILEHEADER; // this struct will be of size 14 (bytes) - FIRST PART OF THE .BMP HEADER

// information about the dimensions and color format
// https://msdn.microsoft.com/en-us/library/dd183376(v=vs.85).aspx
typedef struct
{
    DWORD biSize; // 4 bytes
    LONG biWidth; // 4 bytes
    LONG biHeight; // 4 bytes
    WORD biPlanes; // 2 bytes
    WORD biBitCount; // 2 bytes
    DWORD biCompression; // 4 bytes
    DWORD biSizeImage; // 4 bytes
    LONG biXPelsPerMeter; // 4 bytes
    LONG biYPelsPerMeter; // 4 bytes
    DWORD biClrUsed; // 4 bytes
    DWORD biClrImportant; // 4 bytes
} __attribute__((__packed__))
BITMAPINFOHEADER; // this struct will be of size 40 (bytes) - SECOND PART OF THE .BMP HEADER

// relative intensities of red, green, and blue
// https://msdn.microsoft.com/en-us/library/dd162939(v=vs.85).aspx
typedef struct
{
    BYTE rgbtBlue; // 1 byte
    BYTE rgbtGreen; // 1 byte
    BYTE rgbtRed; // 1 byte
} __attribute__((__packed__))
RGBTRIPLE; // this struct will be of size 3 (bytes)
