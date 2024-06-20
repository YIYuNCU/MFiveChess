#include "pch.h"
#include "ImageShow.h"
#include <atlimage.h>
#include <Windows.h>

BOOL ImageShow::DisplayImage(CSize scaledSize, CPoint position, CDC* pDC, std::string path)
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

    // 创建带有Alpha通道的位图
    BITMAPINFO bmi;
    memset(&bmi, 0, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = scaledSize.cx;
    bmi.bmiHeader.biHeight = -scaledSize.cy; // 负高度表示顶部到底部的颠倒顺序
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32; // 32位色深，包含Alpha通道
    bmi.bmiHeader.biCompression = BI_RGB;

    // 创建DIBSection位图
    void* pvBits;
    HBITMAP hDIBSection = CreateDIBSection(memDC, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0);
    if (hDIBSection == NULL)
    {
        return FALSE; // 创建DIBSection失败
    }

    // 将DIBSection选入内存DC
    HBITMAP hOldBitmap = static_cast<HBITMAP>(SelectObject(memDC, hDIBSection));

    // 在内存DC上绘制缩放后的图片
    image.AlphaBlend(memDC, 0, 0, scaledSize.cx, scaledSize.cy, 0, 0, image.GetWidth(), image.GetHeight());

    // 在目标DC上绘制内存DC中的图像
    AlphaBlend(pDC->GetSafeHdc(), position.x, position.y, scaledSize.cx, scaledSize.cy,
        memDC.GetSafeHdc(), 0, 0, scaledSize.cx, scaledSize.cy, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });

    // 清理资源
    SelectObject(memDC, hOldBitmap);
    DeleteObject(hDIBSection);

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
void ImageShow::SaveBoardAsPNG(std::string& filename)
{
    std::string programPath = GetProgramPath();
    CImage image;
    image.Create(180, 180, 32);
    filename = programPath+ "\\" + filename;
    // 获取CImage的DC指针
    HDC hDC = image.GetDC();

    // 创建一个临时的CDC对象
    CDC dc;
    dc.Attach(hDC);

    // 在CImage的DC上绘制图像
    dc.FillSolidRect(0, 0, 180, 180, RGB(255, 231, 186));
    ImageShow::DrawGrid(&dc);

    // 分离临时CDC对象
    dc.Detach();

    // 释放CImage的DC
    image.ReleaseDC();

    // 保存为PNG格式
    image.Save(CString(filename.c_str()), Gdiplus::ImageFormatPNG);
}

void ImageShow::SaveImageAsPNG(const CImage& image, const std::string& filename)
{
    // 构建完整的文件路径
    std::string programPath = GetProgramPath();
    std::string fullPath = programPath + "\\" + filename;

    // 保存为PNG格式
    image.Save(CString(fullPath.c_str()), Gdiplus::ImageFormatPNG);
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
    temp = ImageShow::GetProgramPath() + "\\" + temp;
}

