CS 415 – Pathfinding Project (BFS & A*)
Sonoma State University – Algorithm Analysis – Fall 2025

Student Names:
Rolando Yax

--------------------------------------------------------

PROJECT OVERVIEW
This project implements two graph-search algorithms—Breadth-First Search (BFS)
and Best-First Search using the A* heuristic—to compute the shortest path 
between two walkable pixels in a BMP maze image. A pixel is considered walkable 
if at least one of its RGB components is bright (R > 100 or G > 100 or B > 100).

The program:
- Loads ANY valid 24-bit or 32-bit BMP file (instructor requirement)
- Accepts START and GOAL coordinates from the user
- Runs BFS and A*
- Outputs two BMP images showing visited nodes (green) and the final shortest path (red)

This updated version includes a fully compliant BMP loader that correctly reads 
any standard uncompressed BMP file, including instructor-provided test mazes.

--------------------------------------------------------
HOW THE PROJECT WORKS

1. Maze Input
   - The user can load *any* valid BMP file from the data/ directory.
   - Our BMP loader supports:
       • 24-bit and 32-bit BMPs  
       • BI_RGB and BI_BITFIELDS formats  
       • Top-down and bottom-up images  
       • Any header size (40, 108, 124 bytes)  
   - This update directly addresses the instructor feedback requiring support 
     for arbitrary BMP inputs, not just the three sample files.

2. Start/Goal Selection
   - User enters row/column coordinates for START and GOAL.
   - Program validates:
       • Coordinates are within bounds  
       • Chosen pixels are walkable  
       • Invalid entries are safely re-prompted  

3. Running BFS
   - Uses a FIFO queue to explore outward evenly.
   - Guarantees the shortest path in an unweighted grid.
   - All visited nodes are marked green; the final path is red.

4. Running A*
   - Uses f(u) = g(u) + h(u) with Manhattan distance as the heuristic.
   - A priority queue ensures faster, guided exploration.
   - Produces the same optimal path as BFS but visits fewer nodes.

5. Output
   - Saves two BMP images:
       • bfs_out.bmp
       • a_star_out.bmp
   - Both preserve the original maze colors while overlaying:
       • Green (visited nodes)
       • Red (shortest path)

--------------------------------------------------------
DIRECTORY STRUCTURE

CS415_Pathfinding_Project/
├── bmp.cpp / bmp.h               (BMP loader/saver)
├── search.cpp / search.h         (BFS & A* algorithms)
├── utils.cpp / utils.h           (Utility helpers)
├── maze_generator.cpp / .h       (Optional DFS maze generator)
├── main.cpp                      (Program menu + control logic)
└── data/
    ├── maze.bmp                  (Instructor test maze)
    ├── bfs_out.bmp               (Generated BFS result)
    └── a_star_out.bmp            (Generated A* result)

--------------------------------------------------------
COMPILATION

Compile with:

g++ -std=c++17 main.cpp bmp.cpp search.cpp utils.cpp maze_generator.cpp -o pathfinder

--------------------------------------------------------
RUNNING THE PROGRAM

Run:

./pathfinder

Main Menu:
1) Generate new maze
2) Load existing BMP maze
3) Exit

• Option 1: Creates a new maze and runs BFS & A*.  
• Option 2: Loads ANY BMP in data/ and runs search from user-selected coordinates.

--------------------------------------------------------
TESTING THE PROJECT

Test 1 — Instructor Test Image (Required)
1. Place maze.bmp in the data/ folder.
2. Run Option 2 → Load existing BMP.
3. Enter START = (10, 10), GOAL = (190, 190).
4. Save results as:
     data/bfs_out.bmp
     data/a_star_out.bmp
Expected:
- BFS explores many nodes (large green area)
- A* explores fewer nodes
- Both produce the same red shortest path

Test 2 — Maze Generation
1. Choose “Generate new maze.”
2. Program outputs suggested START/GOAL.
3. Run BFS & A* and save outputs.

Test 3 — Error Handling
Try:
- Out-of-bounds pixels
- Wall pixels
- Non-numeric input
The program detects and rejects invalid entries safely.

--------------------------------------------------------
EXPECTED OUTPUT CHARACTERISTICS

BFS:
- Wide green exploration pattern.
- Optimal red path from START to GOAL.

A*:
- More focused green region (heuristic-driven).
- Same optimal red path.

--------------------------------------------------------
AI USE STATEMENT

I used AI tools (ChatGPT) only to help explain concepts such as BFS, A*, 
BMP structure. All final code, debugging, and program logic were written and verified by me.

--------------------------------------------------------
CONCLUSION

I Do believe this project:
- Fully implements BFS and A* per CS415 specifications
- Includes a robust and general-purpose BMP loader
- Handles invalid input safely
- Produces clear and correct output visualizations
- Meets all instructor requirements for resubmission