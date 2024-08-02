#pragma once
#include "pch.h"
#include <tlhelp32.h>
/// <summary>
/// 判断文件夹是否存在
/// </summary>
/// <param name="folderPath">文件夹路径</param>
/// <returns>是否存在</returns>
bool FolderExists(const CString& folderPath);
/// <summary>
/// 创建文件夹
/// </summary>
/// <param name="folderPath">文件夹路径</param>
/// <returns>是否创建成功</returns>
bool CreateFolder(const CString& folderPath);
/// <summary>
/// 删除制定文件夹内所有png图片
/// </summary>
/// <param name="folderPath">文件夹路径</param>
void DeleteAllPNGImages(const CString& folderPath);
/// <summary>
/// 运行外部程序
/// </summary>
/// <param name="filePath">程序路径</param>
void RunExternalProcess(CString& filePath);
/// <summary>
/// 尝试终止打开的进程
/// </summary>
void TerminateProcess();
/// <summary>
/// 创建进程话柄
/// </summary>
void CloseProcessHandles();
/// <summary>
/// 判断进程是否在运行
/// </summary>
/// <returns>程序是否在运行</returns>
bool IsProcessRunning();
/// <summary>
/// 判断文件是否存在
/// </summary>
/// <param name="filename">绝对路径+文件名</param>
/// <returns>文件是否存在</returns>
bool fileExists(const std::string& filename);
/// <summary>
/// 获取下一个未被占用的文件名（+序号）
/// </summary>
/// <param name="baseName">基础文件名</param>
/// <returns>可用的文件名</returns>
std::string generateNextFileName(const std::string& baseName);
/// <summary>
/// 获取序号最靠后的文件的名字
/// </summary>
/// <param name="baseName">文件名（包括拓展名但不包括序号）</param>
/// <returns>序号最靠后的文件的名字</returns>
std::string generateLastFileName(const std::string& baseName);
/// <summary>
/// 创建文件
/// </summary>
/// <param name="baseName">基础名+拓展名</param>
/// <returns>生成的基于基础名，带序号的名字</returns>
std::string createFile(const std::string& baseName);
