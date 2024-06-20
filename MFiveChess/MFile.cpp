#include "pch.h"
#include "MFile.h"

bool FolderExists(const CString& folderPath)
{
    DWORD dwAttrib = GetFileAttributes(folderPath);

    return (dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
}

bool CreateFolder(const CString& folderPath)
{
    if (!FolderExists(folderPath))
    {
        if (CreateDirectory(folderPath, NULL))
        {
            return true; // 创建成功
        }
        else
        {
            return false; // 创建失败
        }
    }

    return true; // 文件夹已存在
}

void DeleteAllPNGImages(const CString& folderPath)
{
    // 构造搜索路径
    CString searchPath = folderPath + _T("\\*.png");

    // 查找第一个匹配文件
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(searchPath, &findFileData);

    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            // 构造完整文件路径
            CString filePath = folderPath + _T("\\") + findFileData.cFileName;

            // 删除文件
            if (!DeleteFile(filePath))
            {
                // 处理删除失败的情况
                DWORD dwError = GetLastError();
                // 可以输出错误信息或者进行其他处理
                // 这里简单输出错误码示例
                TRACE(_T("Failed to delete file %s, Error code: %d\n"), (LPCTSTR)filePath, dwError);
            }

        } while (FindNextFile(hFind, &findFileData)); // 继续查找下一个匹配文件

        // 关闭搜索句柄
        FindClose(hFind);
    }
}