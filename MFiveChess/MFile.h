#pragma once
#include "pch.h"
#include <afxwin.h>
#include <afxdlgs.h>
#include <tlhelp32.h>
bool FolderExists(const CString& folderPath);
bool CreateFolder(const CString& folderPath);
void DeleteAllPNGImages(const CString& folderPath);
void RunExternalProcess(CString& filePath);
void TerminateProcess();
void CloseProcessHandles();
bool IsProcessRunning();