#include "utils.h"
#include <cmath>

bool in_bounds(const BMPImage &img, int r, int c)
{
    return r >= 0 && r < img.height() &&
           c >= 0 && c < img.width();
}

bool is_vertex(const BMPImage &img, int r, int c)
{
    uint8_t R, G, B;
    img.getPixel(r, c, R, G, B);
    return (R > 100 || G > 100 || B > 100);
}

int manhattan(const Pixel &a, const Pixel &b)
{
    return abs(a.row - b.row) + abs(a.col - b.col);
}

// void color_visited_and_path(
//     BMPImage &img,
//     const std::vector<std::vector<bool>> &visited,
//     const std::vector<std::vector<Pixel>> &prev,
//     const Pixel &start,
//     const Pixel &goal,
//     bool reached)
// {
//     int H = img.height();
//     int W = img.width();

//     // Green visited
//     for (int r = 0; r < H; r++)
//         for (int c = 0; c < W; c++)
//             if (visited[r][c])
//                 img.setPixel(r, c, 0, 255, 0);

//     if (!reached)
//         return;

//     // Red path
//     Pixel cur = goal;
//     while (!(cur.row == start.row && cur.col == start.col))
//     {
//         img.setPixel(cur.row, cur.col, 255, 0, 0);
//         Pixel p = prev[cur.row][cur.col];
//         if (p.row < 0)
//             break;
//         cur = p;
//     }
//     img.setPixel(start.row, start.col, 255, 0, 0);
// }

void color_visited_and_path(
    BMPImage &img,
    const std::vector<std::vector<bool>> &visited,
    const std::vector<std::vector<Pixel>> &prev,
    const Pixel &start,
    const Pixel &goal,
    bool reached)
{
    int H = img.height();
    int W = img.width();

    // ----------------------------------------------------------
    // 1) Color ALL visited nodes GREEN (but NOT the start/goal)
    // ----------------------------------------------------------
    for (int r = 0; r < H; r++)
    {
        for (int c = 0; c < W; c++)
        {
            if (visited[r][c])
            {
                // leave start/goal untouched for now
                if (!(r == start.row && c == start.col) &&
                    !(r == goal.row && c == goal.col))
                {
                    img.setPixel(r, c, 0, 255, 0); // GREEN
                }
            }
        }
    }

    // ----------------------------------------------------------
    // 2) If no path → done
    // ----------------------------------------------------------
    if (!reached)
        return;

    // ----------------------------------------------------------
    // 3) Color SHORTEST PATH RED (override green)
    // ----------------------------------------------------------
    Pixel cur = goal;

    while (true)
    {
        img.setPixel(cur.row, cur.col, 255, 0, 0); // RED

        if (cur.row == start.row && cur.col == start.col)
            break;

        Pixel p = prev[cur.row][cur.col];
        if (p.row < 0 || p.col < 0)
            break;

        cur = p;
    }
}
