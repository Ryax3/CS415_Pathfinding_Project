// #include "bmp.h"

// #include <fstream>
// #include <iostream>
// #include <cstring>

// #pragma pack(push, 1)
// struct BMPFileHeader
// {
//     uint16_t bfType;
//     uint32_t bfSize;
//     uint16_t bfReserved1;
//     uint16_t bfReserved2;
//     uint32_t bfOffBits;
// };

// struct BMPInfoHeader
// {
//     uint32_t biSize;
//     int32_t biWidth;
//     int32_t biHeight;
//     uint16_t biPlanes;
//     uint16_t biBitCount;
//     uint32_t biCompression;
//     uint32_t biSizeImage;
//     int32_t biXPelsPerMeter;
//     int32_t biYPelsPerMeter;
//     uint32_t biClrUsed;
//     uint32_t biClrImportant;
// };
// #pragma pack(pop)

// // ---------------------------------------------------------

// void BMPImage::create(int width, int height,
//                       uint8_t r, uint8_t g, uint8_t b)
// {
//     width_ = width;
//     height_ = height;
//     pixels_.assign(width_ * height_ * 3, 0);

//     for (int row = 0; row < height_; ++row)
//         for (int col = 0; col < width_; ++col)
//             setPixel(row, col, r, g, b);
// }

// // ---------------------------------------------------------

// // bool BMPImage::load(const std::string &filename)
// // {
// //     std::ifstream in(filename, std::ios::binary);
// //     if (!in)
// //     {
// //         std::cerr << "Cannot open BMP file: " << filename << "\n";
// //         return false;
// //     }

// //     BMPFileHeader fileHeader{};
// //     BMPInfoHeader infoHeader{};

// //     in.read(reinterpret_cast<char *>(&fileHeader), sizeof(fileHeader));
// //     in.read(reinterpret_cast<char *>(&infoHeader), sizeof(infoHeader));

// //     if (fileHeader.bfType != 0x4D42)
// //     {
// //         std::cerr << "Not a valid BMP file.\n";
// //         return false;
// //     }

// //     if (infoHeader.biBitCount != 24 || infoHeader.biCompression != 0)
// //     {
// //         std::cerr << "Only uncompressed 24-bit BMP supported.\n";
// //         return false;
// //     }

// //     width_ = infoHeader.biWidth;
// //     height_ = infoHeader.biHeight;

// //     bool bottomUp = height_ > 0;
// //     if (!bottomUp)
// //         height_ = -height_;

// //     const int rowPadded = ((width_ * 3 + 3) / 4) * 4;
// //     std::vector<uint8_t> rowBuf(rowPadded);

// //     pixels_.assign(width_ * height_ * 3, 0);

// //     in.seekg(fileHeader.bfOffBits, std::ios::beg);

// //     for (int row = 0; row < height_; ++row)
// //     {
// //         int targetRow = bottomUp ? (height_ - 1 - row) : row;

// //         in.read((char *)rowBuf.data(), rowPadded);
// //         if (!in)
// //             return false;

// //         for (int col = 0; col < width_; ++col)
// //         {
// //             uint8_t B = rowBuf[col * 3 + 0];
// //             uint8_t G = rowBuf[col * 3 + 1];
// //             uint8_t R = rowBuf[col * 3 + 2];

// //             setPixel(targetRow, col, R, G, B);
// //         }
// //     }

// //     return true;
// // }

// bool BMPImage::load(const std::string &filename)
// {
//     std::ifstream in(filename, std::ios::binary);
//     if (!in)
//     {
//         std::cerr << "Cannot open BMP file: " << filename << "\n";
//         return false;
//     }

//     BMPFileHeader fileHeader{};
//     BMPInfoHeader infoHeader{};

//     in.read(reinterpret_cast<char *>(&fileHeader), sizeof(fileHeader));
//     in.read(reinterpret_cast<char *>(&infoHeader), sizeof(infoHeader));

//     // Check BMP signature
//     if (fileHeader.bfType != 0x4D42)
//     {
//         std::cerr << "Not a valid BMP file.\n";
//         return false;
//     }

//     // Support only uncompressed formats (BI_RGB = 0)
//     // if (infoHeader.biCompression != 0)
//     // {
//     //     std::cerr << "Compressed BMP not supported.\n";
//     //     return false;
//     // }

//     // Allow uncompressed (BI_RGB = 0) and BI_BITFIELDS (3)
//     if (infoHeader.biCompression != 0 && infoHeader.biCompression != 3)
//     {
//         std::cerr << "Unsupported BMP compression: "
//                   << infoHeader.biCompression << "\n";
//         return false;
//     }

//     int bitCount = infoHeader.biBitCount;
//     if (bitCount != 24 && bitCount != 32)
//     {
//         std::cerr << "Unsupported BMP bit depth: " << bitCount << "\n";
//         return false;
//     }

