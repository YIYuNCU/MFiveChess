#include "pch.h"
#include "ChessBoradPreserve.h" // ��������ͷ�ļ��а����� ChessBoardPreserve �������


using namespace Evian;
// ��ʼ�� ChessBoardPreserve
void ChessBoardPreserve_init(ChessBoardPreserve* board)
{
    board->points = NULL;
    board->num_points = 0;
    board->capacity = 0;
}
// ���� ChessBoardPreserve
void ChessBoardPreserve_destroy(ChessBoardPreserve* board) {
    free(board->points); // �ͷŶ�̬������ڴ�
    board->points = NULL;
    board->num_points = 0;
    board->capacity = 0;
}

// �������������ĳ���������Ƿ����ͬɫ��������
bool check_line(const ChessBoardPreserve* board, int x, int y, int dx, int dy, bool color)
{
    int count = 1; // ��ǰ�����Ѿ�����Ϊ1����������
    int nx, ny;
    // ������
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

    // ������
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

    return count >= 5; // �����Ƿ������������
}

// �����������ĳ������ʤ������
bool check_win_condition(const ChessBoardPreserve* board, bool color)
{
    int directions[4][2] = { {0, 1}, {1, 0}, {1, 1}, {-1, 1} };

    // ����ÿ������λ��
    for (int i = 0; i < board->num_points; ++i)
    {
        int x = board->points[i].x;
        int y = board->points[i].y;

        // ��鵱ǰ������ɫ�Ƿ�Ϊ��������ɫ
        if (board->points[i].color != color)
        {
            continue; // �����ɫ��ƥ�䣬������һ������λ�õļ��
        }

        // ���ĸ������ϼ���Ƿ�����������
        for (int d = 0; d < 4; ++d)
        {
            if (check_line(board, x, y, directions[d][0], directions[d][1], color))
            {
                return true; // �����������飬���� true
            }
        }
    }

    return false; // δ�ҵ��������飬���� false
}

Evian::CPoint ChessBoardPreserve_add_point(ChessBoardPreserve* board, Evian::CPoint point, bool color)
{
    // ����µĵ��Ƿ��Ѿ�����
    for (int i = 0; i < board->num_points; ++i)
    {
        if (board->points[i].x == point.x && board->points[i].y == point.y)
        {
            return Evian::CPoint::ERRPOINT(); // ������Ѿ����ڣ��򷵻ش����
        }
    }

    // ��չ����
    if (board->num_points >= board->capacity)
    {
        int new_capacity = (board->capacity == 0) ? 1 : board->capacity * 2;
        Evian::CPoint* new_points = (Evian::CPoint*)realloc(board->points, new_capacity * sizeof(Evian::CPoint));
        if (new_points == NULL)
        {
            return Evian::CPoint::ERRPOINT(); // �����ڴ����ʧ�����
        }
        board->points = new_points;
        board->capacity = new_capacity;
    }

    // ���µĵ���ӵ�������
    board->points[board->num_points] = point;
    board->points[board->num_points].color = color;
    board->num_points++;
    return point; // �ɹ����
}

bool getChessColorAt(const ChessBoardPreserve* board, int x, int y, bool& color)
{
    for (int j = 0; j < board->num_points; ++j)
    {
        if (board->points[j].x == x && board->points[j].y == y)
        {
            color = board->points[j].color;
            return true; // �ҵ���Ӧλ�õ����ӣ���������ɫ
        }
    }
    return false; // �Ҳ�����Ӧλ�õ����ӣ����� false
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
