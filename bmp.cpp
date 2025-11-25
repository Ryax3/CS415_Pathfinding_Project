#include "bmp.h"

#include <fstream>
#include <iostream>
#include <cstring>

#pragma pack(push, 1)
struct BMPFileHeader
{
    uint16_t bfType;
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
};

struct BMPInfoHeader
{
    uint32_t biSize;
    int32_t biWidth;
    int32_t biHeight;
    uint16_t biPlanes;
    uint16_t biBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    int32_t biXPelsPerMeter;
    int32_t biYPelsPerMeter;
    uint32_t biClrUsed;
    uint32_t biClrImportant;
};
#pragma pack(pop)

// ---------------------------------------------------------

void BMPImage::create(int width, int height,
                      uint8_t r, uint8_t g, uint8_t b)
{
    width_ = width;
    height_ = height;
    pixels_.assign(width_ * height_ * 3, 0);

    for (int row = 0; row < height_; ++row)
        for (int col = 0; col < width_; ++col)
            setPixel(row, col, r, g, b);
}

// ---------------------------------------------------------

bool BMPImage::load(const std::string &filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (!in)
    {
        std::cerr << "❌ Cannot open BMP file: " << filename << "\n";
        return false;
    }

    BMPFileHeader fileHeader{};
    BMPInfoHeader infoHeader{};

    in.read(reinterpret_cast<char *>(&fileHeader), sizeof(fileHeader));
    in.read(reinterpret_cast<char *>(&infoHeader), sizeof(infoHeader));

    if (fileHeader.bfType != 0x4D42)
    {
        std::cerr << "❌ Not a valid BMP file.\n";
        return false;
    }

    if (infoHeader.biBitCount != 24 || infoHeader.biCompression != 0)
    {
        std::cerr << "❌ Only uncompressed 24-bit BMP supported.\n";
        return false;
    }

    width_ = infoHeader.biWidth;
    height_ = infoHeader.biHeight;

    bool bottomUp = height_ > 0;
    if (!bottomUp)
        height_ = -height_;

    const int rowPadded = ((width_ * 3 + 3) / 4) * 4;
    std::vector<uint8_t> rowBuf(rowPadded);

    pixels_.assign(width_ * height_ * 3, 0);

    in.seekg(fileHeader.bfOffBits, std::ios::beg);

    for (int row = 0; row < height_; ++row)
    {
        int targetRow = bottomUp ? (height_ - 1 - row) : row;

        in.read((char *)rowBuf.data(), rowPadded);
        if (!in)
            return false;

        for (int col = 0; col < width_; ++col)
        {
            uint8_t B = rowBuf[col * 3 + 0];
            uint8_t G = rowBuf[col * 3 + 1];
            uint8_t R = rowBuf[col * 3 + 2];

            setPixel(targetRow, col, R, G, B);
        }
    }

    return true;
}

// ---------------------------------------------------------

bool BMPImage::save(const std::string &filename) const
{
    if (width_ <= 0 || height_ <= 0)
    {
        std::cerr << "❌ Cannot save empty BMP\n";
        return false;
    }

    std::ofstream out(filename, std::ios::binary);
    if (!out)
    {
        std::cerr << "❌ Cannot write BMP: " << filename << "\n";
        return false;
    }

    const int rowPadded = ((width_ * 3 + 3) / 4) * 4;
    uint32_t pixelSize = rowPadded * height_;

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

// ---------------------------------------------------------

void BMPImage::getPixel(int row, int col,
                        uint8_t &r, uint8_t &g, uint8_t &b) const
{
    int idx = (row * width_ + col) * 3;
    r = pixels_[idx + 0];
    g = pixels_[idx + 1];
    b = pixels_[idx + 2];
}

void BMPImage::setPixel(int row, int col,
                        uint8_t r, uint8_t g, uint8_t b)
{
    int idx = (row * width_ + col) * 3;
    pixels_[idx + 0] = r;
    pixels_[idx + 1] = g;
    pixels_[idx + 2] = b;
}