//     width_ = infoHeader.biWidth;
//     height_ = infoHeader.biHeight;

//     bool topDown = false;
//     if (height_ < 0)
//     {
//         topDown = true;
//         height_ = -height_;
//     }

//     pixels_.assign(width_ * height_ * 3, 0);

//     // Row sizes
//     int bytesPerPixel = bitCount / 8;
//     int rowPadded = ((width_ * bytesPerPixel + 3) / 4) * 4;

//     std::vector<uint8_t> rowBuf(rowPadded);

//     in.seekg(fileHeader.bfOffBits, std::ios::beg);

//     for (int row = 0; row < height_; ++row)
//     {
//         int targetRow = topDown ? row : (height_ - 1 - row);

//         in.read(reinterpret_cast<char *>(rowBuf.data()), rowPadded);
//         if (!in)
//             return false;

//         for (int col = 0; col < width_; ++col)
//         {
//             uint8_t B = rowBuf[col * bytesPerPixel + 0];
//             uint8_t G = rowBuf[col * bytesPerPixel + 1];
//             uint8_t R = rowBuf[col * bytesPerPixel + 2];

//             setPixel(targetRow, col, R, G, B);
//         }
//     }

//     return true;
// }

// // ---------------------------------------------------------

// bool BMPImage::save(const std::string &filename) const
// {
//     if (width_ <= 0 || height_ <= 0)
//     {
//         std::cerr << "Cannot save empty BMP\n";
//         return false;
//     }

//     std::ofstream out(filename, std::ios::binary);
//     if (!out)
//     {
//         std::cerr << "Cannot write BMP: " << filename << "\n";
//         return false;
//     }

//     const int rowPadded = ((width_ * 3 + 3) / 4) * 4;
//     uint32_t pixelSize = rowPadded * height_;

//     BMPFileHeader fh{};
//     fh.bfType = 0x4D42;
//     fh.bfOffBits = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
//     fh.bfSize = fh.bfOffBits + pixelSize;

//     BMPInfoHeader ih{};
//     ih.biSize = sizeof(BMPInfoHeader);
//     ih.biWidth = width_;
//     ih.biHeight = height_;
//     ih.biPlanes = 1;
//     ih.biBitCount = 24;
//     ih.biCompression = 0;
//     ih.biSizeImage = pixelSize;

//     out.write((char *)&fh, sizeof(fh));
//     out.write((char *)&ih, sizeof(ih));

//     std::vector<uint8_t> row(rowPadded);

//     for (int r = height_ - 1; r >= 0; --r)
//     {
//         for (int c = 0; c < width_; ++c)
//         {
//             uint8_t R, G, B;
//             getPixel(r, c, R, G, B);
//             row[c * 3 + 0] = B;
//             row[c * 3 + 1] = G;
//             row[c * 3 + 2] = R;
//         }
//         std::fill(row.begin() + width_ * 3, row.end(), 0);
//         out.write((char *)row.data(), rowPadded);
//     }

//     return true;
// }

// // ---------------------------------------------------------

// void BMPImage::getPixel(int row, int col,
//                         uint8_t &r, uint8_t &g, uint8_t &b) const
// {
//     int idx = (row * width_ + col) * 3;
//     r = pixels_[idx + 0];
//     g = pixels_[idx + 1];
//     b = pixels_[idx + 2];
// }

// void BMPImage::setPixel(int row, int col,
//                         uint8_t r, uint8_t g, uint8_t b)
// {
//     int idx = (row * width_ + col) * 3;
//     pixels_[idx + 0] = r;
//     pixels_[idx + 1] = g;
//     pixels_[idx + 2] = b;
// }

#include "bmp.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>

#pragma pack(push, 1)
struct BMPFileHeader
{
    uint16_t bfType; // Must be 'BM'
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits; // Pixel data offset
};

struct BMPInfoHeader
{
    uint32_t biSize; // Header size (40, 108, 124)
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;    // 24 or 32
    uint32_t biCompression; // BI_RGB (0) or BI_BITFIELDS (3)
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};
#pragma pack(pop)

// =============================================================
//                       CREATE IMAGE
// =============================================================
void BMPImage::create(int width, int height, uint8_t r, uint8_t g, uint8_t b)
{
    width_ = width;
    height_ = height;
    pixels_.assign(width_ * height_ * 3, 0);

    for (int row = 0; row < height_; ++row)
        for (int col = 0; col < width_; ++col)
            setPixel(row, col, r, g, b);
}

