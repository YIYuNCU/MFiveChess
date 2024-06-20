#include "pch.h"
#include "ImageShow.h"
#include <atlimage.h>
#include <Windows.h>

BOOL ImageShow::DisplayImage(CSize scaledSize, CPoint position, CDC* pDC, std::string path)
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

    // ��������Alphaͨ����λͼ
    BITMAPINFO bmi;
    memset(&bmi, 0, sizeof(BITMAPINFO));
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = scaledSize.cx;
    bmi.bmiHeader.biHeight = -scaledSize.cy; // ���߶ȱ�ʾ�������ײ��ĵߵ�˳��
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32; // 32λɫ�����Alphaͨ��
    bmi.bmiHeader.biCompression = BI_RGB;

    // ����DIBSectionλͼ
    void* pvBits;
    HBITMAP hDIBSection = CreateDIBSection(memDC, &bmi, DIB_RGB_COLORS, &pvBits, NULL, 0);
    if (hDIBSection == NULL)
    {
        return FALSE; // ����DIBSectionʧ��
    }

    // ��DIBSectionѡ���ڴ�DC
    HBITMAP hOldBitmap = static_cast<HBITMAP>(SelectObject(memDC, hDIBSection));

    // ���ڴ�DC�ϻ������ź��ͼƬ
    image.AlphaBlend(memDC, 0, 0, scaledSize.cx, scaledSize.cy, 0, 0, image.GetWidth(), image.GetHeight());

    // ��Ŀ��DC�ϻ����ڴ�DC�е�ͼ��
    AlphaBlend(pDC->GetSafeHdc(), position.x, position.y, scaledSize.cx, scaledSize.cy,
        memDC.GetSafeHdc(), 0, 0, scaledSize.cx, scaledSize.cy, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });

    // ������Դ
    SelectObject(memDC, hOldBitmap);
    DeleteObject(hDIBSection);

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
void ImageShow::SaveBoardAsPNG(std::string& filename)
{
    std::string programPath = GetProgramPath();
    CImage image;
    image.Create(180, 180, 32);
    filename = programPath+ "\\" + filename;
    // ��ȡCImage��DCָ��
    HDC hDC = image.GetDC();

    // ����һ����ʱ��CDC����
    CDC dc;
    dc.Attach(hDC);

    // ��CImage��DC�ϻ���ͼ��
    dc.FillSolidRect(0, 0, 180, 180, RGB(255, 231, 186));
    ImageShow::DrawGrid(&dc);

    // ������ʱCDC����
    dc.Detach();

    // �ͷ�CImage��DC
    image.ReleaseDC();

    // ����ΪPNG��ʽ
    image.Save(CString(filename.c_str()), Gdiplus::ImageFormatPNG);
}

void ImageShow::SaveImageAsPNG(const CImage& image, const std::string& filename)
{
    // �����������ļ�·��
    std::string programPath = GetProgramPath();
    std::string fullPath = programPath + "\\" + filename;

    // ����ΪPNG��ʽ
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
        return; // ����ʧ�ܣ�������Ӵ��������
    }

    int imageWidth = image.GetWidth();
    int imageHeight = image.GetHeight();

    double cellSize = (double)paneSize;
    double newWidth = cellSize;
    double newHeight = cellSize;

    // ����һ���µ� CImage �������洢������С���ͼ��
    CImage resizedImage;
    if (!resizedImage.Create(static_cast<int>(10 * newWidth), static_cast<int>(10 * newHeight), image.GetBPP(), image.GetBPP() == 32 ? CImage::createAlphaChannel : 0))
    {
        TRACE(_T("Failed to create resized image!\n"));
        return; // ����ʧ�ܣ�������Ӵ��������
    }

    // ʹ�� StretchBlt ��ԭʼͼ����Ƶ�������С���ͼ����
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

    // �����µ��ļ��������� _temp ��׺
    CString strImagePath(imagePath.c_str());
    int dotIndex = strImagePath.ReverseFind('.');
    CString strNewImagePath = strImagePath.Left(dotIndex) + _T("_temp") + strImagePath.Mid(dotIndex);

    // ���������С���ͼ����·��
    if (FAILED(resizedImage.Save(strNewImagePath)))
    {
        TRACE(_T("Failed to save resized image!\n"));
        return; // ����ʧ�ܣ�������Ӵ��������
    }

    // ��ʾ����ɹ�
    TRACE(_T("Image resized and saved successfully: %s\n"), strNewImagePath);
}

BOOL ImageShow::SaveProgressAsPNG(std::string pathb, std::string pathf, CSize scaledSize, CPoint position)
{
    // 1. ��pathb��ȡ����ͼƬ
    CImage imgBackground;
    if (FAILED(imgBackground.Load(CString(pathb.c_str()))))
    {
        // �����ȡ����ͼƬʧ�ܵ����
        return FALSE;
    }

    // 2. ��pathf��ȡǰ��ͼƬ
    CImage imgForeground;
    if (FAILED(imgForeground.Load(CString(pathf.c_str()))))
    {
        // �����ȡǰ��ͼƬʧ�ܵ����
        return FALSE;
    }

    // 3. ����ǰ��ͼƬ�ĳߴ磬������ĿҪ���Ǳ���ͼƬ�ߴ��(1/(18*1.2))��
    int foregroundWidth = static_cast<int>(imgBackground.GetWidth() / (18 * 1.2));
    int foregroundHeight = static_cast<int>(imgBackground.GetHeight() / (18 * 1.2));

    // 4. ����ǰ��ͼƬ�ڱ���ͼƬ�ϵ�λ��
    int foregroundLeft = position.x - foregroundWidth / 2;
    int foregroundTop = position.y - foregroundHeight / 2;

    // 5. ������ʱDC���л���
    CDC dc;
    dc.CreateCompatibleDC(nullptr);

    // 6. ������ͼƬ���Ƶ���ʱDC��
    CBitmap bmp;
    bmp.CreateCompatibleBitmap(&dc, imgBackground.GetWidth(), imgBackground.GetHeight());
    dc.SelectObject(&bmp);
    imgBackground.AlphaBlend(dc.GetSafeHdc(), 0, 0, scaledSize.cx, scaledSize.cy, 0, 0, imgBackground.GetWidth(), imgBackground.GetHeight());

    // 7. ��ǰ��ͼƬ���Ƶ�����ͼƬ��
    imgForeground.AlphaBlend(dc.GetSafeHdc(), foregroundLeft, foregroundTop, foregroundWidth, foregroundHeight, 0, 0, imgForeground.GetWidth(), imgForeground.GetHeight());

    // 8. ������ƺ�Ľ��ΪPNG�ļ�������͸��ͨ��
    CString outputPath(pathb.c_str());
    outputPath.Replace(_T(".png"), _T("_progress.png"));  // �滻Ϊprogress.png

    HRESULT hr = imgBackground.Save(outputPath, Gdiplus::ImageFormatPNG);
    if (FAILED(hr))
    {
        // �������ļ�ʧ�ܵ����
        return FALSE;
    }

    // 9. ������ʱ��Դ
    dc.DeleteDC();
    bmp.DeleteObject();

    return TRUE;
}



