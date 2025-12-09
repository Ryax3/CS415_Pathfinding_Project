// #include "utils.h"
// #include <cmath>

// bool in_bounds(const BMPImage &img, int r, int c)
// {
//     return r >= 0 && r < img.height() &&
//            c >= 0 && c < img.width();
// }

// // bool is_vertex(const BMPImage &img, int r, int c)
// // {
// //     uint8_t R, G, B;
// //     img.getPixel(r, c, R, G, B);
// //     return (R > 100 || G > 100 || B > 100);
// // }
// bool is_vertex(const BMPImage &img, int r, int c)
// {
//     uint8_t R, G, B;
//     img.getPixel(r, c, R, G, B);

//     // Walkable only if pixel is white (path)
//     return (R > 200 && G > 200 && B > 200);
// }

// int manhattan(const Pixel &a, const Pixel &b)
// {
//     return abs(a.row - b.row) + abs(a.col - b.col);
// }

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

//     // 1) Color ALL visited nodes GREEN (but NOT the start/goal)
//     for (int r = 0; r < H; r++)
//     {
//         for (int c = 0; c < W; c++)
//         {
//             if (visited[r][c])
//             {
//                 // leave start/goal untouched for now
//                 if (!(r == start.row && c == start.col) &&
//                     !(r == goal.row && c == goal.col))
//                 {
//                     img.setPixel(r, c, 0, 255, 0); // GREEN
//                 }
//             }
//         }
//     }

//     // 2) If no path → done
//     if (!reached)
//         return;

//     // 3) Color SHORTEST PATH RED (override green)
//     Pixel cur = goal;

//     while (true)
//     {
//         img.setPixel(cur.row, cur.col, 255, 0, 0); // RED

//         if (cur.row == start.row && cur.col == start.col)
//             break;

//         Pixel p = prev[cur.row][cur.col];
//         if (p.row < 0 || p.col < 0)
//             break;

//         cur = p;
//     }
// }

// #include "utils.h"
// #include <cmath>

// //-----------------------------------------------------
// // Helper: check boundaries
// //-----------------------------------------------------
// bool in_bounds(const BMPImage &img, int r, int c)
// {
//     return r >= 0 && r < img.height() &&
//            c >= 0 && c < img.width();
// }

// //-----------------------------------------------------
// // Walkable = white pixel (like instructions say)
// //-----------------------------------------------------
// bool is_vertex(const BMPImage &img, int r, int c)
// {
//     uint8_t R, G, B;
//     img.getPixel(r, c, R, G, B);
//     return (R > 200 && G > 200 && B > 200);
// }

// //-----------------------------------------------------
// int manhattan(const Pixel &a, const Pixel &b)
// {
//     return abs(a.row - b.row) + abs(a.col - b.col);
// }

// //-----------------------------------------------------
// // NEW: Draws 3×3 thick red without overwriting walls
// //-----------------------------------------------------
// static void draw_thick_red(BMPImage &img, int r, int c)
// {
//     int H = img.height();
//     int W = img.width();

//     for (int dr = -1; dr <= 1; ++dr)
//     {
//         for (int dc = -1; dc <= 1; ++dc)
//         {
//             int nr = r + dr;
//             int nc = c + dc;

//             if (!in_bounds(img, nr, nc))
//                 continue;

//             uint8_t R, G, B;
//             img.getPixel(nr, nc, R, G, B);

//             // skip walls (black)
//             if (R < 50 && G < 50 && B < 50)
//                 continue;

//             img.setPixel(nr, nc, 255, 0, 0); // RED
//         }
//     }
// }

// //-----------------------------------------------------
// // Color visited (green) and shortest path (red)
// //-----------------------------------------------------
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

//     // 1) Paint visited nodes green (except start/goal)
//     for (int r = 0; r < H; r++)
//     {
//         for (int c = 0; c < W; c++)
//         {
//             if (visited[r][c])
//             {
//                 if (!(r == start.row && c == start.col) &&
//                     !(r == goal.row && c == goal.col))
//                 {
//                     img.setPixel(r, c, 0, 255, 0); // GREEN
//                 }
//             }
//         }
//     }

//     // 2) If no path found, done
//     if (!reached)
//         return;

//     // 3) THICK RED SHORTEST PATH
//     Pixel cur = goal;

//     while (true)
//     {
//         draw_thick_red(img, cur.row, cur.col);

//         if (cur.row == start.row && cur.col == start.col)
//             break;

//         Pixel p = prev[cur.row][cur.col];
//         if (p.row < 0 || p.col < 0)
//             break;

//         cur = p;
//     }
// }

// #include "utils.h"
// #include <cmath>

// bool in_bounds(const BMPImage &img, int r, int c)
// {
//     return r >= 0 && r < img.height() &&
//            c >= 0 && c < img.width();
// }

// bool is_vertex(const BMPImage &img, int r, int c)
// {
//     uint8_t R, G, B;
//     img.getPixel(r, c, R, G, B);
//     return (R > 200 && G > 200 && B > 200);
// }

// int manhattan(const Pixel &a, const Pixel &b)
// {
//     return abs(a.row - b.row) + abs(a.col - b.col);
// }

// // -----------------------------------------------
// // NEW: Brush sizes identical to instructor
// // -----------------------------------------------
// static const int GREEN_BRUSH = 4; // thickness for visited
// static const int RED_BRUSH = 6;   // thickness for shortest path

// // -----------------------------------------------
// // Helper: paint a thick square of a given color
// // -----------------------------------------------
// static void paint_block(BMPImage &img, int r, int c,
//                         int size,
//                         uint8_t R, uint8_t G, uint8_t B)
// {
//     int half = size / 2;

//     for (int dr = -half; dr <= half; dr++)
//     {
//         for (int dc = -half; dc <= half; dc++)
//         {
//             int nr = r + dr;
//             int nc = c + dc;

//             if (!in_bounds(img, nr, nc))
//                 continue;

//             // do NOT paint walls (black)
//             uint8_t rr, gg, bb;
//             img.getPixel(nr, nc, rr, gg, bb);
//             if (rr < 50 && gg < 50 && bb < 50)
//                 continue;

//             img.setPixel(nr, nc, R, G, B);
//         }
//     }
// }

// // -----------------------------------------------
// // MAIN COLORING — matches instructor style
// // -----------------------------------------------
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

//     // 1) Paint visited GREEN (thick)
//     for (int r = 0; r < H; r++)
//     {
//         for (int c = 0; c < W; c++)
//         {
//             if (visited[r][c])
//             {
//                 if (!((r == start.row && c == start.col) ||
//                       (r == goal.row && c == goal.col)))
//                 {
//                     paint_block(img, r, c, GREEN_BRUSH, 0, 255, 0);
//                 }
//             }
//         }
//     }

//     // No path → stop here
//     if (!reached)
//         return;

//     // 2) Paint shortest path RED (very thick)
//     Pixel cur = goal;

//     while (true)
//     {
//         paint_block(img, cur.row, cur.col, RED_BRUSH, 255, 0, 0);

//         if (cur.row == start.row && cur.col == start.col)
//             break;

//         Pixel p = prev[cur.row][cur.col];
//         if (p.row < 0 || p.col < 0)
//             break;

//         cur = p;
//     }
// }

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