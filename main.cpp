#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <limits>

#include "bmp.h"
#include "utils.h"
#include "search.h"
#include "maze_generator.h"

namespace fs = std::filesystem;

// Helper: Clear bad cin input
void clearInput()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Helper: List .bmp files in /data
std::vector<std::string> listBMP(const std::string &folder)
{
    std::vector<std::string> files;
    if (!fs::exists(folder))
        return files;

    for (auto &e : fs::directory_iterator(folder))
    {
        if (!e.is_regular_file())
            continue;
        auto name = e.path().filename().string();
        if (name.size() >= 4 &&
            name.substr(name.size() - 4) == ".bmp")
            files.push_back(name);
    }
    sort(files.begin(), files.end());
    return files;
}

// Helper: Ask user for valid walkable pixel
Pixel askPixel(const std::string &label, const BMPImage &img)
{
    Pixel p;
    while (true)
    {
        std::cout << "Enter " << label << " pixel (row col): ";
        std::cin >> p.row >> p.col;

        if (std::cin.fail())
        {
            clearInput();
            std::cout << "❌ Invalid input. Enter two integers.\n";
            continue;
        }
        if (!in_bounds(img, p.row, p.col))
        {
            std::cout << "❌ Pixel out of bounds.\n";
            continue;
        }
        if (!is_vertex(img, p.row, p.col))
        {
            std::cout << "❌ Pixel is NOT walkable (too dark/wall).\n";
            continue;
        }
        break;
    }
    return p;
}

// Run BFS and A*
void run_pathfinding(const BMPImage &img)
{
    Pixel start = askPixel("START", img);
    Pixel goal = askPixel("GOAL", img);

    std::string bfsOut, astarOut;
    std::cout << "\nEnter BFS output filename  (e.g., data/bfs_out.bmp): ";
    std::cin >> bfsOut;
    std::cout << "Enter A* output filename   (e.g., data/a_star_out.bmp): ";
    std::cin >> astarOut;

    // --- BFS ---
    SearchResult bfsRes = run_bfs(img, start, goal);
    BMPImage bfsImg = img;

    color_visited_and_path(bfsImg, bfsRes.visited, bfsRes.prev,
                           start, goal, bfsRes.reached);

    bfsImg.save(bfsOut);

    // --- A* ---
    SearchResult aRes = run_best_first(img, start, goal);
    BMPImage aImg = img;

    color_visited_and_path(aImg, aRes.visited, aRes.prev,
                           start, goal, aRes.reached);

    aImg.save(astarOut);

    std::cout << "\n----------------------------------------\n";
    std::cout << "                 RESULTS\n";
    std::cout << "----------------------------------------\n";
    std::cout << "BFS Distance:        " << bfsRes.distance << "\n";
    std::cout << "A* Distance:         " << aRes.distance << "\n";
    std::cout << "\nImages saved:\n";
    std::cout << "  ✔ " << bfsOut << "\n";
    std::cout << "  ✔ " << astarOut << "\n";

    if (!bfsRes.reached || !aRes.reached)
    {
        std::cout << "\n⚠ WARNING: No path found between start and goal.\n";
    }

    std::cout << "----------------------------------------\n\n";
}

// Main menu
int main()
{
    const std::string dataFolder = "data";
    fs::create_directories(dataFolder);

    std::cout << "========================================\n";
    std::cout << "      CS415 PATHFINDING PROJECT\n";
    std::cout << "========================================\n\n";

    while (true)
    {
        std::cout << "\n=============== MAIN MENU ===============\n";
        std::cout << "1) Generate new maze\n";
        std::cout << "2) Load existing BMP maze\n";
        std::cout << "3) Exit\n";
        std::cout << "Choose an option: ";

        int choice;
        std::cin >> choice;

        if (std::cin.fail())
        {
            clearInput();
            std::cout << "❌ Invalid option.\n";
            continue;
        }

        // OPTION 3 — EXIT
        if (choice == 3)
        {
            std::cout << "Goodbye!\n";
            return 0;
        }

        // OPTION 1 — Generate maze
        if (choice == 1)
        {
            std::string filename;
            std::cout << "Enter output BMP filename (e.g., data/maze_01.bmp): ";
            std::cin >> filename;

            MazeGenerator gen(25, 25); // 51x51 grid
            gen.generate(filename);

            std::cout << "----------------------------------------\n";
            std::cout << "Maze generated and saved to: " << filename << "\n";
            std::cout << "Suggested START: (" << gen.start_row()
                      << ", " << gen.start_col() << ")\n";
            std::cout << "Suggested GOAL:  (" << gen.goal_row()
                      << ", " << gen.goal_col() << ")\n";
            std::cout << "----------------------------------------\n";

            BMPImage img;
            img.load(filename);
            run_pathfinding(img);
        }

        // OPTION 2 — Load existing BMP
        else if (choice == 2)
        {
            auto files = listBMP(dataFolder);

            std::cout << "\nAvailable BMP files:\n";
            if (files.empty())
            {
                std::cout << "  (no .bmp files found in /data)\n";
                continue;
            }

            for (size_t i = 0; i < files.size(); ++i)
                std::cout << "  " << (i + 1) << ") " << files[i] << "\n";

            std::cout << "Choose a file number: ";

            int fnum;
            std::cin >> fnum;

            if (fnum < 1 || fnum > (int)files.size())
            {
                std::cout << "❌ Invalid selection.\n";
                continue;
            }

            std::string fullpath = dataFolder + "/" + files[fnum - 1];
            BMPImage img;

            if (!img.load(fullpath))
            {
                std::cout << "❌ Could not load file.\n";
                continue;
            }

            std::cout << "Loaded image: " << fullpath << "\n";
            run_pathfinding(img);
        }

        else
        {
            std::cout << "❌ Invalid option.\n";
        }
    }

    return 0;
}