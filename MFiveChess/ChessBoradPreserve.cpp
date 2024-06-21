#include "pch.h"
#include "ChessBoradPreserve.h" // 假设您的头文件中包含了 ChessBoardPreserve 类的声明


using namespace Evian;
// 初始化 ChessBoardPreserve
void ChessBoardPreserve_init(ChessBoardPreserve* board)
{
    board->points = NULL;
    board->num_points = 0;
    board->capacity = 0;
}
// 销毁 ChessBoardPreserve
void ChessBoardPreserve_destroy(ChessBoardPreserve* board) {
    free(board->points); // 释放动态分配的内存
    board->points = NULL;
    board->num_points = 0;
    board->capacity = 0;
}

// 辅助函数：检查某个方向上是否存在同色五子连珠
bool check_line(const ChessBoardPreserve* board, int x, int y, int dx, int dy, bool color)
{
    int count = 1; // 当前棋子已经计数为1（包括自身）
    int nx, ny;
    // 正向检查
    nx = x + dx;
    ny = y + dy;
    while (count < 5 && nx >= 0 && nx < 19 && ny >= 0 && ny < 19)
    {
        bool next_color;
        if (getChessColorAt(board, nx, ny, next_color))
        {
            if (next_color == color)
            {
                count++;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
        nx += dx;
        ny += dy;
    }

    // 反向检查
    nx = x - dx;
    ny = y - dy;
    while (count < 5 && nx >= 0 && nx < 19 && ny >= 0 && ny < 19)
    {
        bool next_color;
        if (getChessColorAt(board, nx, ny, next_color))
        {
            if (next_color == color)
            {
                count++;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
        nx -= dx;
        ny -= dy;
    }

    return count >= 5; // 返回是否存在五子连珠
}

// 主函数：检查某种棋子胜利条件
bool check_win_condition(const ChessBoardPreserve* board, bool color)
{
    int directions[4][2] = { {0, 1}, {1, 0}, {1, 1}, {-1, 1} };

    // 遍历每个棋子位置
    for (int i = 0; i < board->num_points; ++i)
    {
        int x = board->points[i].x;
        int y = board->points[i].y;

        // 检查当前棋子颜色是否为期望的颜色
        if (board->points[i].color != color)
        {
            continue; // 如果颜色不匹配，继续下一个棋子位置的检查
        }

        // 在四个方向上检查是否有五子连珠
        for (int d = 0; d < 4; ++d)
        {
            if (check_line(board, x, y, directions[d][0], directions[d][1], color))
            {
                return true; // 发现五子连珠，返回 true
            }
        }
    }

    return false; // 未找到五子连珠，返回 false
}

Evian::CPoint ChessBoardPreserve_add_point(ChessBoardPreserve* board, Evian::CPoint point, bool color)
{
    // 检查新的点是否已经存在
    for (int i = 0; i < board->num_points; ++i)
    {
        if (board->points[i].x == point.x && board->points[i].y == point.y)
        {
            return Evian::CPoint::ERRPOINT(); // 如果点已经存在，则返回错误点
        }
    }

    // 扩展容量
    if (board->num_points >= board->capacity)
    {
        int new_capacity = (board->capacity == 0) ? 1 : board->capacity * 2;
        Evian::CPoint* new_points = (Evian::CPoint*)realloc(board->points, new_capacity * sizeof(Evian::CPoint));
        if (new_points == NULL)
        {
            return Evian::CPoint::ERRPOINT(); // 处理内存分配失败情况
        }
        board->points = new_points;
        board->capacity = new_capacity;
    }

    // 将新的点添加到数组中
    board->points[board->num_points] = point;
    board->points[board->num_points].color = color;
    board->num_points++;
    return point; // 成功添加
}

bool getChessColorAt(const ChessBoardPreserve* board, int x, int y, bool& color)
{
    for (int j = 0; j < board->num_points; ++j)
    {
        if (board->points[j].x == x && board->points[j].y == y)
        {
            color = board->points[j].color;
            return true; // 找到对应位置的棋子，返回其颜色
        }
    }
    return false; // 找不到对应位置的棋子，返回 false
}

Evian::CPoint::CPoint(LONG x0 = -1,LONG y0 = -1,BOOL color0 = false)
{
    this->x = x0;
    this->y = y0;
    this->color = color0;
}

Evian::CPoint Evian::CPoint::ERRPOINT()
{
    return Evian::CPoint();
}
