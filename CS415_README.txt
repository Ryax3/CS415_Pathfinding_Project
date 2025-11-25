CS 415 – Pathfinding Project (BFS & A*)

Sonoma State University
Algorithm Analysis – Fall 2025
----------------------------------------

Project Overview
This project implements Breadth-First Search (BFS) and Best-First Search (A*) to find 
the shortest path between two pixels in a BMP maze image. Walkable pixels (R>100 or 
G>100 or B>100) are treated as vertices. Edges exist between 4-directional neighbors.

The program:
- Loads or generates a BMP maze
- Accepts start and goal coordinates
- Runs BFS and A*
- Generates two output BMP files
- Colors: Green = visited nodes, Red = shortest path

Directory Structure
CS415_Pathfinding_Project/
├── bmp.cpp / bmp.h
├── search.cpp / search.h
├── utils.cpp / utils.h
├── maze_generator.cpp / .h
├── main.cpp
└── data/
    ├── maze1.bmp
    ├── BFS_OUTPUT.bmp
    └── A*_OUTPUT.bmp

Compile
g++ -std=c++17 main.cpp bmp.cpp search.cpp utils.cpp maze_generator.cpp -o pathfinder

Run
./pathfinder

Testing
1. Load existing maze from /data
2. Enter start and goal coordinates
3. Provide output filenames
4. Compare BFS_OUTPUT.bmp and A*_OUTPUT.bmp

Algorithms
BFS: FIFO queue, explores entire frontier, shortest path guaranteed.
A*: Priority queue, f = g + h, Manhattan heuristic, fewer nodes explored.

Expected Output
- BFS: many green visited pixels
- A*: fewer green nodes, same red shortest path

Error Handling
- Invalid input
- Non-walkable start/goal
- BMP format verification
- Missing files

Conclusion
Fully meets CS415 Project 3 requirements.
