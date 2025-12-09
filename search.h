#ifndef SEARCH_H
#define SEARCH_H

#include "bmp.h"
#include "utils.h"

#include <vector>

struct SearchResult
{
    int distance = -1; // -1 means "no path"
    bool reached = false;
    std::vector<std::vector<bool>> visited;
    std::vector<std::vector<Pixel>> prev;
};

// Breadth-First Search (unweighted shortest path)
SearchResult run_bfs(const BMPImage &img, const Pixel &start, const Pixel &goal);

// Best-First Search (A* with Manhattan heuristic)
SearchResult run_best_first(const BMPImage &img, const Pixel &start, const Pixel &goal);

#endif // SEARCH_H