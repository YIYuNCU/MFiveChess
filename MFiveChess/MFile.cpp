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

LPWSTR ConvertLPCWSTRToLPWSTR(LPCWSTR lpwszStr)
{
    // 计算字符串长度（不包括终止空字符）
    int len = wcslen(lpwszStr);

    // 分配内存空间，包括空字符
    LPWSTR lpwszResult = new wchar_t[len + 1];

    // 复制字符串内容到新分配的内存中
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

    // 使用 const wchar_t* 类型的指针来传递文件路径
    LPCWSTR lpFilePath = filePath.GetString(); 

    // 创建进程
    if (!CreateProcess(NULL,               // 模块名称（使用命令行）
        ConvertLPCWSTRToLPWSTR(lpFilePath),                        // 命令行
        NULL,                              // 进程句柄不可继承
        NULL,                              // 线程句柄不可继承
        FALSE,                             // 设置句柄不可继承
        0,                                 // 无创建标志
        NULL,                              // 使用父进程的环境块
        NULL,                              // 使用父进程的启动目录
        &si,                               // 指向 STARTUPINFO 结构的指针
        &g_ProcessInfo))                   // 指向 PROCESS_INFORMATION 结构的指针
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
        // 如果进程句柄无效，则进程未成功开启
        return false;
    }

    // 检查进程是否仍然存活
    DWORD exitCode = 0;
    if (!GetExitCodeProcess(g_ProcessInfo.hProcess, &exitCode))
    {
        // 如果无法获取进程退出码，则认为进程未成功开启
        return false;
    }

    // 如果进程还在运行中，则返回true
    return exitCode == STILL_ACTIVE;
}