
#ifndef UTILS_H
#define UTILS_H

#include "bmp.h"
#include <vector>
#include <cstdint>

struct Pixel
{
    int row;
    int col;
};

bool in_bounds(const BMPImage &img, int r, int c);
bool is_vertex(const BMPImage &img, int r, int c);
int manhattan(const Pixel &a, const Pixel &b);

void color_visited_and_path(
    BMPImage &img,
    const std::vector<std::vector<bool>> &visited,
    const std::vector<std::vector<Pixel>> &prev,
    const Pixel &start,
    const Pixel &goal,
    bool reached);

#endif