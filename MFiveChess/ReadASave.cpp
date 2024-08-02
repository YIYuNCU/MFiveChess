#include "pch.h"
#include "ReadASave.h"

void Write_Num(std::ofstream& outfile, int num)
{
    outfile << write_tittle("num") << write_content(num);
}

void Write_Color(std::ofstream& outfile, bool Color)
{

    if (Color == TRUE)
    {
        outfile << write_tittle("color") << write_content("black");
    }
    else
    {
        outfile << write_tittle("color") << write_content("white");
    }

}

void Write_Position_x(std::ofstream& outfile, LONG x)
{
    outfile << write_tittle("x") << write_content(x);
}

void Write_Position_y(std::ofstream& outfile, LONG y)
{
    outfile << write_tittle("y") << write_content(y);
}

void Write_Endl(std::ofstream& outfile)
{
    outfile << std::endl;
}
std::string write_tittle(std::string string)
{
    return string + "#";
}
std::string write_tittle(int num)
{
    std::stringstream ss;
    ss << num << "#";
    return ss.str();
}
std::string write_content(std::string string)
{
    return string + ":|";
}
std::string write_content(int num)
{
    std::stringstream ss;
    ss << num << ":|";
    return ss.str();
}
void Write(std::string path, ChessBoardPreserve* board)
{
    int i = 0;
    if (board->points == NULL)
    {
        AfxMessageBox(L"保存错误");
        return;
    }
    while (i < board->num_points)
    {
        std::ofstream outfile(path, std::ios_base::app); // 以追加模式打开文件
        Write_Num(outfile, i);
        Write_Color(outfile, board->points[i].color);
        Write_Position_x(outfile, board->points[i].x);
        Write_Position_y(outfile, board->points[i].y);
        Write_Endl(outfile);
        i++;
    }
}
bool FileExists(const std::string& path)
{
    std::ifstream file(path);
    return file.good();
}


bool stringToBool(const std::string& str)
{
    if (str == "black")
        return true;
    else if (str == "white")
        return false;
    else
        throw std::invalid_argument("Invalid color string: " + str);
}

void ReadFromFile(std::string path, ChessBoardPreserve* boardPreserve)
{
    std::ifstream infile(path);
    if (!infile)
    {
        std::cerr << "无法打开文件: " << path << std::endl;
        return;
    }

    std::string line;
    std::vector<Evian::CPoint> points; // 使用 vector 存储读取的 CPoint 对象
    int temp = -1;

    while (std::getline(infile, line))
    {
        Evian::CPoint accumulatedPoint = Evian::CPoint::ERRPOINT();
        std::string accumulatedData;
        char pre = '0';

        for (char ch : line)
        {
            if (ch == '#')
            {
                // 将累积的数据转换为 CPoint 并存入 vector
                if (!accumulatedData.empty())
                {
                    if (accumulatedData == "num")
                    {
                        temp = 1;
                    }
                    else if (accumulatedData == "color")
                    {
                        temp = 2;
                    }
                    else if (accumulatedData == "x")
                    {
                        temp = 3;
                    }
                    else if (accumulatedData == "y")
                    {
                        temp = 4;
                    }
                    accumulatedData.clear();
                }
            }
            else if (ch == ':')
            {
                pre = ':';
                continue;
            }
            else if (ch == '|' && pre == ':')
            {
                // 将累积的数据转换为 CPoint 并存入 vector
                if (!accumulatedData.empty())
                {
                    if (temp == 1)
                    {
                        /*boardPreserve->num_points++;*/
                    }
                    else if (temp == 2)
                    {
                        if (accumulatedData == "black")
                        {
                            accumulatedPoint.color = true;
                        }
                        else if (accumulatedData == "white")
                        {
                            accumulatedPoint.color = false;
                        }
                    }
                    else if (temp == 3)
                    {
                        accumulatedPoint.x = std::stol(accumulatedData);
                    }
                    else if (temp == 4)
                    {
                        if (accumulatedPoint.x != -1)
                        {
                            accumulatedPoint.y = std::stol(accumulatedData);
                            points.push_back(accumulatedPoint);
                        }
                    }
                    accumulatedData.clear();
                }
            }
            else
            {
                accumulatedData += ch;
            }
        }
    }
    for (Evian::CPoint mpoint : points)
    {
        ChessBoardPreserve_add_point(boardPreserve, mpoint);
    }

    infile.close();
}