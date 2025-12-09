#ifndef BMP_H
#define BMP_H

#include <string>
#include <vector>
#include <cstdint>

class BMPImage
{
public:
    BMPImage() : width_(0), height_(0) {}

    bool load(const std::string &filename);
    bool save(const std::string &filename) const;

    void create(int width, int height, uint8_t r, uint8_t g, uint8_t b);

    int width() const { return width_; }
    int height() const { return height_; }

    void getPixel(int row, int col, uint8_t &r, uint8_t &g, uint8_t &b) const;
    void setPixel(int row, int col, uint8_t r, uint8_t g, uint8_t b);

private:
    int width_, height_;
    std::vector<uint8_t> pixels_; // RGB format
};

#endif