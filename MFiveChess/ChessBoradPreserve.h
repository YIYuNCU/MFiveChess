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
        CPoint(LONG x0, LONG y0, BOOL color0);
        static CPoint ERRPOINT();
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