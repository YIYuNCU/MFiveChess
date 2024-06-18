#include "pch.h"
#include "ImageShow.h"
#include <atlimage.h>
#include <Windows.h>

BOOL ImageShow::DisplayImage(CSize scaledSize, CPoint position, CRect rect, CDC* pDC,std::string path)
{
    // ���ļ���ȡͼƬ
    CImage image;
    if (FAILED(image.Load(CString(path.c_str()))))
    {
        return FALSE; // ��ȡͼƬʧ��
    }

    // ���������ź�ߴ���ͬ���ڴ�DC
    CDC memDC;
    memDC.CreateCompatibleDC(pDC); // ������Ŀ��DC���ݵ��ڴ�DC
    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap(pDC, scaledSize.cx, scaledSize.cy); // ������Ŀ��DC���ݵ�λͼ
    CBitmap* pOldBitmap = memDC.SelectObject(&bitmap); // ��λͼѡ���ڴ�DC

    // ���ڴ�DC�ϻ������ź��ͼƬ
    image.StretchBlt(memDC.m_hDC, 0, 0, scaledSize.cx, scaledSize.cy , // Ŀ����ε����ϽǺͳߴ�
        0, 0, image.GetWidth(), image.GetHeight(), // Դ���ε����ϽǺͳߴ�
        SRCCOPY); // ��դ��������,����ʹ��SRCCOPY��Դ���Ƶ�Ŀ��

    // ��Ŀ��DC�ϻ������ź��ͼƬ
    pDC->BitBlt(position.x, position.y, scaledSize.cx - position.x, scaledSize.cy - position.y, // Ŀ����ε����ϽǺͳߴ�
        &memDC, 0, 0, SRCCOPY); // ԴDC,Դ���ε����Ͻ�,��դ��������

    // ������Դ
    memDC.SelectObject(pOldBitmap); // ��ԭ����λͼѡ���ڴ�DC
    bitmap.DeleteObject(); // ɾ��λͼ����

    return TRUE; // ͼƬ��ʾ�ɹ�
}


void ImageShow::DrawGrid(CDC* pDC)
{
    // ���ñ���ɫΪ��ɫ
    pDC->SetBkColor(RGB(190, 190, 190));

    // ���û�����ɫΪ��ɫ
    CPen pen(PS_SOLID, 1, RGB(140, 140, 140));
    CPen* pOldPen = pDC->SelectObject(&pen);

    // ���û�ˢ��ɫΪ��ɫ
    CBrush brush(RGB(140, 140, 140));
    CBrush* pOldBrush = pDC->SelectObject(&brush);

    // ����ÿ�����ӵĴ�С
    double cellSize = 10;

    // ����������
    for (int i = 0; i <= 18; i++)
    {
        // ���ƴ�ֱ��
        pDC->MoveTo((i * cellSize - 1) > 0 ? (i * cellSize-1) : 0, 0);
        pDC->LineTo((i * cellSize - 1) > 0 ? (i * cellSize-1) : 0, 180);

        // ����ˮƽ��
        pDC->MoveTo(0, (i * cellSize - 1) > 0 ? (i * cellSize-1) : 0);
        pDC->LineTo(180, (i * cellSize - 1) > 0 ? (i * cellSize-1) : 0);
    }

    // ����ԲȦ
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

    // �ָ�ԭ���Ļ�ˢ
    pDC->SelectObject(pOldBrush);

    // �ָ�ԭ���Ļ���
    pDC->SelectObject(pOldPen);
}
void ImageShow::SaveImageAsPNG(std::string& filename)
{
    std::string programPath = GetProgramPath();
    CImage image;
    image.Create(183, 183, 32);
    filename = programPath+ "\\/" + filename;
    // ��ȡCImage��DCָ��
    HDC hDC = image.GetDC();

    // ����һ����ʱ��CDC����
    CDC dc;
    dc.Attach(hDC);

    // ��CImage��DC�ϻ���ͼ��
    dc.FillSolidRect(0, 0, 183, 183, RGB(255, 231, 186));
    ImageShow::DrawGrid(&dc);

    // ������ʱCDC����
    dc.Detach();

    // �ͷ�CImage��DC
    image.ReleaseDC();

    // ����ΪPNG��ʽ
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
