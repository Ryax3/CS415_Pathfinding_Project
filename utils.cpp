#include "utils.h"
#include <cmath>

// ---------------------------------------------------------
// in_bounds
// ---------------------------------------------------------
bool in_bounds(const BMPImage &img, int r, int c)
{
    return r >= 0 && r < img.height() &&
           c >= 0 && c < img.width();
}

// ---------------------------------------------------------
// is_vertex – walkable only if pixel is white (corridor)
// ---------------------------------------------------------
bool is_vertex(const BMPImage &img, int r, int c)
{
    uint8_t R, G, B;
    img.getPixel(r, c, R, G, B);

    // Must be white corridor
    return (R > 200 && G > 200 && B > 200);
}

// ---------------------------------------------------------
// Manhattan Distance
// ---------------------------------------------------------
int manhattan(const Pixel &a, const Pixel &b)
{
    return std::abs(a.row - b.row) + std::abs(a.col - b.col);
}

// ---------------------------------------------------------
// Helper: treat near-black as walls
// ---------------------------------------------------------
static bool is_wall(uint8_t R, uint8_t G, uint8_t B)
{
    // your maze walls are solid black; small threshold is safe
    return (R < 50 && G < 50 && B < 50);
}

// ---------------------------------------------------------
// Helper: Thicken a path pixel into a full red corridor
//
// We color a 9×9 box centered on (r,c), but ONLY on
// non-wall pixels (so we never paint through black walls).
//
// This makes the red ribbon fill the entire corridor width,
// similar to the instructor's example.
// ---------------------------------------------------------

static void thicken_path_pixel(BMPImage &img, int r, int c)
{
    const int radius = 4;                   // 9x9 neighborhood
    const uint8_t RR = 255, GG = 0, BB = 0; // RED

    for (int dr = -radius; dr <= radius; ++dr)
    {
        for (int dc = -radius; dc <= radius; ++dc)
        {
            int nr = r + dr;
            int nc = c + dc;

            if (!in_bounds(img, nr, nc))
                continue;

            uint8_t R, G, B;
            img.getPixel(nr, nc, R, G, B);

            // Do NOT paint walls (keep black borders)
            if (!is_wall(R, G, B))
            {
                img.setPixel(nr, nc, RR, GG, BB);
            }
        }
    }
}

// ---------------------------------------------------------
// color_visited_and_path
// ---------------------------------------------------------
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

    // -----------------------------------------------------
    // 1) Color all visited nodes GREEN (but not walls)
    // -----------------------------------------------------
    for (int r = 0; r < H; r++)
    {
        for (int c = 0; c < W; c++)
        {
            if (!visited[r][c])
                continue;

            // leave start/goal for the path stage
            if ((r == start.row && c == start.col) ||
                (r == goal.row && c == goal.col))
                continue;

            uint8_t R, G, B;
            img.getPixel(r, c, R, G, B);

            // Only paint white corridor as green; never walls
            if (R > 200 && G > 200 && B > 200)
            {
                img.setPixel(r, c, 0, 255, 0); // GREEN
            }
        }
    }

    // -----------------------------------------------------
    // 2) If no path found → stop here
    // -----------------------------------------------------
    if (!reached)
        return;

    // -----------------------------------------------------
    // 3) Draw the shortest path using a thick red ribbon
    // -----------------------------------------------------
    Pixel cur = goal;

    while (true)
    {
        thicken_path_pixel(img, cur.row, cur.col);

        if (cur.row == start.row && cur.col == start.col)
            break;

        Pixel p = prev[cur.row][cur.col];
        if (p.row < 0 || p.col < 0)
            break; // safety, should not normally happen

        cur = p;
    }
}