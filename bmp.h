#ifndef BMP_H
#define BMP_H

#include <string>
#include <vector>
#include <cstdint>

class BMPImage
{
public:
    BMPImage() = default;

    // Create a new image filled with (r,g,b)
    void create(int width, int height,
                uint8_t r = 255, uint8_t g = 255, uint8_t b = 255);

    // Load BMP from disk
    bool load(const std::string &filename);

    // Save BMP to disk
    bool save(const std::string &filename) const;

    inline int width() const { return width_; }
    inline int height() const { return height_; }

    // Safe getters/setters
    void getPixel(int row, int col,
                  uint8_t &r, uint8_t &g, uint8_t &b) const;

    void setPixel(int row, int col,
                  uint8_t r, uint8_t g, uint8_t b);

private:
    int width_ = 0;
    int height_ = 0;
    std::vector<uint8_t> pixels_; // RGBRGBRGB...
};

#endif // BMP_H