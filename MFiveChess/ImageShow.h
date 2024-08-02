#pragma once
#include <string>

class ImageShow
{
public:
    /// <summary>
    /// 显示图像
    /// </summary>
    /// <param name="scaledSize">显示的大小</param>
    /// <param name="position">显示的位置</param>
    /// <param name="pDC">显示图像用的句柄</param>
    /// <param name="path">图像路径</param>
    /// <returns>是否成功显示</returns>
    static BOOL DisplayImage(CSize scaledSize, CPoint position, CDC* pDC, std::string path);
    /// <summary>
    /// 绘制棋盘
    /// </summary>
    /// <param name="pDC">句柄</param>
    static void DrawGrid(CDC* pDC);
    /// <summary>
    /// 用于将绘制出的棋盘保存为png图片
    /// </summary>
    /// <param name="filename">保存路径</param>
    static void SaveBoardAsPNG(std::string& filename);
    /// <summary>
    /// 获取程序所在的路径
    /// </summary>
    /// <returns>程序路径</returns>
    static std::string GetProgramPath();
    /// <summary>
    /// 基于程序路径将给定的相对路径转换为绝对路径
    /// </summary>
    /// <param name="temp">绝对路径</param>
    static void GetPath(std::string& temp);
    /// <summary>
    /// 将棋盘情况截取，并保存为PNG图片
    /// </summary>
    /// <param name="pDC">句柄</param>
    /// <param name="size">截取区域大小</param>
    /// <param name="position">开始截取位置</param>
    /// <param name="savepath">保存位置</param>
    /// <param name="nownum">当前保存的数量（用于构建文件名）</param>
    /// <returns>保存文件名</returns>
    static CString OnCaptureImage(CDC* pDC, CSize size, CPoint position, std::string savepath, int nownum);
};
