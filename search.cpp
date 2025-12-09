#include "search.h"

#include <queue>
#include <limits>
#include <cmath>

namespace
{
    constexpr int INF = std::numeric_limits<int>::max();

    struct Node
    {
        int row;
        int col;
        int f; // f = g + h
    };

    struct NodeCompare
    {
        bool operator()(const Node &a, const Node &b) const
        {
            return a.f > b.f; // min-heap
        }
    };
}

// BFS

SearchResult run_bfs(const BMPImage &img, const Pixel &start, const Pixel &goal)
{
    const int H = img.height();
    const int W = img.width();

    SearchResult result;
    result.visited.assign(H, std::vector<bool>(W, false));
    result.prev.assign(H, std::vector<Pixel>(W, Pixel{-1, -1}));

    if (start.row == goal.row && start.col == goal.col)
    {
        if (!in_bounds(img, start.row, start.col) || !is_vertex(img, start.row, start.col))
        {
            result.distance = -1;
            result.reached = false;
            return result;
        }
        result.visited[start.row][start.col] = true;
        result.distance = 0;
        result.reached = true;
        return result;
    }

    std::queue<Pixel> q;
    std::vector<std::vector<int>> dist(H, std::vector<int>(W, INF));

    q.push(start);
    result.visited[start.row][start.col] = true;
    dist[start.row][start.col] = 0;

    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    while (!q.empty())
    {
        Pixel u = q.front();
        q.pop();

        if (u.row == goal.row && u.col == goal.col)
            break;

        for (int k = 0; k < 4; k++)
        {
            int nr = u.row + dr[k];
            int nc = u.col + dc[k];

            if (!in_bounds(img, nr, nc))
                continue;
            if (!is_vertex(img, nr, nc))
                continue;
            if (result.visited[nr][nc])
                continue;

            result.visited[nr][nc] = true;
            dist[nr][nc] = dist[u.row][u.col] + 1;
            result.prev[nr][nc] = u;
            q.push(Pixel{nr, nc});
        }
    }

    if (!result.visited[goal.row][goal.col])
    {
        result.distance = -1;
        result.reached = false;
        return result;
    }

    result.distance = dist[goal.row][goal.col];
    result.reached = true;
    return result;
}

// Best-First Search (A*)

SearchResult run_best_first(const BMPImage &img, const Pixel &start, const Pixel &goal)
{
    const int H = img.height();
    const int W = img.width();

    SearchResult result;
    result.visited.assign(H, std::vector<bool>(W, false));
    result.prev.assign(H, std::vector<Pixel>(W, Pixel{-1, -1}));

    if (start.row == goal.row && start.col == goal.col)
    {
        if (!in_bounds(img, start.row, start.col) || !is_vertex(img, start.row, start.col))
        {
            result.distance = -1;
            result.reached = false;
            return result;
        }
        result.visited[start.row][start.col] = true;
        result.distance = 0;
        result.reached = true;
        return result;
    }

    std::priority_queue<Node, std::vector<Node>, NodeCompare> pq;
    std::vector<std::vector<int>> g(H, std::vector<int>(W, INF));

    g[start.row][start.col] = 0;
    int h0 = manhattan(start, goal);
    pq.push(Node{start.row, start.col, 0 + h0});

    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    while (!pq.empty())
    {
        Node cur = pq.top();
        pq.pop();

        int r = cur.row;
        int c = cur.col;

        // Ignore outdated PQ entries
        int expected_f = g[r][c] + manhattan(Pixel{r, c}, goal);
        if (cur.f != expected_f)
            continue;

        // Finalize node
        if (result.visited[r][c])
            continue;

        result.visited[r][c] = true;

        // Goal reached
        if (r == goal.row && c == goal.col)
        {
            result.distance = g[r][c];
            result.reached = true;
            return result;
        }

        // Expand neighbors
        for (int k = 0; k < 4; k++)
        {
            int nr = r + dr[k];
            int nc = c + dc[k];

            if (!in_bounds(img, nr, nc))
                continue;
            if (!is_vertex(img, nr, nc))
                continue;

            int new_g = g[r][c] + 1;
            if (new_g < g[nr][nc])
            {
                g[nr][nc] = new_g;
                result.prev[nr][nc] = Pixel{r, c};

                int fval = new_g + manhattan(Pixel{nr, nc}, goal);
                pq.push(Node{nr, nc, fval});
            }
        }
    }

    result.distance = -1;
    result.reached = false;
    return result;
}