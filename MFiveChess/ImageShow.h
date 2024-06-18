#pragma once
#include <string>

class ImageShow
{
public:
    static BOOL DisplayImage(CSize scaledSize, CPoint position, CRect rect, CDC* pDC, std::string path);
    static void DrawGrid(CDC* pDC);
    static void SaveImageAsPNG(std::string& filename);
    static std::string GetProgramPath();
    static CImage ReadAndResizeImage(const std::string& imagePath, int paneSize);
    static void GetPath(std::string& temp);
    static void ResizeImage(CImage& image, int newWidth, int newHeight);
};
