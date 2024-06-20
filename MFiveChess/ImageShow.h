#pragma once
#include <string>

class ImageShow
{
public:
    static BOOL DisplayImage(CSize scaledSize, CPoint position, CDC* pDC, std::string path);
    static void DrawGrid(CDC* pDC);
    static void SaveBoardAsPNG(std::string& filename);
    static std::string GetProgramPath();
    static void ResizeAndSaveImage(const std::string& imagePath, int paneSize);
    static void GetPath(std::string& temp);
    static CString OnCaptureImage(CDC* pDC, CSize size, CPoint position, std::string savepath, int nownum);
};
