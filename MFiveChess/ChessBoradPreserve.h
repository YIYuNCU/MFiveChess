#pragma once
#include <iostream>
#include <vector>
/// <summary>
/// 用于重定义CPoint的命名空间，防止混淆标准库
/// </summary>
namespace Evian
{
    class CPoint
    {
    public:
        LONG x;
        LONG y;
        BOOL color;
        CPoint(LONG x0 = -1, LONG y0 = -1, BOOL color0 = true);
        CPoint(::CPoint, BOOL color0 = true);
        static CPoint ERRPOINT();
        bool operator==(const CPoint& other)const
        {
            return (x == other.x) && (y == other.y);
        }
        bool operator!=(const CPoint& other)const
        {
            return (x != other.x) || (y != other.y);
        }
        static ::CPoint GetStdCPoint(Evian::CPoint point);
    };
}
typedef struct {
    Evian::CPoint* points;
    int num_points;
    int capacity;
} ChessBoardPreserve;
/// <summary>
/// 棋盘初始化函数，用于将棋盘结构体初始化(points指向NULL,num和capacity均置为零)
/// </summary>
/// <param name="board">需要初始化的棋盘结构体</param>
void ChessBoardPreserve_init(ChessBoardPreserve* board);
/// <summary>
/// 用于向棋盘中加入点
/// </summary>
/// <param name="board">棋盘变量</param>
/// <param name="point">给定需要添加的点的信息</param>
/// <returns>如果能顺利加入点则返回对应坐标，否则返回ERRORPOINT</returns>
Evian::CPoint ChessBoardPreserve_add_point(ChessBoardPreserve* board, Evian::CPoint point);
/// <summary>
/// 用于销毁棋盘变量占用的内存空间
/// </summary>
/// <param name="board">需要销毁的棋盘变量</param>
void ChessBoardPreserve_destroy(ChessBoardPreserve* board);
/// <summary>
/// 判断给定的颜色是否获胜
/// </summary>
/// <returns>给定的颜色是否获胜(True/False)</returns>
bool check_win_condition(const ChessBoardPreserve* board, bool color);
/// <summary>
/// 用于获得给定的点的颜色
/// </summary>
/// <param name="color">用来存储对应点的颜色的变量</param>
/// <returns>给定位置是否存在点(这里返回False则上面color获取的信息无效)</returns>
bool getChessColorAt(const ChessBoardPreserve* board, int x, int y, bool& color);
/// <summary>
/// 用于判断给定的坐标是否存在点
/// </summary>
bool getChessAt(const ChessBoardPreserve* board, int x, int y);
/// <summary>
/// 用于撤回棋盘上最后一个加入的点
/// </summary>
/// <param name="board"></param>
/// <returns>撤回的点的信息</returns>
Evian::CPoint ChessBoardPreserve_remove_last_point(ChessBoardPreserve* board);