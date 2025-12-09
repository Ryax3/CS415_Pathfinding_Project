#include "maze_generator.h"
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <iostream>

// SCALE controls wall/corridor thickness (instructor uses ~6px)
static const int SCALE = 6;

MazeGenerator::MazeGenerator(int cellRows, int cellCols)
    : cellRows_(cellRows), cellCols_(cellCols)
{
    // Base maze structure: (2N+1) x (2M+1)
    H_ = 2 * cellRows_ + 1;
    W_ = 2 * cellCols_ + 1;

    sr_ = 1; // suggested start in grid coords
    sc_ = 1;
    gr_ = H_ - 2; // suggested goal
    gc_ = W_ - 2;
}

// DFS recursive backtracker
void MazeGenerator::carve(int r, int c,
                          std::vector<std::vector<bool>> &isPath,
                          std::vector<std::vector<bool>> &visited)
{
    visited[r][c] = true;
    isPath[r][c] = true;

    static thread_local std::mt19937 rng(
        std::chrono::high_resolution_clock::now().time_since_epoch().count());

    const int dr[4] = {-2, 2, 0, 0};
    const int dc[4] = {0, 0, -2, 2};
    std::vector<int> dirs = {0, 1, 2, 3};
    std::shuffle(dirs.begin(), dirs.end(), rng);

    for (int k : dirs)
    {
        int nr = r + dr[k];
        int nc = c + dc[k];

        if (nr <= 0 || nr >= H_ - 1)
            continue;
        if (nc <= 0 || nc >= W_ - 1)
            continue;

        if (!visited[nr][nc])
        {
            // Knock down wall between (r,c) and (nr,nc)
            isPath[(r + nr) / 2][(c + nc) / 2] = true;

            carve(nr, nc, isPath, visited);
        }
    }
}

std::string MazeGenerator::generate(const std::string &filename)
{
    // Base maze grid
    std::vector<std::vector<bool>> isPath(H_, std::vector<bool>(W_, false));
    std::vector<std::vector<bool>> visited(H_, std::vector<bool>(W_, false));

    // Run DFS carving
    carve(1, 1, isPath, visited);
    isPath[sr_][sc_] = true;
    isPath[gr_][gc_] = true;

    // Create scaled image with thick walls like instructor maze
    BMPImage img;
    img.create(W_ * SCALE, H_ * SCALE, 0, 0, 0); // black background = walls

    // Paint white corridors
    for (int r = 0; r < H_; r++)
    {
        for (int c = 0; c < W_; c++)
        {
            if (isPath[r][c])
            {
                for (int dr = 0; dr < SCALE; dr++)
                    for (int dc = 0; dc < SCALE; dc++)
                        img.setPixel(r * SCALE + dr,
                                     c * SCALE + dc,
                                     255, 255, 255);
            }
        }
    }

    // Convert start/goal to scaled coordinates (center of corridor)
    sr_ = sr_ * SCALE + SCALE / 2;
    sc_ = sc_ * SCALE + SCALE / 2;
    gr_ = gr_ * SCALE + SCALE / 2;
    gc_ = gc_ * SCALE + SCALE / 2;

    img.save(filename);
    return filename;
}