// =============================================================
//                       LOAD BMP
// =============================================================
bool BMPImage::load(const std::string &filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (!in)
    {
        std::cerr << "Cannot open BMP file: " << filename << "\n";
        return false;
    }

    BMPFileHeader fileHeader{};
    BMPInfoHeader infoHeader{};

    in.read(reinterpret_cast<char *>(&fileHeader), sizeof(fileHeader));
    in.read(reinterpret_cast<char *>(&infoHeader), sizeof(infoHeader));

    // Validate BMP magic number ("BM")
    if (fileHeader.bfType != 0x4D42)
    {
        std::cerr << "Not a valid BMP file.\n";
        return false;
    }

    // Accept 24-bit and 32-bit BMP only
    if (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 32)
    {
        std::cerr << "Unsupported BMP bit depth: " << infoHeader.biBitCount << "\n";
        return false;
    }

    // Accept uncompressed RGB & BITFIELDS
    if (infoHeader.biCompression != 0 && infoHeader.biCompression != 3)
    {
        std::cerr << "Unsupported BMP compression: " << infoHeader.biCompression << "\n";
        return false;
    }

    int bitCount = infoHeader.biBitCount;
    int bytesPerPixel = bitCount / 8;

    width_ = infoHeader.biWidth;
    height_ = infoHeader.biHeight;

    bool topDown = false;
    if (height_ < 0)
    {
        topDown = true;
        height_ = -height_;
    }

    pixels_.assign(width_ * height_ * 3, 0);

    // =====================================================
    // ✔ FIX: Skip BITFIELDS masks when compression = 3
    // =====================================================
    if (bitCount == 32 && infoHeader.biCompression == 3)
    {
        int maskBytes = fileHeader.bfOffBits - sizeof(BMPFileHeader) - infoHeader.biSize;
        if (maskBytes > 0)
        {
            in.seekg(maskBytes, std::ios::cur);
        }
    }

    // Jump to pixel array
    in.seekg(fileHeader.bfOffBits, std::ios::beg);

    int rowSize = ((width_ * bytesPerPixel + 3) / 4) * 4;
    std::vector<uint8_t> rowBuf(rowSize);

    for (int row = 0; row < height_; ++row)
    {
        int targetRow = topDown ? row : (height_ - 1 - row);

        in.read(reinterpret_cast<char *>(rowBuf.data()), rowSize);
        if (!in)
        {
            std::cerr << "Unexpected end of BMP file.\n";
            return false;
        }

        for (int col = 0; col < width_; ++col)
        {
            uint8_t B = rowBuf[col * bytesPerPixel + 0];
            uint8_t G = rowBuf[col * bytesPerPixel + 1];
            uint8_t R = rowBuf[col * bytesPerPixel + 2];

            setPixel(targetRow, col, R, G, B);
        }
    }

    return true;
}

// =============================================================
//                    SAVE BMP (24-bit only)
// =============================================================
bool BMPImage::save(const std::string &filename) const
{
    if (width_ <= 0 || height_ <= 0)
    {
        std::cerr << "Cannot save empty BMP\n";
        return false;
    }

    std::ofstream out(filename, std::ios::binary);
    if (!out)
    {
        std::cerr << "Cannot write BMP: " << filename << "\n";
        return false;
    }

    const int rowPadded = ((width_ * 3 + 3) / 4) * 4;
    uint32_t pixelSize = height_ * rowPadded;

    BMPFileHeader fh{};
    fh.bfType = 0x4D42;
    fh.bfOffBits = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
    fh.bfSize = fh.bfOffBits + pixelSize;

    BMPInfoHeader ih{};
    ih.biSize = sizeof(BMPInfoHeader);
    ih.biWidth = width_;
    ih.biHeight = height_;
    ih.biPlanes = 1;
    ih.biBitCount = 24;
    ih.biCompression = 0;
    ih.biSizeImage = pixelSize;

    out.write((char *)&fh, sizeof(fh));
    out.write((char *)&ih, sizeof(ih));

    std::vector<uint8_t> row(rowPadded);

    for (int r = height_ - 1; r >= 0; --r)
    {
        for (int c = 0; c < width_; ++c)
        {
            uint8_t R, G, B;
            getPixel(r, c, R, G, B);
            row[c * 3 + 0] = B;
            row[c * 3 + 1] = G;
            row[c * 3 + 2] = R;
        }

        std::fill(row.begin() + width_ * 3, row.end(), 0);
        out.write((char *)row.data(), rowPadded);
    }

    return true;
}

// =============================================================
//                     PIXEL ACCESSORS
// =============================================================
void BMPImage::getPixel(int row, int col, uint8_t &r, uint8_t &g, uint8_t &b) const
{
    int idx = (row * width_ + col) * 3;
    r = pixels_[idx + 0];
    g = pixels_[idx + 1];
    b = pixels_[idx + 2];
}

void BMPImage::setPixel(int row, int col, uint8_t r, uint8_t g, uint8_t b)
{
    int idx = (row * width_ + col) * 3;
    pixels_[idx + 0] = r;
    pixels_[idx + 1] = g;
    pixels_[idx + 2] = b;
}