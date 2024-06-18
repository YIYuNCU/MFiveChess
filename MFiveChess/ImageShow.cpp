#include "pch.h"
#include "ImageShow.h"
#include <atlimage.h>
#include <Windows.h>

BOOL ImageShow::DisplayImage(CSize scaledSize, CPoint position, CRect rect, CDC* pDC,std::string path)
{
    // 从文件读取图片
    CImage image;
    if (FAILED(image.Load(CString(path.c_str()))))
    {
        return FALSE; // 读取图片失败
    }

    // 创建与缩放后尺寸相同的内存DC
    CDC memDC;
    memDC.CreateCompatibleDC(pDC); // 创建与目标DC兼容的内存DC
    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap(pDC, scaledSize.cx, scaledSize.cy); // 创建与目标DC兼容的位图
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap); // 将位图选入内存DC

    // 在内存DC上绘制缩放后的图片
    image.StretchBlt(memDC.m_hDC, 0, 0, scaledSize.cx, scaledSize.cy , // 目标矩形的左上角和尺寸
        0, 0, image.GetWidth(), image.GetHeight(), // 源矩形的左上角和尺寸
        SRCCOPY); // 光栅操作代码,这里使用SRCCOPY将源复制到目标

    // 在目标DC上绘制缩放后的图片
    pDC->BitBlt(position.x, position.y, scaledSize.cx - position.x, scaledSize.cy - position.y, // 目标矩形的左上角和尺寸
        &memDC, 0, 0, SRCCOPY); // 源DC,源矩形的左上角,光栅操作代码

    // 清理资源
    memDC.SelectObject(pOldBitmap); // 将原来的位图选回内存DC
    bitmap.DeleteObject(); // 删除位图对象

    return TRUE; // 图片显示成功
}


void ImageShow::DrawGrid(CDC* pDC)
{
    // 设置背景色为黑色
    pDC->SetBkColor(RGB(190, 190, 190));

    // 设置画笔颜色为白色
    CPen pen(PS_SOLID, 1, RGB(140, 140, 140));
    CPen* pOldPen = pDC->SelectObject(&pen);

    // 设置画刷颜色为白色
    CBrush brush(RGB(140, 140, 140));
    CBrush* pOldBrush = pDC->SelectObject(&brush);

    // 计算每个格子的大小
    double cellSize = 10;

    // 绘制网格线
    for (int i = 0; i <= 18; i++)
    {
        // 绘制垂直线
        pDC->MoveTo((i * cellSize - 1) > 0 ? (i * cellSize-1) : 0, 0);
        pDC->LineTo((i * cellSize - 1) > 0 ? (i * cellSize-1) : 0, 180);

        // 绘制水平线
        pDC->MoveTo(0, (i * cellSize - 1) > 0 ? (i * cellSize-1) : 0);
        pDC->LineTo(180, (i * cellSize - 1) > 0 ? (i * cellSize-1) : 0);
    }

    // 绘制圆圈
    int circleRadius = cellSize / 4;
    int circleX[] = { 3, 9, 15 };
    int circleY[] = { 3, 9, 15 };

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            int x = circleX[i] * cellSize - circleRadius;
            int y = circleY[j] * cellSize - circleRadius;
            pDC->Ellipse(x, y, x + 2 * circleRadius, y + 2 * circleRadius);
        }
    }

    // 恢复原来的画刷
    pDC->SelectObject(pOldBrush);

    // 恢复原来的画笔
    pDC->SelectObject(pOldPen);
}
void ImageShow::SaveImageAsPNG(std::string& filename)
{
    std::string programPath = GetProgramPath();
    CImage image;
    image.Create(183, 183, 32);
    filename = programPath+ "\\/" + filename;
    // 获取CImage的DC指针
    HDC hDC = image.GetDC();

    // 创建一个临时的CDC对象
    CDC dc;
    dc.Attach(hDC);

    // 在CImage的DC上绘制图像
    dc.FillSolidRect(0, 0, 183, 183, RGB(255, 231, 186));
    ImageShow::DrawGrid(&dc);

    // 分离临时CDC对象
    dc.Detach();

    // 释放CImage的DC
    image.ReleaseDC();

    // 保存为PNG格式
    image.Save(CString(filename.c_str()), Gdiplus::ImageFormatPNG);
}

std::string ImageShow::GetProgramPath()
{
    char buffer[MAX_PATH] = { 0 };
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
}
void ImageShow::GetPath(std::string& temp)
{
    temp = ImageShow::GetProgramPath() + "\\/" + temp;
}

CImage ImageShow::ReadAndResizeImage(const std::string& imagePath,int paneSize)
{
    CImage image;
    image.Load(CA2CT(imagePath.c_str()));

    int imageWidth = image.GetWidth();
    int imageHeight = image.GetHeight();

    int cellSize = min(imageWidth, imageHeight) / paneSize;
    double newWidth = cellSize * paneSize * 0.3;
    double newHeight = cellSize * paneSize *0.3;

    CImage resizedImage;
    resizedImage.Create(10 * newWidth, 10 * newHeight, image.GetBPP());

    image.StretchBlt(resizedImage.GetDC(), 0, 0, newWidth, newHeight, 0, 0, imageWidth, imageHeight, SRCCOPY);
    resizedImage.ReleaseDC();

    return resizedImage;
}

void ImageShow::ResizeImage(CImage& image, int newWidth, int newHeight) 
{
    if (image.IsNull() || (image.GetWidth() == newWidth && image.GetHeight() == newHeight)) 
    {
        return;
    }

    CImage resizedImage;
    resizedImage.Create(newWidth, newHeight, image.GetBPP());

    CDC memDC;
    memDC.CreateCompatibleDC(NULL);

    CBitmap bitmap;
    bitmap.Attach(resizedImage.Detach());
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

    memDC.SetStretchBltMode(HALFTONE);
    image.StretchBlt(memDC.m_hDC, 0, 0, newWidth, newHeight, 0, 0, image.GetWidth(), image.GetHeight(), SRCCOPY);

    memDC.SelectObject(pOldBitmap);

    image.Destroy();
    image.Attach(resizedImage.Detach());
    resizedImage.Destroy();
}
