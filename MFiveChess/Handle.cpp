#include "pch.h"
#include "Handle.h"

SharedMemoryListener::SharedMemoryListener()
    : hMapFile(nullptr), mappedPoint(nullptr), stopListening(false) {}

SharedMemoryListener::~SharedMemoryListener() 
{
    stopListeningThread(); // 确保停止监听线程
    if (mappedPoint) 
    {
        UnmapViewOfFile(mappedPoint);
        mappedPoint = nullptr;
    }
    if (hMapFile) 
    {
        CloseHandle(hMapFile);
        hMapFile = nullptr;
    }
}

bool SharedMemoryListener::initialize() 
{
    hMapFile = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, L"Local\\MFiveChessMemory");
    if (hMapFile == nullptr) 
    {
        AfxMessageBox(L"打开共享内存失败!");
        return false;
    }

    mappedPoint = (Point*)MapViewOfFile(hMapFile, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, sizeof(Point));
    if (mappedPoint == nullptr) 
    {
        AfxMessageBox(L"读取共享内存失败!");
        CloseHandle(hMapFile);
        hMapFile = nullptr;
        return false;
    }

    return true;
}

void SharedMemoryListener::listenSharedMemoryChanges() 
{
    Point currentPoint;
    while (!stopListening) 
    {
        if (receivePoint(currentPoint)) 
        {
            CString message;
            if (currentPoint.x < 0 || currentPoint.y < 0)
            {
                Sleep(100);
                continue;
            }
            else
            {
                invalidatePoint();
            }
            message.Format(L"Received Point: (%d, %d)", currentPoint.x, currentPoint.y);
            AfxMessageBox(message);
        }
        Sleep(100);  // 每0.1秒监听一次
    }
}

bool SharedMemoryListener::startListening()
{
    if (!initialize()) 
    {
        return false;
    }
    listenerThread = std::thread(&SharedMemoryListener::listenSharedMemoryChanges, this);
    return true;
}

void SharedMemoryListener::stopListeningThread() 
{
    stopListening = true;
    if (listenerThread.joinable()) 
    {
        listenerThread.join();
    }
}

bool SharedMemoryListener::receivePoint(Point& point) 
{
    std::lock_guard<std::mutex> lock(mtx);
    if (mappedPoint) 
    {
        point.x = mappedPoint->x;
        point.y = mappedPoint->y;
        return true;
    }
    return false;
}

void SharedMemoryListener::invalidatePoint()
{
    std::lock_guard<std::mutex> lock(mtx);

    if (mappedPoint) 
    {
        mappedPoint->x = -1; // 设置为非法值
        mappedPoint->y = -1; // 设置为非法值
        
    }
}