void ImageShow::ResizeAndSaveImage(const std::string& imagePath, int paneSize)
{
    CImage image;
    if (FAILED(image.Load(CString(imagePath.c_str()))))
    {
        TRACE(_T("Failed to load image\n"));
        return; // 加载失败，可以添加错误处理代码
    }

    int imageWidth = image.GetWidth();
    int imageHeight = image.GetHeight();

    double cellSize = (double)paneSize;
    double newWidth = cellSize;
    double newHeight = cellSize;

    // 创建一个新的 CImage 对象来存储调整大小后的图像
    CImage resizedImage;
    if (!resizedImage.Create(static_cast<int>(10 * newWidth), static_cast<int>(10 * newHeight), image.GetBPP(), image.GetBPP() == 32 ? CImage::createAlphaChannel : 0))
    {
        TRACE(_T("Failed to create resized image!\n"));
        return; // 创建失败，可以添加错误处理代码
    }

    // 使用 StretchBlt 将原始图像绘制到调整大小后的图像中
    CDC dcResized;
    if (dcResized.CreateCompatibleDC(NULL))
    {
        dcResized.SelectObject(resizedImage);

        CDC dcImage;
        if (dcImage.Attach(image.GetDC()))
        {
            dcResized.StretchBlt(0, 0, static_cast<int>(10 * newWidth), static_cast<int>(10 * newHeight),
                &dcImage, 0, 0, imageWidth, imageHeight, SRCCOPY);
            dcImage.Detach();
            image.ReleaseDC();
        }
        else
        {
            TRACE(_T("Failed to attach DC for image!\n"));
            resizedImage.Destroy();
            return;
        }
    }
    else
    {
        TRACE(_T("Failed to create compatible DC!\n"));
        resizedImage.Destroy();
        return;
    }

    // 构建新的文件名，加上 _temp 后缀
    CString strImagePath(imagePath.c_str());
    int dotIndex = strImagePath.ReverseFind('.');
    CString strNewImagePath = strImagePath.Left(dotIndex) + _T("_temp") + strImagePath.Mid(dotIndex);

    // 保存调整大小后的图像到新路径
    if (FAILED(resizedImage.Save(strNewImagePath)))
    {
        TRACE(_T("Failed to save resized image!\n"));
        return; // 保存失败，可以添加错误处理代码
    }

    // 提示保存成功
    TRACE(_T("Image resized and saved successfully: %s\n"), strNewImagePath);
}

BOOL ImageShow::SaveProgressAsPNG(std::string pathb, std::string pathf, CSize scaledSize, CPoint position)
{
    // 1. 从pathb读取背景图片
    CImage imgBackground;
    if (FAILED(imgBackground.Load(CString(pathb.c_str()))))
    {
        // 处理读取背景图片失败的情况
        return FALSE;
    }

    // 2. 从pathf读取前景图片
    CImage imgForeground;
    if (FAILED(imgForeground.Load(CString(pathf.c_str()))))
    {
        // 处理读取前景图片失败的情况
        return FALSE;
    }

    // 3. 计算前景图片的尺寸，按照题目要求是背景图片尺寸的(1/(18*1.2))倍
    int foregroundWidth = static_cast<int>(imgBackground.GetWidth() / (18 * 1.2));
    int foregroundHeight = static_cast<int>(imgBackground.GetHeight() / (18 * 1.2));

    // 4. 计算前景图片在背景图片上的位置
    int foregroundLeft = position.x - foregroundWidth / 2;
    int foregroundTop = position.y - foregroundHeight / 2;

    // 5. 创建临时DC进行绘制
    CDC dc;
    dc.CreateCompatibleDC(nullptr);

    // 6. 将背景图片绘制到临时DC上
    CBitmap bmp;
    bmp.CreateCompatibleBitmap(&dc, imgBackground.GetWidth(), imgBackground.GetHeight());
    dc.SelectObject(&bmp);
    imgBackground.AlphaBlend(dc.GetSafeHdc(), 0, 0, scaledSize.cx, scaledSize.cy, 0, 0, imgBackground.GetWidth(), imgBackground.GetHeight());

    // 7. 将前景图片绘制到背景图片上
    imgForeground.AlphaBlend(dc.GetSafeHdc(), foregroundLeft, foregroundTop, foregroundWidth, foregroundHeight, 0, 0, imgForeground.GetWidth(), imgForeground.GetHeight());

    // 8. 保存绘制后的结果为PNG文件，保留透明通道
    CString outputPath(pathb.c_str());
    outputPath.Replace(_T(".png"), _T("_progress.png"));  // 替换为progress.png

    HRESULT hr = imgBackground.Save(outputPath, Gdiplus::ImageFormatPNG);
    if (FAILED(hr))
    {
        // 处理保存文件失败的情况
        return FALSE;
    }

    // 9. 清理临时资源
    dc.DeleteDC();
    bmp.DeleteObject();

    return TRUE;
}



