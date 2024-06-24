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

LPWSTR ConvertLPCWSTRToLPWSTR(LPCWSTR lpwszStr)
{
    // �����ַ������ȣ���������ֹ���ַ���
    int len = wcslen(lpwszStr);

    // �����ڴ�ռ䣬�������ַ�
    LPWSTR lpwszResult = new wchar_t[len + 1];

    // �����ַ������ݵ��·�����ڴ���
    wcscpy_s(lpwszResult, len + 1, lpwszStr);

    return lpwszResult;
}

PROCESS_INFORMATION g_ProcessInfo;

void RunExternalProcess(CString& filePath)
{
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&g_ProcessInfo, sizeof(g_ProcessInfo));

    // ʹ�� const wchar_t* ���͵�ָ���������ļ�·��
    LPCWSTR lpFilePath = filePath.GetString(); 

    // ��������
    if (!CreateProcess(NULL,               // ģ�����ƣ�ʹ�������У�
        ConvertLPCWSTRToLPWSTR(lpFilePath),                        // ������
        NULL,                              // ���̾�����ɼ̳�
        NULL,                              // �߳̾�����ɼ̳�
        FALSE,                             // ���þ�����ɼ̳�
        0,                                 // �޴�����־
        NULL,                              // ʹ�ø����̵Ļ�����
        NULL,                              // ʹ�ø����̵�����Ŀ¼
        &si,                               // ָ�� STARTUPINFO �ṹ��ָ��
        &g_ProcessInfo))                   // ָ�� PROCESS_INFORMATION �ṹ��ָ��
    {
        std::cerr << "CreateProcess failed: " << GetLastError() << std::endl;
        return;
    }
}


void TerminateProcess()
{
    if (g_ProcessInfo.hProcess != NULL)
    {
        // Terminate the process
        if (!TerminateProcess(g_ProcessInfo.hProcess, 0))
        {
            std::cerr << "TerminateProcess failed: " << GetLastError() << std::endl;
            return;
        }

        // Close process and thread handles
        CloseHandle(g_ProcessInfo.hProcess);
        CloseHandle(g_ProcessInfo.hThread);
        ZeroMemory(&g_ProcessInfo, sizeof(g_ProcessInfo));
    }
}

void CloseProcessHandles()
{
    if (g_ProcessInfo.hProcess != NULL)
    {
        // Close process and thread handles
        CloseHandle(g_ProcessInfo.hProcess);
        CloseHandle(g_ProcessInfo.hThread);
        ZeroMemory(&g_ProcessInfo, sizeof(g_ProcessInfo));
    }
}

bool IsProcessRunning()
{
    if (g_ProcessInfo.hProcess == NULL || g_ProcessInfo.hProcess == INVALID_HANDLE_VALUE)
    {
        // ������̾����Ч�������δ�ɹ�����
        return false;
    }

    // �������Ƿ���Ȼ���
    DWORD exitCode = 0;
    if (!GetExitCodeProcess(g_ProcessInfo.hProcess, &exitCode))
    {
        // ����޷���ȡ�����˳��룬����Ϊ����δ�ɹ�����
        return false;
    }

    // ������̻��������У��򷵻�true
    return exitCode == STILL_ACTIVE;
}