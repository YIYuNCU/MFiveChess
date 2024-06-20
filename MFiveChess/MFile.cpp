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
            return true; // �����ɹ�
        }
        else
        {
            return false; // ����ʧ��
        }
    }

    return true; // �ļ����Ѵ���
}

void DeleteAllPNGImages(const CString& folderPath)
{
    // ��������·��
    CString searchPath = folderPath + _T("\\*.png");

    // ���ҵ�һ��ƥ���ļ�
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(searchPath, &findFileData);

    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            // ���������ļ�·��
            CString filePath = folderPath + _T("\\") + findFileData.cFileName;

            // ɾ���ļ�
            if (!DeleteFile(filePath))
            {
                // ����ɾ��ʧ�ܵ����
                DWORD dwError = GetLastError();
                // �������������Ϣ���߽�����������
                // ��������������ʾ��
                TRACE(_T("Failed to delete file %s, Error code: %d\n"), (LPCTSTR)filePath, dwError);
            }

        } while (FindNextFile(hFind, &findFileData)); // ����������һ��ƥ���ļ�

        // �ر��������
        FindClose(hFind);
    }
}