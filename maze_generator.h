#ifndef MAZE_GENERATOR_H
#define MAZE_GENERATOR_H

#include "bmp.h"
#include <string>

class MazeGenerator
{
public:
    MazeGenerator(int cellRows = 25, int cellCols = 25);

    std::string generate(const std::string &filename);

    int start_row() const { return sr_; }
    int start_col() const { return sc_; }
    int goal_row() const { return gr_; }
    int goal_col() const { return gc_; }

private:
    int cellRows_, cellCols_;
    int H_, W_;
    int sr_, sc_, gr_, gc_;

    void carve(int r, int c,
               std::vector<std::vector<bool>> &isPath,
               std::vector<std::vector<bool>> &visited);
};

#endif