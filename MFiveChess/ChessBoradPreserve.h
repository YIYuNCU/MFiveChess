#pragma once
#include <iostream>
#include <vector>
namespace Evian
{
    class CPoint
    {
    public:
        LONG x;
        LONG y;
        BOOL color;
        CPoint(LONG x0 = -1, LONG y0 = -1, BOOL color0 = true);
        static CPoint ERRPOINT();
        bool operator==(const CPoint& other)const
        {
            return (x == other.x) && (y == other.y);
        }
        bool operator!=(const CPoint& other)const
        {
            return (x != other.x) || (y != other.y);
        }
    };
}
typedef struct {
    Evian::CPoint* points;
    int num_points;
    int capacity;
} ChessBoardPreserve;

void ChessBoardPreserve_init(ChessBoardPreserve* board);
Evian::CPoint ChessBoardPreserve_add_point(ChessBoardPreserve* board, Evian::CPoint point, bool color);
void ChessBoardPreserve_destroy(ChessBoardPreserve* board);
bool check_win_condition(const ChessBoardPreserve* board, bool color);
bool getChessColorAt(const ChessBoardPreserve* board, int x, int y, bool& color);
Evian::CPoint ChessBoardPreserve_remove_last_point(ChessBoardPreserve* board);