#include "maze_generator.h"
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <iostream>

MazeGenerator::MazeGenerator(int cellRows, int cellCols)
    : cellRows_(cellRows), cellCols_(cellCols)
{
    H_ = 2 * cellRows_ + 1;
    W_ = 2 * cellCols_ + 1;
    sr_ = 1;
    sc_ = 1;
    gr_ = H_ - 2;
    gc_ = W_ - 2;
}

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
    std::vector<int> order = {0, 1, 2, 3};
    std::shuffle(order.begin(), order.end(), rng);

    for (int k : order)
    {
        int nr = r + dr[k];
        int nc = c + dc[k];

        if (nr <= 0 || nr >= H_ - 1)
            continue;
        if (nc <= 0 || nc >= W_ - 1)
            continue;

        if (!visited[nr][nc])
        {
            isPath[(r + nr) / 2][(c + nc) / 2] = true;
            carve(nr, nc, isPath, visited);
        }
    }
}

std::string MazeGenerator::generate(const std::string &filename)
{
    std::vector<std::vector<bool>> isPath(H_, std::vector<bool>(W_, false));
    std::vector<std::vector<bool>> visited(H_, std::vector<bool>(W_, false));

    carve(1, 1, isPath, visited);
    isPath[sr_][sc_] = true;
    isPath[gr_][gc_] = true;

    BMPImage img;
    img.create(W_, H_, 0, 0, 0);

    for (int r = 0; r < H_; r++)
        for (int c = 0; c < W_; c++)
            if (isPath[r][c])
                img.setPixel(r, c, 255, 255, 255);

    img.save(filename);
    return filename;
}